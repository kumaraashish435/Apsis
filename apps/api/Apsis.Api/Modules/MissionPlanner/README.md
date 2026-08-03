# Modules/MissionPlanner — Product 2 backend module

Status: **not yet built.** Build trigger: Product 1 (Orbital Simulator) is live and validated. Full product spec: `docs/products/product-2-mission-planner.md`.

## What this folder will contain

`Mission`, `SpacecraftComponent`, `GroundStation`, `Pass`, and the power/fuel budget calculators. `MissionsController`, or split into `MissionsController` + `GroundStationsController` if it grows unwieldy.

## How to build it

1. **`Mission` entity references `Modules/Simulations`' existing `Satellite` table via `SatelliteId` FK.** This is the whole point of this module existing — do not duplicate orbit/TLE data here. A mission is created either against an existing Product-1 `Satellite` or a newly-entered one, but it's the same table either way.
2. **`SpacecraftComponent`** — keep the catalog small on purpose: battery, solar panel, radio, one payload type. Store per-component parameters as `jsonb` rather than a rigid column-per-parameter schema, since the catalog will grow across later products and a jsonb bag avoids a migration every time.
3. **`GroundStation` + `Pass`** — pass prediction calls the *same* `ISgp4Propagator` interface from `Modules/Simulations`/`Propagation/Sgp4` that Product 1 already validated. Do not write a second propagator here, and do not stand up a separate engine/service for this — it's a geometry calculation (elevation angle from station lat/long against a propagated state vector) layered on an existing, trusted primitive.
4. **Power/fuel budget calculators** — plain C# methods (orbit-averaged generation vs. consumption; Tsiolkovsky rocket equation for Δv/fuel mass). No new infrastructure, no separate service.
5. REST: `GET/POST /api/v1/missions`, `.../spacecraft`, `.../ground-stations`, `.../budget`.

## What NOT to build here

No dynamic subsystem *simulation* — everything in this module is a static estimate/budget, computed once per request. Running simulations over time is Product 3/4's job. No telemetry, no commands.
