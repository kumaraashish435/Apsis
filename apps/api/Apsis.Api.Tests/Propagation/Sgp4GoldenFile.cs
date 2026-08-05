using System.Globalization;

namespace Apsis.Api.Tests.Propagation;

/// <summary>
/// Parses the two files in testdata/, both taken verbatim from Vallado's
/// official C++ SGP4 distribution (fetched from the widely-used
/// brandon-rhodes/python-sgp4 mirror, sgp4/SGP4-VER.TLE + sgp4/tcppver.out —
/// the same pair virtually every SGP4 reimplementation validates against).
/// See ../README.md for how these two files relate and why they're trusted.
/// </summary>
public static class Sgp4GoldenFile
{
    private static readonly string DataDir = Path.Combine(AppContext.BaseDirectory, "Propagation", "testdata");

    public record TleCase(int SatelliteNumber, string Line1, string Line2, double StartMin, double StopMin, double StepMin);

    public record ExpectedState(double TsinceMinutes, double Rx, double Ry, double Rz, double Vx, double Vy, double Vz);

    public record VerificationCase(TleCase Tle, IReadOnlyList<ExpectedState> States);

    /// <summary>
    /// Both files list the same 33 test satellites in the same order (satellite
    /// 20413 appears twice — once over a short span, once over a ~3.5-year
    /// stress-test span — so cases are paired positionally, not by satellite
    /// number, which wouldn't be unique).
    /// </summary>
    public static IReadOnlyList<VerificationCase> LoadAll()
    {
        var tles = ParseTleFile(Path.Combine(DataDir, "SGP4-VER.TLE"));
        var blocks = ParseOutputFile(Path.Combine(DataDir, "tcppver.out"));

        if (tles.Count != blocks.Count)
        {
            throw new InvalidOperationException(
                $"Golden file mismatch: {tles.Count} TLE entries in SGP4-VER.TLE but {blocks.Count} blocks in tcppver.out.");
        }

        var cases = new List<VerificationCase>(tles.Count);
        for (var i = 0; i < tles.Count; i++)
        {
            cases.Add(new VerificationCase(tles[i], blocks[i]));
        }

        return cases;
    }

    private static List<TleCase> ParseTleFile(string path)
    {
        var cases = new List<TleCase>();
        string? line1 = null;

        foreach (var raw in File.ReadLines(path))
        {
            if (raw.Length == 0 || raw[0] == '#')
            {
                continue;
            }

            if (raw.StartsWith("1 ", StringComparison.Ordinal))
            {
                line1 = raw;
            }
            else if (raw.StartsWith("2 ", StringComparison.Ordinal) && line1 is not null)
            {
                var satNum = int.Parse(line1.Substring(2, 5).Trim(), CultureInfo.InvariantCulture);

                // The trailing 3 whitespace-separated tokens are always start/stop/step
                // (in minutes since epoch) appended after the standard 69-column TLE —
                // taken from the end because earlier columns tokenize inconsistently
                // (some fields glue together with no space, some don't).
                var tokens = raw.Split(' ', StringSplitOptions.RemoveEmptyEntries);
                var stepMin = double.Parse(tokens[^1], CultureInfo.InvariantCulture);
                var stopMin = double.Parse(tokens[^2], CultureInfo.InvariantCulture);
                var startMin = double.Parse(tokens[^3], CultureInfo.InvariantCulture);

                cases.Add(new TleCase(satNum, line1, raw, startMin, stopMin, stepMin));
                line1 = null;
            }
        }

        return cases;
    }

    private static List<List<ExpectedState>> ParseOutputFile(string path)
    {
        var blocks = new List<List<ExpectedState>>();
        List<ExpectedState>? current = null;

        foreach (var raw in File.ReadLines(path))
        {
            var line = raw.Trim();
            if (line.Length == 0)
            {
                continue;
            }

            var tokens = line.Split(' ', StringSplitOptions.RemoveEmptyEntries);

            // Block headers look like "5 xx" or "20413 xx" — exactly 2 tokens, second is "xx".
            if (tokens.Length == 2 && tokens[1] == "xx")
            {
                current = [];
                blocks.Add(current);
                continue;
            }

            // Data rows have >= 7 tokens: tsince, rx, ry, rz, vx, vy, vz, [diagnostic columns...].
            if (current is null || tokens.Length < 7)
            {
                continue;
            }

            current.Add(new ExpectedState(
                double.Parse(tokens[0], CultureInfo.InvariantCulture),
                double.Parse(tokens[1], CultureInfo.InvariantCulture),
                double.Parse(tokens[2], CultureInfo.InvariantCulture),
                double.Parse(tokens[3], CultureInfo.InvariantCulture),
                double.Parse(tokens[4], CultureInfo.InvariantCulture),
                double.Parse(tokens[5], CultureInfo.InvariantCulture),
                double.Parse(tokens[6], CultureInfo.InvariantCulture)));
        }

        return blocks;
    }
}
