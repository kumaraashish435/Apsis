namespace Apsis.Api.Modules.Simulations;

public enum SimulationInputType
{
    Tle,
    Elements,
}

/// <summary>
/// A saved simulation: either a TLE (propagated with SGP4/SDP4) or a set of
/// classical orbital elements (propagated with the unperturbed Keplerian
/// model) plus a propagation window. Only the input is persisted — propagated
/// state vectors are always recomputed on read from Line1/Line2 or the
/// element fields, never stored, so there's nothing to keep in sync if the
/// propagator implementation ever changes.
/// </summary>
public class Simulation
{
    public Guid Id { get; set; } = Guid.NewGuid();

    public required Guid OwnerId { get; set; }

    public required string Name { get; set; }

    public SimulationInputType InputType { get; set; }

    // Populated when InputType == Tle.
    public string? TleLine1 { get; set; }
    public string? TleLine2 { get; set; }

    // Populated when InputType == Elements.
    public double? SemiMajorAxisKm { get; set; }
    public double? Eccentricity { get; set; }
    public double? InclinationDeg { get; set; }
    public double? RaanDeg { get; set; }
    public double? ArgPerigeeDeg { get; set; }
    public double? TrueAnomalyDeg { get; set; }

    /// <summary>For a TLE simulation this is an arbitrary propagation-window start, independent of the TLE's own epoch. For an Elements simulation, this *is* the epoch the elements are defined at (KeplerianPropagator has no separate epoch concept).</summary>
    public required DateTimeOffset StartTime { get; set; }
    public required DateTimeOffset EndTime { get; set; }
    public required int StepSeconds { get; set; }

    /// <summary>Always generated at creation; only resolvable via the public share endpoint once IsPublic is true.</summary>
    public Guid ShareToken { get; set; } = Guid.NewGuid();
    public bool IsPublic { get; set; }

    public DateTimeOffset CreatedAt { get; set; } = DateTimeOffset.UtcNow;
}
