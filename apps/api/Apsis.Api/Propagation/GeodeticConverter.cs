using Apsis.Api.Propagation.Sgp4;

namespace Apsis.Api.Propagation;

/// <summary>
/// Converts an inertial (ECI/TEME) state vector into ground-track coordinates —
/// latitude/longitude of the sub-satellite point — for the 2D ground-track view.
/// Two steps: rotate ECI into the Earth-fixed frame by Greenwich Mean Sidereal
/// Time (accounts for Earth's rotation under the satellite), then read
/// latitude/longitude off the rotated position on a spherical Earth. Spherical,
/// not WGS84-ellipsoidal — matches the 6371 km mean radius the 3D globe already
/// uses (apps/web's utils/constants.ts EARTH_RADIUS_KM), so the ground track
/// lines up with what the 3D view renders. Accurate to a few km on the ground,
/// which is well within what a visualization needs — this is not a geodesy tool.
/// </summary>
public static class GeodeticConverter
{
    private static readonly DateTimeOffset J2000Epoch = new(2000, 1, 1, 12, 0, 0, TimeSpan.Zero);

    public static (double LatDeg, double LonDeg) ToGroundTrack(StateVector state)
    {
        var gmstRad = GreenwichMeanSiderealTimeRad(state.Epoch);
        var cosG = Math.Cos(gmstRad);
        var sinG = Math.Sin(gmstRad);

        // Rotate ECI -> ECEF: R3(+GMST) applied as the inverse rotation of the
        // Earth-fixed frame relative to inertial space.
        var xEcef = cosG * state.X + sinG * state.Y;
        var yEcef = -sinG * state.X + cosG * state.Y;
        var zEcef = state.Z;

        var r = Math.Sqrt(xEcef * xEcef + yEcef * yEcef + zEcef * zEcef);
        var latDeg = Math.Asin(Math.Clamp(zEcef / r, -1.0, 1.0)) * 180.0 / Math.PI;
        var lonDeg = Math.Atan2(yEcef, xEcef) * 180.0 / Math.PI;

        return (latDeg, lonDeg);
    }

    /// <summary>Vallado's simplified GMST formula (IAU 1982), accurate to well under a second — plenty for a visualization.</summary>
    private static double GreenwichMeanSiderealTimeRad(DateTimeOffset at)
    {
        var julianCenturiesSinceJ2000 = (at - J2000Epoch).TotalDays / 36525.0;
        var t = julianCenturiesSinceJ2000;

        var gmstDeg = 280.46061837
            + 360.98564736629 * (at - J2000Epoch).TotalDays
            + 0.000387933 * t * t
            - t * t * t / 38710000.0;

        gmstDeg %= 360.0;
        if (gmstDeg < 0)
        {
            gmstDeg += 360.0;
        }

        return gmstDeg * Math.PI / 180.0;
    }
}
