# mission-planner — Product 2 frontend module

Status: **not yet built.** Build trigger: Product 1 (Orbital Simulator) is live and validated with real users. Full product spec: `docs/products/product-2-mission-planner.md`.

## What this folder will contain

```
mission-planner/
├── mission-planner.routes.ts    Lazy-loaded route table (mirrors orbital-simulator.routes.ts)
├── project-switcher/             Org/Project picker — the first UI to touch Organizations
├── mission-dashboard/            List of missions within the current project
├── spacecraft-builder/           Component catalog UI: battery, panels, radio, one payload
├── ground-stations/               Map + pass/contact-window table
├── budgets/                       Power budget view + fuel/Δv budget view
└── mission-timeline/               Sortable table of passes/operations (NOT a Gantt chart — see below)
```

## How to build it, in order

1. **Wire the module into the shell first.** Add a `path: 'mission-planner'` + `loadChildren` entry to `../app.routes.ts` (follow the exact pattern already there for `simulator`), and flip `active: true` for "Mission Planner" in `../shell/shell.ts`. Generate each component with `ng generate component mission-planner/<name>` from `apps/web` (not `app/mission-planner/<name>` — that duplicates the `app/` segment, a mistake made once already during initial scaffolding).
2. **`project-switcher/` before anything else** — every other page in this module needs an active org/project context. Build against the `Modules/Orgs` backend endpoints (see `apps/api/Apsis.Api/Modules/Orgs/README.md`).
3. **`spacecraft-builder/` next** — this is where the "link to an existing Orbital Simulator simulation" picker lives, the concrete UI expression of the zero-re-entry integration story. Call `GET /api/v1/simulations` (already built in Product 1) to populate that picker.
4. **`ground-stations/`** — a map component (plain Leaflet/Mapbox is enough here; Cesium is Product 1's `viewer-3d` concern, don't duplicate it) plus a table fed by `GET /api/v1/missions/{id}/ground-stations`.
5. **`budgets/`** — two simple views (power, fuel), no charting library needed beyond what Product 1 already uses for the comparison plot.
6. **`mission-timeline/`** — build this as a plain sortable `<table>`. Do not reach for a Gantt-chart library. See `aspis-simplification-review.md` item #5 — Gantt rendering is real frontend effort for a visualization that isn't this product's core value. Upgrade only once the table view genuinely feels limiting to real users.

## What NOT to build here yet

No Viewer/Editor/Admin role UI (single "member" role only — see the backend README for why). No live/streaming telemetry — everything in this module is a request/response read, no SignalR connection. No subsystem simulation UI — budgets are static estimates, not running simulations.
