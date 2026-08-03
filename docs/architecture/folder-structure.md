# ASPIS Monorepo — Folder Structure Guide

One repo, growing by addition, never rewritten. This file is the map: for every folder in the tree, which product it belongs to, what goes inside it, and — for anything not active yet — the exact condition that triggers building it. When in doubt about whether to start a folder early, the answer is in `aspis-simplification-review.md`: build it when a real, current need demands it, not because a later product might want it.

```
/Apsis
├── apps/
│   ├── web/                 Angular workspace — one app shell, one lazy-loaded module per product
│   ├── api/                 ASP.NET Core — modular monolith (Products 1-3, until Product 4's extraction)
│   ├── twin-orchestrator/   Extracted service — Product 4 only
│   ├── sim-engine/          C++ physics engine — Product 4 only
│   └── ai-services/         Python/FastAPI — Product 5 only
├── libs/
│   └── proto/                gRPC/event contracts shared across languages — Product 4+
├── infra/
│   ├── docker/                Local dev + early production (Products 1-3)
│   ├── k8s/                   At-scale orchestration — only if Container Apps isn't enough
│   └── ci/                    GitHub Actions workflows — active from day one
└── docs/
    ├── architecture/          System design docs + ADRs
    ├── api/                   OpenAPI specs
    ├── physics/                Propagation/subsystem model derivations + validation method
    ├── simulation/             Event schema, failure-injection catalog
    └── deployment/             Runbooks, DR plan
```

---

## `apps/web/` — Angular frontend (one app shell, five lazy-loaded modules)

| Folder | Product | Contains | Status |
|---|---|---|---|
| `src/app/shell/` | Foundation | Nav, auth guard, layout, org/project switcher | **Active now (P1)** |
| `src/app/orbital-simulator/` | Product 1 | Dashboard, Simulation Editor, 3D Viewer (Cesium), Comparison view, Glossary | **Active now (P1)** |
| `src/app/mission-planner/` | Product 2 | Org/project switcher, Spacecraft Builder, Ground Station manager, Budget views, Mission Timeline | Not yet — starts once P1 is live |
| `src/app/mission-control/` | Product 3 | Live dashboard, Telemetry Charts, Command Console, Alerts feed, Timeline | Not yet — starts once P2 is live |
| `src/app/digital-twin/` | Product 4 | Replay scrubber, Scenario Comparison view | Not yet — starts once P3 is live |
| `src/app/ai-assistant/` | Product 5 | Anomaly feed, conversational assistant panel | Not yet — starts once P4 is live |
| `src/app/shared/` | Foundation | Shared UI components, chart wrappers, HTTP services | **Active now (P1)**, grows with every product |

Each product's own subfolder structure (e.g. `orbital-simulator/dashboard/`, `/simulation-editor/`, `/viewer-3d/`, `/comparison/`, `/glossary/`) is already scaffolded for Product 1 as an example of the pattern each later module follows.

---

## `apps/api/` — ASP.NET Core backend (modular monolith)

Stays **one deployable** through Products 1-3. Only `TwinOrchestrator` (Product 4) is ever extracted into its own service — and only once its boundary is proven stable, not preemptively.

