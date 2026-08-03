# Product 1 — ASPIS Orbital Simulator

Status: **active — this is what you're building right now.**

## Why this is built first

It's the smallest unit of real value in the entire ASPIS vision: pure orbital mechanics, no operations, no AI, no multi-user complexity. Buildable by two people in one quarter, sellable to a real paying customer segment (universities, hobbyists) immediately, and it forces the one piece of IP every later product depends on — a validated, trustworthy propagation engine. Lowest risk in the whole roadmap: no real telemetry, no commands, nothing safety- or export-control-relevant.

## Product Vision

- **Problem it solves:** STK/AGI is powerful but expensive and desktop-bound; GMAT is free but has 2000s-era UX and a steep learning curve. Nothing lets a student or early-stage engineer open a browser, paste a TLE, and see an orbit in seconds, or compare how two propagators diverge over time.
- **Who pays:** universities (course licenses), individual hobbyists/engineers (freemium → subscription), CubeSat teams doing early feasibility work.
- **Customers:** aerospace/astronautics students and professors, CubeSat hobbyists, early-stage startup engineers.
- **Competitors:** STK/AGI, NASA GMAT, Celestrak/n2yo (visualization only, no authoring), poliastro/Skyfield (developer libraries, not a product).
- **Unique advantage:** modern collaborative web UX in a domain still dominated by 2000s desktop software; propagator comparison as a first-class feature; a free tier that's the top of the ASPIS funnel.

## MVP — exactly what exists, exactly what doesn't

**Exists:** create a satellite from a TLE or classical orbital elements; propagate with SGP4/SDP4; 2D ground-track + 3D globe view (Cesium); orbital parameter readout (altitude, period, inclination, eccentricity, RAAN, arg. of perigee, true anomaly); export (CZML/CSV/JSON — CSV/JSON first, CZML is a fast-follow); side-by-side SGP4 vs. two-body Keplerian comparison; single-user accounts; save/reload/share simulations.

**Does not exist:** organizations/teams, spacecraft subsystem modeling, ground stations/contact windows (explicitly Product 2), telemetry, commands, any AI.

## Architecture (as scaffolded today)

```
apps/web/src/app/orbital-simulator/   Angular module, lazy-loaded from app.routes.ts
apps/api/Apsis.Api/Modules/Simulations/   SimulationsController (CRUD/propagate/compare/export)
apps/api/Apsis.Api/Modules/Users/          UsersController (auth)
apps/api/Apsis.Api/Propagation/Sgp4/        ISgp4Propagator — the core IP
apps/api/Apsis.Api/Propagation/Keplerian/   IKeplerianPropagator — comparison view only
```

No separate C++ engine, no gRPC, no message broker, no Kubernetes. One Angular app, one ASP.NET Core project, one Postgres database, Docker Compose for local dev, Azure App Service for hosting. See `aspis-simplification-review.md` items #1–#3 for why, and `docs/architecture/adr/ADR-0001-in-process-propagation.md`.

**Data flow:** only *inputs* (TLE/elements, timespan) are persisted in Postgres. Propagated state vectors are recomputed on every load via `ISgp4Propagator`/`IKeplerianPropagator` and streamed to the client, rendered client-side in Cesium. No large time-series blobs — this keeps the database trivial at this product's scale.

## Backend

- `Modules/Users/UsersController` — ASP.NET Core Identity, JWT, Google OAuth. The `User` entity (added in Sprint 3-4) carries a nullable `OrgId` from day one, unused until Product 2.
- `Modules/Simulations/SimulationsController` — `GET/POST /api/v1/simulations`, `GET /api/v1/simulations/{id}/propagate`, `.../compare`, `.../export`.
- `Propagation/Sgp4/ISgp4Propagator` — the correctness-critical core. Implementation must pass the golden-file suite in `Apsis.Api.Tests/Propagation` against Vallado's published SGP4 test vectors before anything is built on top of it.
- `Propagation/Keplerian/IKeplerianPropagator` — simple two-body propagation, used only by the Comparison view.
- **Patterns:** modular monolith; Repository pattern over EF Core (once added); Strategy pattern for propagators (`ISgp4Propagator`/`IKeplerianPropagator` share the `StateVector` shape specifically so a numerical propagator can be swapped in later — Product 6+ — without touching calling code).
- **Auth:** single-tenant per user by design — no org enforcement in Product 1.

