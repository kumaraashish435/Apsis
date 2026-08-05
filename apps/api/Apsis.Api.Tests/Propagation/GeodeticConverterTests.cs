using Apsis.Api.Propagation;
using Apsis.Api.Propagation.Sgp4;

namespace Apsis.Api.Tests.Propagation;

public class GeodeticConverterTests
{
    [Fact]
    public void Point_on_z_axis_is_the_north_pole()
    {
        var state = new StateVector(DateTimeOffset.UtcNow, 0, 0, 7000, 0, 0, 0);
        var (lat, _) = GeodeticConverter.ToGroundTrack(state);
        Assert.Equal(90.0, lat, 6);
    }

    [Fact]
    public void Point_on_negative_z_axis_is_the_south_pole()
    {
        var state = new StateVector(DateTimeOffset.UtcNow, 0, 0, -7000, 0, 0, 0);
        var (lat, _) = GeodeticConverter.ToGroundTrack(state);
        Assert.Equal(-90.0, lat, 6);
    }

    [Fact]
    public void Equatorial_point_has_zero_latitude()
    {
        // Any point in the ECI xy-plane has zero declination regardless of Earth's
        // rotation (GMST only shifts longitude), so this holds for any epoch.
        var state = new StateVector(new DateTimeOffset(2026, 6, 15, 3, 47, 0, TimeSpan.Zero), 7000, 1234, 0, 0, 0, 0);
        var (lat, _) = GeodeticConverter.ToGroundTrack(state);
        Assert.Equal(0.0, lat, 6);
    }

    [Fact]
    public void Longitude_wraps_to_plus_minus_180()
    {
        var state = new StateVector(DateTimeOffset.UtcNow, -7000, -0.0001, 0, 0, 0, 0);
        var (_, lon) = GeodeticConverter.ToGroundTrack(state);
        Assert.InRange(lon, -180.0, 180.0);
    }
}