| Folder | Product | Contains | Status |
|---|---|---|---|
| `Apsis.Api/Modules/Users/` | Product 1 | Auth (ASP.NET Identity, JWT, Google OAuth), accounts | **Active now (P1)** |
| `Apsis.Api/Modules/Simulations/` | Product 1 | Simulation CRUD, export, propagator comparison orchestration | **Active now (P1)** |
| `Apsis.Api/Propagation/Sgp4/`, `/Keplerian/` | Product 1 | Propagation logic **in-process in C#** — no separate engine or gRPC yet (see review item #1) | **Active now (P1)** |
| `Apsis.Api/Modules/Orgs/` | Product 2 | Organizations, Projects, membership, single "member" role (not full RBAC yet — see review item #4) | Not yet |
| `Apsis.Api/Modules/MissionPlanner/` | Product 2 | Missions, Spacecraft Builder, Ground Stations, power/fuel budget calculators | Not yet |
| `Apsis.Api/Modules/MissionControl/` | Product 3 | Telemetry query API, alert rules, event-log read model | Not yet |
| `Apsis.Api/Modules/Commands/` | Product 3 | Command dispatch, authorization, audit log — build with real rigor from day one | Not yet |
| `Apsis.Api/Modules/Reports/` | Product 3 | PDF/exportable mission summaries | Not yet |
| `Apsis.Api.Tests/Propagation/` | Product 1 | Golden-file tests against Vallado's published SGP4 reference vectors — the single most important test suite in the company | **Active now (P1)** |

---

## `apps/twin-orchestrator/` — Product 4 only

The twin lifecycle service (start/stop/snapshot/fork/replay a running twin). **Do not build this as a separate deployable before Product 4** — it starts as a module inside `apps/api`, and is extracted only once its boundary is proven stable across three prior products' worth of real usage (review item #11).

## `apps/sim-engine/` — Product 4 only

A separate C++ simulation engine, introduced only when real subsystem physics (ADCS, thermal, power) genuinely needs a dedicated high-performance process and a stable cross-language (C#/Python) contract. Products 1-3's propagation and pass-prediction logic deliberately stay in-process in C# (review item #1) — this folder's `propagation/` and `pass-prediction/` subfolders exist as placeholders for where that logic *migrates to*, if and when Product 4's fidelity requirements justify the move.

| Subfolder | Contains |
|---|---|
| `src/propagation/` | Migrated propagation logic (only if/when justified) |
| `src/pass-prediction/` | Migrated pass-prediction logic (only if/when justified) |
| `src/attitude/` | ADCS — quaternion attitude dynamics, torque model |
| `src/power/` | Real power model (equivalent-circuit battery, panel efficiency) |
| `src/thermal/` | Real thermal model (multi-node network) |
| `src/scheduler/` | Multi-rate tick loop |
| `tests/` | Golden-file/scenario tests |
| `proto/` | gRPC contracts (mirrors `libs/proto/`) |

## `apps/ai-services/` — Product 5 only

Python/FastAPI. Every subfolder connects to Postgres/Timescale with a **read-only** database role — enforced at the credential level, not just convention, so AI failures can never corrupt simulation or telemetry state.

| Subfolder | Contains |
|---|---|
| `anomaly_detection/` | Rule-based v1, then ML (isolation forest/autoencoder) v2 |
| `predictive_maintenance/` | LSTM/temporal models for battery fade, fuel remaining |
| `orbit_optimization/` | Optimization service over the Simulation Engine |
| `assistant/` | LLM + RAG — conversational assistant, NL query, root-cause narration |
| `training_pipelines/` | Synthetic dataset generation from Digital Twin failure-injection sweeps |

---

## `libs/proto/` — Product 4+

Shared gRPC/event contracts between `apps/api`, `apps/sim-engine`, and `apps/ai-services`. Created alongside `apps/sim-engine` — there is no cross-language boundary to define a contract for before then.

## `infra/`

| Folder | Contains | Status |
|---|---|---|
| `docker/` | `docker-compose.yml` for local dev (api + postgres) | **Active now (P1)** |
| `ci/` | GitHub Actions: build/lint/test/deploy | **Active now (P1)** |
| `k8s/` | Kubernetes manifests/Helm charts | Not yet — default to Azure Container Apps / scaled App Service; only populate this once autoscaling needs genuinely exceed what Container Apps can do (review item #10) |

## `docs/`

| Folder | Contains | Status |
|---|---|---|
| `architecture/` + `architecture/adr/` | System design docs, Architecture Decision Records (start with ADR-0001: "SGP4 in-process, not a gRPC engine, for Product 1") | **Active now (P1)** |
| `api/` | Auto-generated OpenAPI spec | **Active now (P1)** |
| `physics/` | SGP4/two-body derivation and validation methodology now; grows with ADCS/thermal/power docs at Product 4 | **Active now (P1)**, grows over time |
| `simulation/` | Event schema design (Product 3), failure-injection catalog (Product 4) | Not yet |
| `deployment/` | Runbooks, disaster-recovery plan | Starts at Product 3, when real production infrastructure exists |

---

## The one rule that governs every "not yet" folder above

Every folder marked "not yet" already exists in the tree so the target shape of the monorepo is visible today, but each one carries a placeholder `README.md` stating its build trigger. Do not fill in a "not yet" folder just because it's sitting there empty — that's the exact premature-infrastructure pattern flagged in `aspis-simplification-review.md`. Build it when the product whose turn it is has validated the one before it.
