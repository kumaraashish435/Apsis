namespace Apsis.Api.Propagation.Sgp4;

/// <summary>
/// SGP4/SDP4 propagation — Product 1 Sprint 1-2. Deliberately in-process,
/// not a separate C++/gRPC engine — see ADR-0001 and
/// docs/architecture/aspis-simplification-review.md item #1.
///
/// Correctness contract: every implementation must pass the golden-file
/// suite in Apsis.Api.Tests/Propagation, checked against Vallado's published
/// SGP4 reference test vectors. Nothing in this namespace ships without that
/// suite passing — see docs/physics/ for the source vectors and tolerances.
/// </summary>
public interface ISgp4Propagator
{
    /// <summary>Propagates a TLE to a series of state vectors over the given timespan.</summary>
    IReadOnlyList<StateVector> Propagate(string tleLine1, string tleLine2, DateTimeOffset start, DateTimeOffset end, TimeSpan step);
}

public readonly record struct StateVector(DateTimeOffset Epoch, double X, double Y, double Z, double Vx, double Vy, double Vz);
