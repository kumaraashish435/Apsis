# Propagation/Keplerian — Product 1, active, lower priority than Sgp4

Full product spec: `docs/products/product-1-orbital-simulator.md` (Sprint 7-8).

## What this module does

Simple two-body Keplerian propagation from classical orbital elements — used for exactly one purpose: the Comparison view, where a user sees how an idealized two-body orbit diverges from SGP4's perturbation-aware propagation over time. It is not a general-purpose alternative to `Propagation/Sgp4` and is never used by any other module.

## How it works today

`IKeplerianPropagator` defines the contract and `OrbitalElements` the input shape (semi-major axis, eccentricity, inclination, RAAN, argument of perigee, true anomaly). It deliberately returns the same `StateVector` shape `Propagation/Sgp4` uses, so `Modules/Simulations`' compare endpoint can diff the two series directly without a translation step. No implementation exists yet.

## How it's meant to work once implemented

Solve Kepler's equation for eccentric anomaly at each time step, convert to a position/velocity state vector in the perifocal frame, then rotate into the same inertial frame `Propagation/Sgp4` outputs. This is meaningfully simpler than SGP4 — no perturbation terms, no drag/SRP — and doesn't need the same depth of validation, though it should still be checked against a known analytical result for a simple circular/elliptical case.

## Process: why this is explicitly deferred, not part of the MVP critical path

The Comparison view is a differentiator, not a "does the core product work" feature — see `aspis-simplification-review.md` item #3. Don't build this before the Sprint 1-6 core loop (create satellite → propagate with SGP4 → view in 3D → save/share) is shipped and has real users. Building the comparison feature first would compete with sprints that prove whether anyone wants the product at all.
