# Propagation/Sgp4 — Product 1, active, highest-priority correctness work

Full product spec: `docs/products/product-1-orbital-simulator.md` (Sprint 1-2).

## What this module does

Propagates a two-line element set (TLE) to a series of position/velocity state vectors over time using SGP4/SDP4 — the industry-standard propagator for near-Earth objects. This is the single piece of code every other product in the company eventually depends on, directly or indirectly: Product 2's pass prediction, Product 3-4's orbital state for simulated telemetry, all trace back to this being correct.

## How it works today

| File | Role |
|---|---|
| `ISgp4Propagator.cs` | The contract — `Propagate(tleLine1, tleLine2, start, end, step) -> IReadOnlyList<StateVector>`. `StateVector` (`Epoch, X, Y, Z, Vx, Vy, Vz`, km and km/s, TEME frame) is the shared shape `Propagation/Keplerian`'s comparison output also uses. |
| `TLE.cs`, `ElsetRec.cs`, `SGP4.cs` | A pre-existing, faithful C# port of Vallado's reference SGP4/SDP4 algorithm (the `sgp4fix` comments throughout are Vallado's own). This is where the actual physics lives — over 1,300 lines, not something to rewrite casually. |
| `Sgp4Propagator.cs` | The `ISgp4Propagator` implementation. A thin adapter, not a reimplementation: it owns none of the physics, it just drives `TLE.getRV(minutesSinceEpoch)` in a loop and converts between this interface's wall-clock `DateTimeOffset` API and the ported code's "minutes since epoch" API. Throws `Sgp4PropagationException` (carrying Vallado's numeric error code) when the underlying algorithm refuses to propagate further — almost always orbital decay. |

Registered in `Program.cs` as `AddSingleton<ISgp4Propagator, Sgp4Propagator>()` — it's stateless, one instance is fine. **Not yet wired into `Modules/Simulations/SimulationsController`**, which still returns `501 Not Implemented` on `/propagate` — that's persistence/CRUD/DTO-shaping work, a separate task from proving the propagator itself is correct.

## Correctness — the golden-file suite

`Apsis.Api.Tests/Propagation/Sgp4VerificationTests.cs` checks this implementation against Vallado's own reference C++ output — see that folder's README for the full writeup, including one real, narrow, documented discrepancy the suite found (satellite 23599, a known "Lyddane choice" edge case) that's tracked rather than hidden.

## Deliberately out of scope here

No numerical/perturbation-theory propagator (J2-J6, drag, SRP) — that's Product 6, and it's a different algorithm family (Cowell's method via Orekit), not an extension of this module.
