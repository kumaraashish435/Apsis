# Modules/Simulations — Product 1, active

Full product spec: `docs/products/product-1-orbital-simulator.md`.

## What this module does

CRUD for saved simulations, plus the endpoints that trigger propagation, propagator comparison, and export. This is the core surface a user actually interacts with in Product 1 — everything else in this product (auth, the frontend pages) exists to support this module.

## How it works today

`SimulationsController` exposes the route shape (`GET/POST /api/v1/simulations`, `GET .../{id}/propagate`, `.../compare`, `.../export`) but every action beyond `List()` returns `501 Not Implemented`. There is no `Simulation`/`Satellite` EF Core entity or Postgres table yet.

## How it's meant to work once built out (Sprint 3-8)

1. **Sprint 3-4:** add `Simulation` and `Satellite` EF Core entities (see `docs/products/product-1-orbital-simulator.md`'s Database section for the exact shape), wire up `POST /api/v1/simulations` to persist a satellite's TLE/orbital elements. Note what's *not* persisted: propagated state vectors are never stored, only recomputed on read — see the Data Flow note in the product doc.
2. **Sprint 1-2 dependency:** `GET .../{id}/propagate` calls `Propagation/Sgp4`'s `ISgp4Propagator` directly, in-process — no network hop, no message queue. That interface needs to already be implemented and golden-file-tested (see `Propagation/Sgp4/README.md`) before this endpoint means anything.
3. **Sprint 7-8:** `GET .../{id}/compare` calls both `ISgp4Propagator` and `Propagation/Keplerian`'s `IKeplerianPropagator` against the same inputs and returns both series for the frontend's divergence plot. This is a fast-follow, not required for the initial 8-week launch.
4. `GET .../{id}/export` serializes the propagated series to CSV/JSON first; CZML (Cesium's format) is lower priority — see `aspis-simplification-review.md` item #3.

## Process: how a request flows through this module

`SimulationEditor` (frontend) → `POST /api/v1/simulations` (persist inputs only) → user requests a view → `GET .../{id}/propagate` → `SimulationsController` calls `ISgp4Propagator.Propagate(...)` → state vectors returned directly in the response, rendered client-side in Cesium. Nothing here writes propagated output back to the database.

## Testing

Integration tests against a real (or in-memory) Postgres instance for the CRUD paths. The propagation/comparison paths are only as trustworthy as `Propagation/Sgp4`'s and `Propagation/Keplerian`'s own golden-file suites — this module doesn't duplicate that testing, it just calls those interfaces.
