# Propagation/Sgp4 — Product 1, active, highest-priority correctness work

Full product spec: `docs/products/product-1-orbital-simulator.md` (Sprint 1-2).

## What this module does

Propagates a two-line element set (TLE) to a series of position/velocity state vectors over time using SGP4/SDP4 — the industry-standard propagator for near-Earth objects. This is the single piece of code every other product in the company eventually depends on, directly or indirectly: Product 2's pass prediction, Product 3-4's orbital state for simulated telemetry, all trace back to this being correct.

## How it works today

`ISgp4Propagator` defines the contract (`Propagate(tleLine1, tleLine2, start, end, step) -> IReadOnlyList<StateVector>`) and `StateVector` is the shared shape (`Epoch, X, Y, Z, Vx, Vy, Vz`) that `Propagation/Keplerian`'s comparison output also uses, specifically so the two can be diffed directly. **No implementation exists yet** — this is the literal next thing to build.

## How it's meant to work once implemented

1. Implement `ISgp4Propagator` — either a from-scratch implementation of Vallado's SGP4/SDP4 algorithm, or a well-vetted existing .NET port wrapped behind this interface (either is fine; what's not fine is skipping the validation step below regardless of which you choose).
2. **Before this is trusted by anything else in the codebase**, it must pass the golden-file suite in `Apsis.Api.Tests/Propagation` — see that folder's README for the process. This is not optional or a "nice to have for later" — every downstream product's correctness assumption rests on this step actually happening.
3. Register `ISgp4Propagator` in `Program.cs`'s DI container once implemented; `Modules/Simulations`' `SimulationsController` consumes it directly, in-process (no gRPC — see `docs/architecture/adr/ADR-0001-in-process-propagation.md`).

## Process: what "done" means for this module

Not "it compiles" and not "it returns plausible-looking numbers." Done means: it matches Vallado's published SGP4 reference test vectors within their stated tolerance, for both near-Earth (SGP4) and deep-space (SDP4) test cases. If you only test near-Earth orbits, deep-space satellites (period > 225 minutes) will silently propagate incorrectly — this is a well-known SGP4 pitfall, not a hypothetical one.

## Deliberately out of scope here

No numerical/perturbation-theory propagator (J2-J6, drag, SRP) — that's Product 6, and it's a different algorithm family (Cowell's method via Orekit), not an extension of this module.
