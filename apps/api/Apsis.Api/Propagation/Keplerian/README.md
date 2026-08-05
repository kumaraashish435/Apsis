# Propagation/Keplerian — Product 1, active

Full product spec: `docs/products/product-1-orbital-simulator.md` (Sprint 7-8).

## What this module does

Unperturbed two-body ("Keplerian") propagation from classical orbital elements. Two consumers, not one: it's the propagator for any simulation created directly from classical elements (`SimulationInputType.Elements` — no TLE, no SGP4 involved at all), and it's the second series in the Comparison view, where a user sees how much SGP4's perturbation terms (drag, J2) actually matter by diffing against an orbit that ignores them entirely.

## How it works today

`IKeplerianPropagator.Propagate(elements, start, end, step) -> IReadOnlyList<StateVector>` — same signature shape as `ISgp4Propagator`, deliberately: it returns the identical `StateVector` record `Propagation/Sgp4` uses, so `Modules/Simulations`' compare endpoint can diff the two series directly with no translation step, and so `SimulationsController` can treat "propagate this simulation" as a two-branch `if` on `InputType` with no other code caring which propagator actually ran.

`KeplerianPropagator.cs` implements it: solve Kepler's equation (`M = E - e·sin E`) for eccentric anomaly at each time step via Newton-Raphson, build position/velocity in the perifocal (PQW) frame, then rotate into ECI by the classical 3-1-3 Euler sequence (`R3(-RAAN) · R1(-i) · R3(-argp)`) — standard algorithm, Vallado's *Fundamentals of Astrodynamics and Applications*.

### Why these choices

- **No perturbation terms, on purpose.** Adding J2/drag here would make it a second, worse SGP4 — the entire point is that it *doesn't* perturb, so the Comparison view's divergence line is showing something real (what SGP4's physics adds) rather than two slightly-different-but-both-approximate models.
- **Newton-Raphson with a 1e-12 convergence tolerance and a 50-iteration cap**, not a fixed iteration count. Kepler's equation has no closed-form solution; Newton-Raphson converges in single digits of iterations for any orbit this product accepts (`Eccentricity` is validated to `[0, 1)` before propagation starts), so the cap is a safety net, not something normal input ever hits.

## Correctness

`Apsis.Api.Tests/Propagation/KeplerianPropagatorTests.cs` checks three independent ways, per this namespace's own testing philosophy (see `Propagation/Sgp4/README.md` for why independent verification matters more than "it compiles"):

1. **Closed-form match** — an equatorial circular orbit has an exact analytical position (`a·cos(nt)`, `a·sin(nt)`) with no propagator logic involved in computing the expected value; checked to 1e-8 km.
2. **Conservation laws** — specific orbital energy and specific angular momentum magnitude are analytically constant for *any* unperturbed orbit. Checked across a full period for three very different orbits (near-circular LEO, eccentric GPS-like, near-equatorial GEO — the last one specifically exercises the RAAN singularity guard in `OrbitalElementsCalculator`). This check is derivation-independent of the propagator's own math, so it catches classes of bug (e.g. a rotation-matrix sign error) that the closed-form case can't, since that case has zero inclination to rotate through.
3. **Round-trip through `OrbitalElementsCalculator`** — propagate elements to a state vector, convert that state vector back to elements, assert the result matches the input. Proves the propagator and the elements calculator (`Propagation/OrbitalElementsCalculator.cs`) agree with each other, across five geometries including the two singular cases (equatorial, circular).

All of the above pass. See `apps/api/Apsis.Api.Tests/Propagation/README.md` for the full suite's current pass count.

## Process: what NOT to add here

Resist the urge to add J2 "just for a bit more realism" — the moment this module perturbs at all, it stops being a useful comparison baseline for SGP4 and just becomes a second, less-validated propagator. If a *third* propagation model is ever wanted (e.g. numerical integration with a real force model), that's a new module, not an extension of this one.
