# orbital-simulator — Product 1, active

Full product spec: `docs/products/product-1-orbital-simulator.md`.

## What this module does

The entire Product 1 frontend: five lazy-loaded pages under `/simulator/*`, routed by `orbital-simulator.routes.ts`. This is the only product module with real routes right now — every other product folder (`mission-planner/`, `mission-control/`, `digital-twin/`, `ai-assistant/`) is a placeholder until its own product's turn.

## What each page does and how it fits together

| Page | Route | What it does | Backend it calls |
|---|---|---|---|
| `dashboard/` | `/simulator/dashboard` | Lists the current user's saved simulations — the landing page after login | `GET /api/v1/simulations` |
| `simulation-editor/` | `/simulator/simulations/new`, `/simulator/simulations/:id` | Form for entering a TLE or classical orbital elements; creates or edits a `Simulation`/`Satellite` | `POST /api/v1/simulations` |
| `viewer-3d/` | `/simulator/simulations/:id/view` | CesiumJS globe + ground track for a propagated simulation | `GET /api/v1/simulations/{id}/propagate` |
| `comparison/` | `/simulator/compare` | SGP4 vs. Keplerian divergence plot — fast-follow, not required for MVP launch | `GET /api/v1/simulations/{id}/compare` |
| `glossary/` | `/simulator/glossary` | Static orbital-mechanics reference content for the student segment | none (static content) |

## How it works today

All five pages are scaffolded as standalone, lazy-loaded components with placeholder templates — each `loadComponent()` in `orbital-simulator.routes.ts` resolves, but no page has real data-fetching or form logic wired up yet. `viewer-3d/` in particular has no Cesium dependency installed yet — that gets added specifically when this page's real implementation starts (Sprint 5-6), not before, to keep the dependency tree lean until it's needed.

## Process: build order (see `docs/products/product-1-orbital-simulator.md` for the full sprint plan)

1. `simulation-editor/` first — nothing else is useful without a way to create a simulation, and it's the page that exercises the backend's `Modules/Simulations` CRUD path end to end.
2. `dashboard/` next — trivial once the create flow exists (just a list view over the same API).
3. `viewer-3d/` — the Cesium integration, the most time-consuming single page in this product; timebox it and lean on Cesium's own sandcastle examples rather than building the 3D scene from primitives.
4. `comparison/` and `glossary/` last, as fast-follows after the core loop (create → propagate → view → save/share) is shipped and has real users — see `aspis-simplification-review.md` item #3.

## What NOT to build here

No organizations/project switcher (Product 2). No ground-station map (explicitly Product 2's job, even though it will also call the propagation engine — don't build it here just because the underlying capability is adjacent).
