# digital-twin — Product 4 frontend module

Status: **not yet built.** Build trigger: Product 3 (Mission Control) is live and validated. Full product spec: `docs/products/product-4-digital-twin.md`.

## What this folder will contain

```
digital-twin/
├── digital-twin.routes.ts
├── replay/                Scrubber over the mission event log
├── failure-injection/       Fault catalog + injection controls
└── scenario-comparison/     Fork-a-mission + side-by-side diff view (build LAST, see below)
```

## How to build it, in order — sequence matters more here than in any other module

1. **`replay/` first, as simple frame playback.** Re-display already-computed telemetry frames from `GET /api/v1/missions/{id}/telemetry` over a time range — a scrubber control (a plain range `<input type="range">` plus a play/pause button is enough for v1) that re-renders `mission-control`'s telemetry charts against historical frames instead of live SignalR data. Do **not** build a scrubber that re-runs an exact deterministic re-simulation for v1 — that depends on backend work (`MissionEvent` upgraded to true event sourcing) that lands later in this same product's timeline. See `docs/products/product-4-digital-twin.md` and `aspis-simplification-review.md` item #12.
2. **`failure-injection/` second** — a form to pick a fault from the catalog and a target mission, calling the injection API. Visually, tag injected-fault events distinctly in the `mission-control/timeline` view (a different icon/color) so instructors and ops trainees can tell "this went wrong on purpose" apart from a real anomaly.
3. **`scenario-comparison/` last, as a fast-follow — not part of the initial release.** Forking a mission and running two variants side by side is real added UI complexity (two telemetry chart sets, a diff view) on top of Replay + Failure Injection, which are the actual core value of this product. Don't start this until the first two are shipped and validated. See `aspis-simplification-review.md` item #13.

## What NOT to build here yet

No AI-generated explanations of what a replayed anomaly means — that's Product 5's Root Cause Analysis feature, strictly read-only against this product's data, never the other way around.
