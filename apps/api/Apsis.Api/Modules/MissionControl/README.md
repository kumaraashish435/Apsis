# Modules/MissionControl — Product 3 backend module

Status: **not yet built.** Build trigger: Product 2 (Mission Planner) is live and validated. Full product spec: `docs/products/product-3-mission-control.md`.

## What this folder will contain

Telemetry query API, alert rule evaluation, event-log read model. `TelemetryController`, `AlertsController`.

## How to build it

1. **`MissionState` table** (mutable, one row per subsystem/key, easy to query "what's the value right now") **and** a separate append-only `MissionEvent` table (audit log: every state change, command, later failure-injection). This is the lean alternative to full event sourcing — see `aspis-simplification-review.md` item #7. Design the `MissionEvent` schema (what columns/payload shape a row has) jointly with whoever builds `Modules/Commands`, before either of you write code against it — it's the one artifact in this product worth a real joint design session.
2. **Telemetry storage: a plain Postgres table**, `TelemetryData(time, channel_id, value)`, with a `(channel_id, time)` index. Not TimescaleDB yet — see `aspis-simplification-review.md` item #8. Add the extension later; it's a low-friction migration when real data volume demands it.
3. **The simulation tick loop is an in-process `BackgroundService`, not a RabbitMQ consumer.** It ticks the (currently placeholder-fidelity) subsystem models, writes to `TelemetryData`/`MissionState`/`MissionEvent`, and pushes updates directly via SignalR. See `aspis-simplification-review.md` item #6 — no message broker until there are genuinely multiple concurrent simulation workers.
4. **SignalR hubs**: `TelemetryHub` (range + live), `AlertHub`. No Redis backplane yet — single API instance is fine until there's a real reason to run more than one (item #9).
5. **Alert rules**: simple threshold checks evaluated against each incoming telemetry frame; on breach, write a `MissionEvent` and push via `AlertHub`.
6. REST: `GET /api/v1/missions/{id}/telemetry`, `.../alerts`, `.../events`.

## What NOT to build here

No Kafka, no RabbitMQ, no Timescale, no Kubernetes deployment target — all explicitly deferred, see `aspis-simplification-review.md` items #6, #8, #10. No AI-based anomaly detection (rule-based thresholds only; ML-based detection is Product 5, reading this module's data, never writing to it).
