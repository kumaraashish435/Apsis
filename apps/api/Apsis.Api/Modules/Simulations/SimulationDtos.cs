using System.ComponentModel.DataAnnotations;
using Apsis.Api.Propagation;
using Apsis.Api.Propagation.Sgp4;

namespace Apsis.Api.Modules.Simulations;

public record SaveSimulationRequest
{
    [Required, MinLength(1)]
    public required string Name { get; init; }

    [Required]
    public required SimulationInputType InputType { get; init; }

    public string? TleLine1 { get; init; }
    public string? TleLine2 { get; init; }

    public double? SemiMajorAxisKm { get; init; }
    public double? Eccentricity { get; init; }
    public double? InclinationDeg { get; init; }
    public double? RaanDeg { get; init; }
    public double? ArgPerigeeDeg { get; init; }
    public double? TrueAnomalyDeg { get; init; }

    [Required]
    public required DateTimeOffset StartTime { get; init; }

    [Required]
    public required DateTimeOffset EndTime { get; init; }

    [Required, Range(1, int.MaxValue)]
    public required int StepSeconds { get; init; }
}

public record SimulationResponse
{
    public required Guid Id { get; init; }
    public required string Name { get; init; }
    public required SimulationInputType InputType { get; init; }
    public string? TleLine1 { get; init; }
    public string? TleLine2 { get; init; }
    public double? SemiMajorAxisKm { get; init; }
    public double? Eccentricity { get; init; }
    public double? InclinationDeg { get; init; }
    public double? RaanDeg { get; init; }
    public double? ArgPerigeeDeg { get; init; }
    public double? TrueAnomalyDeg { get; init; }
    public required DateTimeOffset StartTime { get; init; }
    public required DateTimeOffset EndTime { get; init; }
    public required int StepSeconds { get; init; }
    public required bool IsPublic { get; init; }
    /// <summary>Only populated for the owner's own view — the public/shared endpoints never echo this back.</summary>
    public Guid? ShareToken { get; init; }
    public required DateTimeOffset CreatedAt { get; init; }

    public static SimulationResponse FromEntity(Simulation s, bool includeShareToken) => new()
    {
        Id = s.Id,
        Name = s.Name,
        InputType = s.InputType,
        TleLine1 = s.TleLine1,
        TleLine2 = s.TleLine2,
        SemiMajorAxisKm = s.SemiMajorAxisKm,
        Eccentricity = s.Eccentricity,
        InclinationDeg = s.InclinationDeg,
        RaanDeg = s.RaanDeg,
        ArgPerigeeDeg = s.ArgPerigeeDeg,
        TrueAnomalyDeg = s.TrueAnomalyDeg,
        StartTime = s.StartTime,
        EndTime = s.EndTime,
        StepSeconds = s.StepSeconds,
        IsPublic = s.IsPublic,
        ShareToken = includeShareToken ? s.ShareToken : null,
        CreatedAt = s.CreatedAt,
    };
}

public record StateVectorResponse
{
    public required double EpochSeconds { get; init; }
    public required double XKm { get; init; }
    public required double YKm { get; init; }
    public required double ZKm { get; init; }
    public required double VxKmS { get; init; }
    public required double VyKmS { get; init; }
    public required double VzKmS { get; init; }
    public required double LatDeg { get; init; }
    public required double LonDeg { get; init; }

    public static StateVectorResponse FromStateVector(StateVector state, DateTimeOffset start)
    {
        var (lat, lon) = GeodeticConverter.ToGroundTrack(state);
        return new StateVectorResponse
        {
            EpochSeconds = (state.Epoch - start).TotalSeconds,
            XKm = state.X,
            YKm = state.Y,
            ZKm = state.Z,
            VxKmS = state.Vx,
            VyKmS = state.Vy,
            VzKmS = state.Vz,
            LatDeg = lat,
            LonDeg = lon,
        };
    }
}

public record PropagateResponse
{
    public required IReadOnlyList<StateVectorResponse> States { get; init; }
    /// <summary>Parallel to States — the osculating classical elements at each sample. See OrbitalElementsCalculator.cs.</summary>
    public required IReadOnlyList<OrbitalElementsReadout> Elements { get; init; }
}

public record DivergencePoint
{
    public required double EpochSeconds { get; init; }
    public required double DistanceKm { get; init; }
}

public record CompareResponse
{
    public required IReadOnlyList<StateVectorResponse> Sgp4States { get; init; }
    public required IReadOnlyList<StateVectorResponse> KeplerianStates { get; init; }
    public required IReadOnlyList<DivergencePoint> Divergence { get; init; }
}

public record ShareResponse
{
    public required bool IsPublic { get; init; }
    public required Guid ShareToken { get; init; }
}
