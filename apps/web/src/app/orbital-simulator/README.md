# orbital-simulator — Product 1, active

Full product spec: `docs/products/product-1-orbital-simulator.md`.

## What this module does

The entire Product 1 frontend: six pages under `/simulator/*` (five nested under `Shell`'s
product-switcher chrome, one deliberately not — see below), plus the public,
unauthenticated shared-viewer page. Routed by `orbital-simulator.routes.ts` (nested pages)
and `../app.routes.ts` (the 3D viewer and the shared-viewer route, both top-level). This is
the only product module with real routes right now — every other product folder
(`mission-planner/`, `mission-control/`, `digital-twin/`, `ai-assistant/`) is a placeholder
until its own product's turn.

## What each page does and how it fits together

| Page | Route | What it does | Backend it calls |
|---|---|---|---|
| `dashboard/` | `/simulator/dashboard` | Lists, shares/unshares, and deletes the current user's saved simulations — the landing page after login | `GET/DELETE /api/v1/simulations`, `POST/DELETE .../share` |
| `simulation-editor/` | `/simulator/simulations/new`, `/simulator/simulations/:id` | Form for entering a TLE or classical orbital elements plus a propagation window; creates or edits a `Simulation` | `POST/PUT /api/v1/simulations` |
| `viewer-3d/` | `/simulator/simulations/:id/view` (top-level, not under `Shell` — see `../app.routes.ts`) | Three.js globe (`viewer-3d/earth-view/`) + 2D ground track (`viewer-3d/ground-track/`) toggle, the orbital-parameters readout, and export buttons | `GET .../{id}/propagate`, `.../export` |
| `comparison/` | `/simulator/compare` | SGP4 vs. Keplerian divergence chart, with a picker across the user's TLE-based simulations | `GET .../{id}/compare` |
| `glossary/` | `/simulator/glossary` | Static orbital-mechanics reference content, covering exactly the terms the other pages use | none (static content) |
| `shared-viewer/` | `/shared/:token` (top-level, public, no `Shell`, no auth guard) | Read-only mirror of `viewer-3d/` for anyone with a share link — no session required | `GET /api/v1/simulations/shared/{token}` and its `/propagate`, `/export` counterparts |

Two more folders support all of the above rather than being pages themselves:

| Folder | What it is |
|---|---|
| `simulations/` | `SimulationsService` (all the HTTP calls above) + `simulation.models.ts` (TypeScript types mirroring the backend's DTOs exactly — see `apps/api/Apsis.Api/Modules/Simulations/SimulationDtos.cs`). Every page above goes through this service; none of them call `HttpClient` directly. |
| `_page.scss` | Shared layout/component styles (`.page`, `.card`, `.btn`, `.sim-table`, form fields) `@use`'d by `dashboard/`, `simulation-editor/`, `comparison/`, `glossary/` — the "normal content page" look, as opposed to `viewer-3d/`'s and `shared-viewer/`'s own full-bleed chrome (`viewer-3d/_viewer-chrome.scss`). |

## How it works today

All six pages are real, not placeholders — see `Modules/Simulations/README.md` (backend) and `viewer-3d/earth-view/README.md` (the 3D rendering internals) for the two pieces with the most design decisions behind them. The 3D viewer is built on **Three.js**, not Cesium — an earlier version of this doc said Cesium; that never happened, the actual 3D work (see `viewer-3d/earth-view/`) was built on Three.js from the start and there's no plan to add a second 3D engine alongside it.

## Process: what shipped, in what order

Matches the build order this doc previously laid out:

1. `simulation-editor/` — the create flow, the thing nothing else is useful without.
2. `dashboard/` — a list view over the same API.
3. `viewer-3d/` — the Three.js integration, the most time-consuming single page.
4. `comparison/` and `glossary/`, plus the shareable-link feature (`shared-viewer/` + the backend's `/shared/*` routes) — shipped together as the remaining MVP scope once the core loop (create → propagate → view → save/share) worked end to end.

## What NOT to build here

No organizations/project switcher (Product 2). No ground-station map (explicitly Product 2's job, even though it will also call the propagation engine — don't build it here just because the underlying capability is adjacent).
