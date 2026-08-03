# Product 3 — ASPIS Mission Control

Status: **not yet built.** Build trigger: Product 2 (Mission Planner) is live and validated.

## Why now

This is the first product requiring real *operational*, real-time infrastructure — telemetry pipeline and command authorization that everything after depends on. Deliberately built to work against **simulated** telemetry first, with the ingestion interface designed so a real spacecraft is just a second telemetry *source* later, not a rebuild.

## Product Vision

- **Problem:** existing ground-segment tools (Yamcs, OpenC3/COSMOS) are solid but pure ops — no design linkage, no simulated-telemetry-first workflow, dated collaborative UX.
- **Who pays:** teams that now have a real or soon-to-be-real spacecraft and need to operate it — the natural graduation of a Product-2 customer.
- **Unique advantage:** works immediately with simulated telemetry generated from a Mission Planner spacecraft config — a team can train ops crew months before a real satellite exists.

## MVP — exactly what exists, exactly what doesn't

**Exists:** live telemetry dashboard (source: the sim scheduler runs a spacecraft forward using its Product-2 config and Product-1 orbit); command system (build/send/authorize/audit against the simulated spacecraft); threshold alerts; mission timeline (event-log view); basic PDF reports; multi-user collaboration.

**Does not exist:** real spacecraft ingestion (interface designed now, feature ships later); AI; failure-injection as a first-class UI feature (Product 4 exposes it, though event tagging is cheap to include now); deep subsystem *dynamic* simulation beyond what's needed for believable telemetry.

## How to build it — the lean version, not the original heavy one

The original architecture draft for this product specified RabbitMQ, full event sourcing, TimescaleDB, a Redis SignalR backplane, and an AKS migration, all at once. That was over-scoped for the actual load (one simulation producer, a handful of users) and underpriced the real cost of some of those pieces (Kubernetes especially). Build the lean version instead — see `aspis-simplification-review.md` items #6–#10 for the full reasoning:

```
apps/api/Apsis.Api/Modules/MissionControl/   NEW — telemetry query API, alert rules, event-log read model
apps/api/Apsis.Api/Modules/Commands/          NEW — command dispatch, authorization, audit log
apps/web/src/app/mission-control/              NEW — dashboard, telemetry charts, command console, alerts, timeline
```

- **No RabbitMQ.** One in-process `BackgroundService` ticks the simulation, writes telemetry to Postgres, and pushes updates via SignalR directly. Add a broker only when there are genuinely multiple concurrent simulation workers or Commands need guaranteed retry semantics at scale.
- **No full event sourcing.** A mutable `MissionState` table (easy to query — "what's the battery level right now" is just a row read) **plus** an append-only `MissionEvent` audit-log table for history. This gets ~80% of replay/audit value without snapshot-management complexity. Upgrade to true fold-over-log event sourcing only in Product 4, when Replay/Failure-Injection/Scenario-Comparison genuinely need it.
- **No TimescaleDB yet.** A plain Postgres table for telemetry with a `(channel_id, time)` index handles this product's actual data volume fine. Add the Timescale extension later — it's a low-friction migration when real volume demands it.
- **No Redis SignalR backplane.** Plain SignalR, single API instance. Add a backplane the day more than one instance is actually running.
- **No Kubernetes.** Azure Container Apps (or scaled App Service). Only move to k8s once autoscaling needs genuinely exceed what Container Apps can do — a production-grade k8s setup run by two people is realistically a 2-3 month tax, not a one-sprint task.

**What stays non-negotiable regardless of the above:** the discipline around command authorization — auth checks and a full audit trail — from day one. What's being simplified is the infrastructure underneath that discipline, not the discipline itself. And the event *schema* (what fields a `MissionEvent` row has) is still worth designing jointly between both developers before either of you build against it, even though it's stored more simply than a true event-sourced log for now.

## Backend

- `Modules/MissionControl/` — telemetry range queries, alert rule evaluation, event-log read model.
- `Modules/Commands/` — command validation, authorization (a dedicated "can send commands" permission, separate from "can view telemetry" — never bundle these by default), dispatch, audit.
- REST: `/api/v1/missions/{id}/telemetry`, `/api/v1/missions/{id}/commands`, `/api/v1/missions/{id}/alerts`, `/api/v1/missions/{id}/events`, `/api/v1/missions/{id}/reports`.
- SignalR hubs: `TelemetryHub`, `CommandStatusHub`, `AlertHub`.

## Frontend

Mission Dashboard (live status), Telemetry Charts (streaming + historical), Command Console (builder/queue/history), Alerts feed, Mission Timeline (event log — the seed of Product 4's replay scrubber), Reports.

## Database (additions — nothing re-modeled)

```
MissionState (mission_id FK, subsystem, key, value, updated_at)   -- current state, easy to query
MissionEvent (seq, mission_id FK, event_type, payload jsonb, occurred_at)   -- append-only audit log
TelemetryChannel (id, mission_id FK, name, unit, subsystem)
TelemetryData (time, channel_id FK, value)   -- plain Postgres table + index, not a hypertable yet
Command (id, mission_id FK, command_type, parameters jsonb)
CommandExecution (id, command_id FK, status, executed_at)
```

`MissionEvent` is exactly the table Product 4 upgrades to true event-sourcing semantics — designed once, here, evolved later, never replaced with something unrecognizable.

## Deployment / Testing / Documentation

Azure Container Apps (not AKS — see above). Load test telemetry ingestion (k6) before onboarding real ops teams. Scenario tests for command authorization (unauthorized user provably blocked). `docs/simulation/event-schema.md` — the most important document written for this product; `docs/deployment/` runbook starts here, since this is the first product with real production infrastructure.

## Timeline (2-week sprints, ~8 weeks)

| Sprint | Goals | Success criteria |
|---|---|---|
| 1–2 | Event schema (joint design), in-process scheduler, MissionState + MissionEvent tables | Two engineers sign off the event schema before writing consuming code |
| 3–4 | Telemetry pipeline (Postgres + SignalR), basic dashboard | Dashboard updates within 1s of a simulated tick |
| 5–6 | Command system: builder, authorization, audit log | Unauthorized user is provably blocked from sending a command |
| 7–8 | Alerts, Timeline, deploy to Container Apps | First customer runs a full simulated ops session unassisted |

## Business Strategy

SaaS subscription priced per-satellite-per-month. Commercial — the recurring-revenue backbone of the company. First segment: existing Product-2 customers graduating to ops (zero acquisition cost). Funds Product 4 and 5.
