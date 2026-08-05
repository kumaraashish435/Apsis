using Apsis.Api.Propagation.Sgp4;

namespace Apsis.Api.Propagation;

/// <summary>
/// The "orbital parameter readout" feature: given a single propagated state
/// vector (position + velocity), recovers the classical orbital elements that
/// describe it — the inverse of what Keplerian/KeplerianPropagator.cs does.
/// Standard algorithm (Vallado, "rv2coe"): build the specific angular momentum,
/// node, and eccentricity vectors from r and v, then read each element off of
/// them. Works on SGP4 output too — the elements it returns are the
/// *osculating* elements for that instant, not the mean elements the TLE was
/// built from, which is why they drift slightly from one time step to the next
/// even though the underlying orbit is "the same" — see README.md.
/// </summary>
public static class OrbitalElementsCalculator
{
    private const double MuKm3S2 = 398600.4418;
    private const double EarthRadiusKm = 6378.137; // WGS84 equatorial radius — see GeodeticConverter for why this differs from the 6371 mean radius used elsewhere.
    private const double Tolerance = 1e-10;

    public static OrbitalElementsReadout FromStateVector(StateVector state)
    {
        var r = new Vec3(state.X, state.Y, state.Z);
        var v = new Vec3(state.Vx, state.Vy, state.Vz);

        var rMag = r.Length;
        var vMag = v.Length;

        var h = Vec3.Cross(r, v); // specific angular momentum
        var hMag = h.Length;

        var n = Vec3.Cross(new Vec3(0, 0, 1), h); // node vector (points toward ascending node)
        var nMag = n.Length;

        // Eccentricity vector — points from focus toward perigee.
        var eVec = ((vMag * vMag - MuKm3S2 / rMag) * r - Vec3.Dot(r, v) * v) / MuKm3S2;
        var e = eVec.Length;

        var specificEnergy = vMag * vMag / 2 - MuKm3S2 / rMag;
        var a = Math.Abs(1 - e) > Tolerance ? -MuKm3S2 / (2 * specificEnergy) : rMag; // parabolic fallback, not expected in practice (validated at input)

        var inclination = Math.Acos(Clamp(h.Z / hMag));

        // RAAN and argument of perigee are undefined (divide-by-zero) for
        // equatorial (nMag≈0) or circular (e≈0) orbits respectively — a known,
        // documented limitation (same category as SGP4's own near-singular
        // edge cases), not silently wrong: both default to 0 rather than NaN.
        double raan = 0;
        if (nMag > Tolerance)
        {
            raan = Math.Acos(Clamp(n.X / nMag));
            if (n.Y < 0)
            {
                raan = 2 * Math.PI - raan;
            }
        }

        double argPerigee = 0;
        if (nMag > Tolerance && e > Tolerance)
        {
            argPerigee = Math.Acos(Clamp(Vec3.Dot(n, eVec) / (nMag * e)));
            if (eVec.Z < 0)
            {
                argPerigee = 2 * Math.PI - argPerigee;
            }
        }

        double trueAnomaly;
        if (e > Tolerance)
        {
            trueAnomaly = Math.Acos(Clamp(Vec3.Dot(eVec, r) / (e * rMag)));
            if (Vec3.Dot(r, v) < 0)
            {
                trueAnomaly = 2 * Math.PI - trueAnomaly;
            }
        }
        else
        {
            // Circular orbit: no perigee to measure from — fall back to angle from the node (argument of latitude).
            trueAnomaly = nMag > Tolerance ? Math.Acos(Clamp(Vec3.Dot(n, r) / (nMag * rMag))) : Math.Acos(Clamp(r.X / rMag));
            if (r.Z < 0)
            {
                trueAnomaly = 2 * Math.PI - trueAnomaly;
            }
        }

        var periodSeconds = 2 * Math.PI * Math.Sqrt(a * a * a / MuKm3S2);

        return new OrbitalElementsReadout(
            AltitudeKm: rMag - EarthRadiusKm,
            SemiMajorAxisKm: a,
            Eccentricity: e,
            InclinationDeg: RadToDeg(inclination),
            RaanDeg: RadToDeg(raan),
            ArgPerigeeDeg: RadToDeg(argPerigee),
            TrueAnomalyDeg: RadToDeg(trueAnomaly),
            PeriodSeconds: periodSeconds,
            ApogeeAltitudeKm: a * (1 + e) - EarthRadiusKm,
            PerigeeAltitudeKm: a * (1 - e) - EarthRadiusKm);
    }

    private static double Clamp(double cosineValue) => Math.Clamp(cosineValue, -1.0, 1.0);
    private static double RadToDeg(double rad) => rad * 180.0 / Math.PI;

    private readonly record struct Vec3(double X, double Y, double Z)
    {
        public double Length => Math.Sqrt(X * X + Y * Y + Z * Z);

        public static Vec3 Cross(Vec3 a, Vec3 b) => new(
            a.Y * b.Z - a.Z * b.Y,
            a.Z * b.X - a.X * b.Z,
            a.X * b.Y - a.Y * b.X);

        public static double Dot(Vec3 a, Vec3 b) => a.X * b.X + a.Y * b.Y + a.Z * b.Z;

        public static Vec3 operator -(Vec3 a, Vec3 b) => new(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
        public static Vec3 operator *(double s, Vec3 a) => new(s * a.X, s * a.Y, s * a.Z);
        public static Vec3 operator /(Vec3 a, double s) => new(a.X / s, a.Y / s, a.Z / s);
    }
}

/// <summary>Altitude is instantaneous (current |r| - Earth radius); the rest are classical elements, ~constant for Keplerian orbits and slowly-varying (osculating) for SGP4 ones.</summary>
public record OrbitalElementsReadout(
    double AltitudeKm,
    double SemiMajorAxisKm,
    double Eccentricity,
    double InclinationDeg,
    double RaanDeg,
    double ArgPerigeeDeg,
    double TrueAnomalyDeg,
    double PeriodSeconds,
    double ApogeeAltitudeKm,
    double PerigeeAltitudeKm);