## Frontend

- `dashboard/` — saved simulations list (landing page).
- `simulation-editor/` — TLE/orbital-element input form.
- `viewer-3d/` — CesiumJS globe + ground track. *Not installed yet* — add `cesium`/`@cesium/engine` when this page is actually implemented (Sprint 5-6), not before.
- `comparison/` — SGP4 vs. Keplerian divergence plot. Fast-follow, not required for MVP launch — see `aspis-simplification-review.md` item #3.
- `glossary/` — orbital-mechanics terms, a genuine acquisition hook for the student segment.
- Routing: `orbital-simulator.routes.ts`, lazy-loaded as a whole from `app.routes.ts`.

## Database

```
User (id, email, org_id [nullable, unused until Product 2])
Simulation (id, user_id FK, name, created_at, is_public)
Satellite (id, simulation_id FK, name, tle_line1, tle_line2, orbital_elements jsonb)
```

`Satellite` is the seed of the shared schema — Product 2's Mission Planner attaches spacecraft configuration to this exact table via foreign key, never re-modeled. Indexes: `(user_id)` on `Simulation`, `(simulation_id)` on `Satellite`.

## Deployment

Docker Compose locally (api + postgres). GitHub Actions CI (build/lint/test/container build) → Azure App Service for API + frontend, Azure Database for PostgreSQL. Serilog → Azure Monitor. Secrets in Azure Key Vault from day one. **No Kubernetes** — see `aspis-simplification-review.md` item #10; that's deferred to Product 3 at the earliest, and Azure Container Apps is preferred over raw AKS even then.

## Testing

- **Golden-file tests** (`Apsis.Api.Tests/Propagation`): SGP4 output checked against Vallado's published reference vectors for a battery of real TLEs. This is the single most valuable test suite in the company — every later product inherits this engine's correctness.
- Integration tests for the Simulations API.
- Regression: golden files re-run on every PR.
- Performance: not a concern yet — basic response-time smoke tests only.

## Documentation

- `docs/physics/` — SGP4 and two-body derivation, with references and the validation methodology (which golden-file scenario proves what).
- `docs/api/` — OpenAPI spec (auto-generated from the controllers).
- `docs/architecture/adr/ADR-0001-in-process-propagation.md` — why propagation is in-process C#, not a gRPC engine, in Product 1.

## Timeline (2-week sprints, ~8 weeks per the lean path)

| Sprint | Goals | Success criteria |
|---|---|---|
| 1–2 | Implement `ISgp4Propagator`, golden-file tests vs. Vallado vectors | 100% of the reference test-vector suite passes within published tolerance |
| 3–4 | EF Core + Postgres, Users/Simulations CRUD, auth | Can create/save/reload a simulation via the API |
| 5–6 | Cesium integration, Dashboard + Simulation Editor + 3D Viewer | A user can paste a TLE and watch it orbit in-browser |
| 7–8 | Deploy, polish, share links, first external users | Product is live at a public URL; 5 external users onboarded unassisted |

Comparison view and extra export formats (CZML) are explicitly **not** in this 8-week critical path — add them as a fast-follow once the core loop has real user pull (see `aspis-simplification-review.md` item #3).

## Business Strategy

Freemium (limited saves, no export) + Pro ($9–15/mo) + University Site License (~$500–2,000/yr/department). Consider open-sourcing the SGP4 wrapper alone for community goodwill while keeping the product commercial. First segment: university courses. Revenue funds Product 2.

## Risks specific to this product

- Underestimating SGP4/SDP4 edge cases (deep-space orbits need SDP4, not SGP4) — budget real time for Sprint 1-2, don't rush the golden-file suite.
- Cesium's learning curve — it's a large library; timebox the initial integration and lean on its official sandcastle examples rather than building the 3D scene from primitives.
