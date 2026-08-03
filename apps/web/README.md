# ASPIS Web — Frontend

Angular workspace. One app shell, one lazy-loaded module per product. See `docs/architecture/folder-structure.md` at the repo root for the full monorepo map, and `docs/products/` for what each product actually does.

## What's here right now

```
src/app/
├── app.ts / app.html / app.routes.ts / app.config.ts   Root: just <router-outlet>, routes point at the shell
├── shell/                Nav bar + <router-outlet> — wraps every product page
├── orbital-simulator/    Product 1 — ACTIVE, has real routes and placeholder pages
│   ├── orbital-simulator.routes.ts
│   ├── dashboard/
│   ├── simulation-editor/
│   ├── viewer-3d/
│   ├── comparison/
│   └── glossary/
├── mission-planner/      Product 2 — not built, see its README.md
├── mission-control/       Product 3 — not built, see its README.md
├── digital-twin/          Product 4 — not built, see its README.md
├── ai-assistant/          Product 5 — not built, see its README.md
└── shared/                Shared UI components, chart wrappers, HTTP services — grows with every product
```

Routing today: `/` redirects to `/simulator`, which lazy-loads `orbital-simulator.routes.ts`. The nav bar in `shell/` shows all five products; only Orbital Simulator is a real link — the rest render disabled with a "not built yet" tooltip, so the shell doesn't need to change shape when a new product module lands. To activate a product, give it a `path` + `loadChildren` entry in `app.routes.ts` (see the comment there) and flip its `active: true` in `shell.ts`.

## How to run it

```bash
npm install        # first time only
npm start           # ng serve — http://localhost:4200
npm run build       # ng build — output to dist/apsis-web
npm test             # ng test (Vitest)
```

## Why it's built this way

- **One Angular workspace, not five separate frontends** — every product is a lazy-loaded route/module in the same app shell, so navigation, auth, and the design system are shared automatically. See `aspis-blueprint.md` Section 0 ("Shared Foundation").
- **`loadComponent`/`loadChildren` everywhere** — even Product 1's own pages are individually lazy-loaded (see the per-page chunks in a `ng build` output). This is cheap to do from day one and means adding Products 2–5 later never bloats the initial bundle.
- **No state management library yet** (NgRx/Signals store, etc.) — Product 1 doesn't need one. Add one only when cross-page state sharing genuinely gets complicated; don't pre-adopt it — same "build it when justified" discipline as the rest of the roadmap (`aspis-simplification-review.md`).
- **CesiumJS is not installed yet** — it gets added when `viewer-3d/` is actually implemented (Product 1, Sprint 5-6 per `docs/products/product-1-orbital-simulator.md`), not before, to keep the dependency tree lean until it's needed.

## Next concrete step

Implement `simulation-editor/` (TLE input form) and wire it to `POST /api/v1/simulations` on the backend — see the Sprint 3-6 plan in `docs/products/product-1-orbital-simulator.md`.
