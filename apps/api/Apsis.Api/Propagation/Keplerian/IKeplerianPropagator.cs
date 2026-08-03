using Apsis.Api.Propagation.Sgp4;

namespace Apsis.Api.Propagation.Keplerian;

/// <summary>
/// Simple two-body Keplerian propagation — Product 1 Sprint 7-8, used only
/// as the second series in the propagator Comparison view (not required for
/// the initial MVP launch). Shares the StateVector shape with ISgp4Propagator
/// so the comparison endpoint can diff the two series directly.
/// </summary>
public interface IKeplerianPropagator
{
    IReadOnlyList<StateVector> Propagate(OrbitalElements elements, DateTimeOffset start, DateTimeOffset end, TimeSpan step);
}

public readonly record struct OrbitalElements(
    double SemiMajorAxisKm,
    double Eccentricity,
    double InclinationDeg,
    double RaanDeg,
    double ArgPerigeeDeg,
    double TrueAnomalyDeg);
