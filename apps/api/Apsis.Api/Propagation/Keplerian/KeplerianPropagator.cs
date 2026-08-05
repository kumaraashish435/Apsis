using Apsis.Api.Propagation.Sgp4;

namespace Apsis.Api.Propagation.Keplerian;

/// <summary>
/// Unperturbed two-body ("Keplerian") propagation — no J2, no drag, no third-body
/// effects. Deliberately simple: this exists specifically so the Comparison view
/// can show how much SGP4's perturbation terms matter by diffing against an orbit
/// that ignores them, not to be an accurate propagator in its own right. Standard
/// algorithm (Vallado, "Fundamentals of Astrodynamics and Applications"): Kepler's
/// equation solved by Newton-Raphson, position/velocity built in the perifocal
/// (PQW) frame, then rotated into ECI by the classical 3-1-3 Euler sequence
/// (R3(-RAAN) · R1(-i) · R3(-argp)). Correctness is checked in
/// Apsis.Api.Tests/Propagation/KeplerianPropagatorTests.cs against closed-form
/// circular-orbit positions and, for the general case, conservation of specific
/// energy and specific angular momentum — see that file's comments.
/// </summary>
public class KeplerianPropagator : IKeplerianPropagator
{
    // Earth's gravitational parameter, km^3/s^2 (same constant magnitude SGP4 uses
    // internally via WGS72's xke/tumin, kept here as a plain literal since this
    // propagator doesn't share any code with the SGP4 port).
    private const double MuKm3S2 = 398600.4418;

    public IReadOnlyList<StateVector> Propagate(OrbitalElements elements, DateTimeOffset start, DateTimeOffset end, TimeSpan step)
    {
        if (step <= TimeSpan.Zero)
        {
            throw new ArgumentOutOfRangeException(nameof(step), "step must be positive.");
        }
        if (end < start)
        {
            throw new ArgumentOutOfRangeException(nameof(end), "end must not be before start.");
        }
        if (elements.SemiMajorAxisKm <= 0)
        {
            throw new ArgumentException("SemiMajorAxisKm must be positive.", nameof(elements));
        }
        if (elements.Eccentricity is < 0 or >= 1)
        {
            throw new ArgumentException("Eccentricity must be in [0, 1) — parabolic/hyperbolic orbits aren't supported.", nameof(elements));
        }

        var a = elements.SemiMajorAxisKm;
        var e = elements.Eccentricity;
        var incl = DegToRad(elements.InclinationDeg);
        var raan = DegToRad(elements.RaanDeg);
        var argp = DegToRad(elements.ArgPerigeeDeg);

        var meanMotion = Math.Sqrt(MuKm3S2 / (a * a * a)); // rad/s
        var e0 = TrueToEccentricAnomaly(DegToRad(elements.TrueAnomalyDeg), e);
        var m0 = e0 - e * Math.Sin(e0); // mean anomaly at epoch (start)

        // Rotation from perifocal (PQW) to ECI — built once, reused every step.
        var (r11, r12, r21, r22, r31, r32) = PerifocalToEciRotation(raan, incl, argp);

        var results = new List<StateVector>();
        for (var t = start; t <= end; t += step)
        {
            var dtSeconds = (t - start).TotalSeconds;
            var meanAnomaly = m0 + meanMotion * dtSeconds;
            var eccentricAnomaly = SolveKeplersEquation(meanAnomaly, e);

            var cosE = Math.Cos(eccentricAnomaly);
            var sinE = Math.Sin(eccentricAnomaly);
            var rMag = a * (1 - e * cosE);

            // Position and velocity in the perifocal frame (P along eccentricity
            // vector toward perigee, Q 90° ahead in the orbit plane).
            var pP = a * (cosE - e);
            var pQ = a * Math.Sqrt(1 - e * e) * sinE;
            var vFactor = Math.Sqrt(MuKm3S2 * a) / rMag;
            var vP = -vFactor * sinE;
            var vQ = vFactor * Math.Sqrt(1 - e * e) * cosE;

            var x = r11 * pP + r12 * pQ;
            var y = r21 * pP + r22 * pQ;
            var z = r31 * pP + r32 * pQ;
            var vx = r11 * vP + r12 * vQ;
            var vy = r21 * vP + r22 * vQ;
            var vz = r31 * vP + r32 * vQ;

            results.Add(new StateVector(t, x, y, z, vx, vy, vz));
        }

        return results;
    }

    private static double TrueToEccentricAnomaly(double trueAnomaly, double e)
    {
        var y = Math.Sqrt(1 - e) * Math.Sin(trueAnomaly / 2);
        var x = Math.Sqrt(1 + e) * Math.Cos(trueAnomaly / 2);
        return 2 * Math.Atan2(y, x);
    }

    private static double SolveKeplersEquation(double meanAnomaly, double e)
    {
        // Wrap to [-pi, pi] so Newton-Raphson starts near the root even for large |M|.
        var m = Math.IEEERemainder(meanAnomaly, 2 * Math.PI);
        var eAnom = e < 0.8 ? m : Math.PI; // standard starting guess

        for (var i = 0; i < 50; i++)
        {
            var delta = (eAnom - e * Math.Sin(eAnom) - m) / (1 - e * Math.Cos(eAnom));
            eAnom -= delta;
            if (Math.Abs(delta) < 1e-12)
            {
                break;
            }
        }

        return eAnom;
    }

    private static (double r11, double r12, double r21, double r22, double r31, double r32)
        PerifocalToEciRotation(double raan, double incl, double argp)
    {
        var cosRaan = Math.Cos(raan);
        var sinRaan = Math.Sin(raan);
        var cosIncl = Math.Cos(incl);
        var sinIncl = Math.Sin(incl);
        var cosArgp = Math.Cos(argp);
        var sinArgp = Math.Sin(argp);

        // R3(-raan) * R1(-incl) * R3(-argp), applied to the perifocal (P, Q) basis
        // vectors only — the perifocal frame's W axis is never populated (2D motion
        // in the orbit plane), so only the columns that multiply P and Q are needed.
        var r11 = cosRaan * cosArgp - sinRaan * sinArgp * cosIncl;
        var r12 = -cosRaan * sinArgp - sinRaan * cosArgp * cosIncl;
        var r21 = sinRaan * cosArgp + cosRaan * sinArgp * cosIncl;
        var r22 = -sinRaan * sinArgp + cosRaan * cosArgp * cosIncl;
        var r31 = sinArgp * sinIncl;
        var r32 = cosArgp * sinIncl;

        return (r11, r12, r21, r22, r31, r32);
    }

    private static double DegToRad(double deg) => deg * Math.PI / 180.0;
}
