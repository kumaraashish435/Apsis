using Apsis.Api.Propagation;
using Apsis.Api.Propagation.Keplerian;

namespace Apsis.Api.Tests.Propagation;

/// <summary>
/// Per this namespace's README: "a circular or simple elliptical orbit where
/// the analytical position at a given time can be computed by hand or with a
/// trusted reference tool, checked at a handful of time steps." Plus two
/// stronger, derivation-independent checks (conservation of specific energy
/// and specific angular momentum) that catch bugs the closed-form case alone
/// wouldn't — in particular, bugs in the perifocal-to-ECI rotation, which a
/// purely equatorial test case can't exercise (it has no inclination to
/// rotate through).
/// </summary>
public class KeplerianPropagatorTests
{
    private const double MuKm3S2 = 398600.4418;

    [Fact]
    public void Equatorial_circular_orbit_matches_closed_form_position()
    {
        const double aKm = 7000;
        var meanMotion = Math.Sqrt(MuKm3S2 / (aKm * aKm * aKm));
        var elements = new OrbitalElements(aKm, 0, 0, 0, 0, 0);
        var start = new DateTimeOffset(2026, 1, 1, 0, 0, 0, TimeSpan.Zero);

        var propagator = new KeplerianPropagator();
        var states = propagator.Propagate(elements, start, start.AddMinutes(120), TimeSpan.FromMinutes(10));

        foreach (var state in states)
        {
            var t = (state.Epoch - start).TotalSeconds;
            var expectedX = aKm * Math.Cos(meanMotion * t);
            var expectedY = aKm * Math.Sin(meanMotion * t);
            var expectedVx = -aKm * meanMotion * Math.Sin(meanMotion * t);
            var expectedVy = aKm * meanMotion * Math.Cos(meanMotion * t);

            AssertClose(expectedX, state.X, 1e-8, "x");
            AssertClose(expectedY, state.Y, 1e-8, "y");
            AssertClose(0, state.Z, 1e-8, "z");
            AssertClose(expectedVx, state.Vx, 1e-10, "vx");
            AssertClose(expectedVy, state.Vy, 1e-10, "vy");
            AssertClose(0, state.Vz, 1e-10, "vz");
        }
    }

    [Theory]
    [InlineData(7000, 0.01, 28.5, 40, 15)]
    [InlineData(26560, 0.2, 55, 120, 300)] // GPS-like, eccentric
    [InlineData(42164, 0.001, 0.1, 0, 0)] // near-equatorial GEO — exercises the RAAN singularity guard
    public void Specific_energy_and_angular_momentum_are_conserved(
        double aKm, double e, double inclDeg, double raanDeg, double argpDeg)
    {
        var elements = new OrbitalElements(aKm, e, inclDeg, raanDeg, argpDeg, 0);
        var start = new DateTimeOffset(2026, 1, 1, 0, 0, 0, TimeSpan.Zero);
        var periodSeconds = 2 * Math.PI * Math.Sqrt(aKm * aKm * aKm / MuKm3S2);

        var propagator = new KeplerianPropagator();
        var states = propagator.Propagate(elements, start, start.AddSeconds(periodSeconds), TimeSpan.FromSeconds(periodSeconds / 50));

        var expectedEnergy = -MuKm3S2 / (2 * aKm);
        var expectedAngularMomentum = Math.Sqrt(MuKm3S2 * aKm * (1 - e * e));

        foreach (var state in states)
        {
            var rMag = Math.Sqrt(state.X * state.X + state.Y * state.Y + state.Z * state.Z);
            var vMag = Math.Sqrt(state.Vx * state.Vx + state.Vy * state.Vy + state.Vz * state.Vz);
            var energy = vMag * vMag / 2 - MuKm3S2 / rMag;

            var hx = state.Y * state.Vz - state.Z * state.Vy;
            var hy = state.Z * state.Vx - state.X * state.Vz;
            var hz = state.X * state.Vy - state.Y * state.Vx;
            var hMag = Math.Sqrt(hx * hx + hy * hy + hz * hz);

            AssertClose(expectedEnergy, energy, 1e-6, "specific energy");
            AssertClose(expectedAngularMomentum, hMag, 1e-6, "specific angular momentum");
        }
    }

    [Theory]
    [InlineData(7000, 0.0, 0.0, 0.0, 0.0, 0.0)]
    [InlineData(7500, 0.05, 28.5, 40, 15, 200)]
    [InlineData(26560, 0.2, 55, 120, 300, 90)]
    [InlineData(42164, 0.001, 0.1, 0, 0, 0)]
    [InlineData(9000, 0.3, 98, 200, 250, 340)]
    public void Round_trips_through_OrbitalElementsCalculator(
        double aKm, double e, double inclDeg, double raanDeg, double argpDeg, double trueAnomalyDeg)
    {
        var elements = new OrbitalElements(aKm, e, inclDeg, raanDeg, argpDeg, trueAnomalyDeg);
        var start = new DateTimeOffset(2026, 1, 1, 0, 0, 0, TimeSpan.Zero);

        var propagator = new KeplerianPropagator();
        var state = propagator.Propagate(elements, start, start, TimeSpan.FromSeconds(1)).Single();

        var readout = OrbitalElementsCalculator.FromStateVector(state);

        AssertClose(aKm, readout.SemiMajorAxisKm, 1e-6, "semi-major axis");
        AssertClose(e, readout.Eccentricity, 1e-9, "eccentricity");
        AssertClose(inclDeg, readout.InclinationDeg, 1e-6, "inclination");
        AssertClose(NormalizeDeg(trueAnomalyDeg), NormalizeDeg(readout.TrueAnomalyDeg), 1e-5, "true anomaly");

        // RAAN/argp are only well-defined once the orbit isn't equatorial/circular
        // — the same singularities OrbitalElementsCalculator itself documents.
        if (inclDeg is not (0.0 or 180.0))
        {
            AssertClose(NormalizeDeg(raanDeg), NormalizeDeg(readout.RaanDeg), 1e-5, "RAAN");
        }
        if (e > 1e-8 && inclDeg is not (0.0 or 180.0))
        {
            AssertClose(NormalizeDeg(argpDeg), NormalizeDeg(readout.ArgPerigeeDeg), 1e-5, "argument of perigee");
        }
    }

    private static double NormalizeDeg(double deg)
    {
        var normalized = deg % 360.0;
        return normalized < 0 ? normalized + 360.0 : normalized;
    }

    private static void AssertClose(double expected, double actual, double tolerance, string label)
    {
        var diff = Math.Abs(expected - actual);
        Assert.True(diff <= tolerance, $"{label}: expected {expected}, got {actual}, diff {diff} > tolerance {tolerance}");
    }
}
