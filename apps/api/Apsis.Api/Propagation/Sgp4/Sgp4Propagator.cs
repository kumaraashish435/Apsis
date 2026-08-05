namespace Apsis.Api.Propagation.Sgp4;

/// <summary>
/// Thin adapter over the ported <see cref="TLE"/>/<see cref="SGP4"/> algorithm,
/// translating its "minutes since epoch" API into the interface's
/// wall-clock-time API. All the actual physics lives in SGP4.cs — this class
/// owns none of it, it just drives the loop and converts units/time.
/// Correctness is proven by the golden-file suite in
/// Apsis.Api.Tests/Propagation/Sgp4VerificationTests.cs — see that file and
/// this namespace's README before changing anything here.
/// </summary>
public class Sgp4Propagator : ISgp4Propagator
{
    public IReadOnlyList<StateVector> Propagate(
        string tleLine1,
        string tleLine2,
        DateTimeOffset start,
        DateTimeOffset end,
        TimeSpan step)
    {
        ArgumentNullException.ThrowIfNull(tleLine1);
        ArgumentNullException.ThrowIfNull(tleLine2);
        if (step <= TimeSpan.Zero)
        {
            throw new ArgumentOutOfRangeException(nameof(step), "step must be positive.");
        }
        if (end < start)
        {
            throw new ArgumentOutOfRangeException(nameof(end), "end must not be before start.");
        }

        var tle = new TLE(tleLine1, tleLine2);
        if (!string.IsNullOrEmpty(tle.getParseErrors()))
        {
            throw new ArgumentException($"Invalid TLE: {tle.getParseErrors()}");
        }

        var epoch = new DateTimeOffset(tle.getEpoch());
        var results = new List<StateVector>();

        for (var t = start; t <= end; t += step)
        {
            var minutesSinceEpoch = (t - epoch).TotalMinutes;
            var rv = tle.getRV(minutesSinceEpoch);

            if (tle.getSgp4Error() != 0)
            {
                throw new Sgp4PropagationException(tle.getSgp4Error(), t);
            }

            var r = rv[0];
            var v = rv[1];
            results.Add(new StateVector(t, r[0], r[1], r[2], v[0], v[1], v[2]));
        }

        return results;
    }
}

/// <summary>
/// SGP4/SDP4 refused to propagate further — almost always because the orbit
/// has decayed (perigee below the Earth's surface) or the input elements are
/// degenerate. <paramref name="ErrorCode"/> matches Vallado's sgp4() codes:
/// 1 = mean eccentricity out of range, 2 = mean motion &lt; 0,
/// 3 = perturbed eccentricity out of range, 4 = semi-latus rectum &lt; 0,
/// 5 = epoch elements are sub-orbital, 6 = satellite has decayed.
/// </summary>
public class Sgp4PropagationException(int errorCode, DateTimeOffset at)
    : Exception($"SGP4 propagation failed with error code {errorCode} at {at:o}.")
{
    public int ErrorCode { get; } = errorCode;
    public DateTimeOffset At { get; } = at;
}
