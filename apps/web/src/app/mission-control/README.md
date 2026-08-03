# mission-control — Product 3 frontend module

Status: **not yet built.** Build trigger: Product 2 (Mission Planner) is live and validated. Full product spec: `docs/products/product-3-mission-control.md`.

## What this folder will contain

```
mission-control/
├── mission-control.routes.ts
├── dashboard/            Live status cards, fed by SignalR — the first real-time page in the app
├── telemetry-charts/      Streaming + historical charts, per subsystem
├── command-console/       Command builder, pending queue, execution history
├── alerts/                 Threshold-breach feed
├── timeline/               Event-log view — the visual seed of Product 4's replay scrubber
└── reports/                 List + download of generated PDF reports
```

## How to build it, in order

1. **Set up the SignalR client connection at the shell level, not per-page.** This module is the first one that needs a persistent WebSocket connection alive across route changes — wire it in `../shell/shell.ts` (or a dedicated service in `../shared/`), not inside `dashboard/` alone, so navigating between `dashboard/`, `telemetry-charts/`, and `alerts/` doesn't reconnect each time.
2. **`telemetry-charts/` next**, backed by `GET /api/v1/missions/{id}/telemetry` for historical range queries and the `TelemetryHub` for live updates. Reuse whatever charting approach Product 1's `comparison/` page settled on — don't introduce a second charting library.
3. **`command-console/`** — this is the one page in the whole frontend that deserves extra scrutiny in review. It must clearly distinguish "can view telemetry" from "can send commands" in the UI (disable the send button, don't just hide it, so the authorization boundary is visible not just enforced server-side).
4. **`alerts/`** and **`timeline/`** — straightforward list views over `AlertHub`/`GET /api/v1/missions/{id}/events`.
5. **`reports/`** last — simplest page, a list + download links against `GET /api/v1/missions/{id}/reports`.

## What NOT to build here yet

No failure-injection button/UI (Product 4 exposes that, even though the backend tags events for it now). No AI panel (Product 5). No Gantt/replay-scrubbing controls beyond a plain chronological list — the actual scrubber UI is a Product 4 concern once `MissionEvent` is upgraded to true event sourcing.
