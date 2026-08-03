# Product 2 — ASPIS Mission Planner

Status: **not yet built.** Build trigger: Product 1 (Orbital Simulator) is live and validated with real users.

## Why now

Once orbit propagation is trusted, the next layer of value is designing the *mission* around that orbit: the spacecraft itself, payload, ground stations, comms windows, rough power/fuel budgets. This product is valuable specifically *because* Product 1 exists — a simulation a student built there becomes the orbital basis for a real mission plan with zero data re-entry.

## Product Vision

- **Problem:** teams doing power/fuel/link budgets in scattered spreadsheets, disconnected from whatever tool computed the orbit.
- **Who pays:** CubeSat/smallsat startups and university teams doing real (not just educational) mission design.
- **Competitors:** STK Mission modules, SMAD-style spreadsheet methodology, bespoke Excel tools.
- **Unique advantage:** zero re-entry of orbital data from Product 1; real multi-user Organizations make it genuinely collaborative where competitors are single-seat or file-passing.

## MVP — exactly what exists, exactly what doesn't

**Exists:** Organizations & Projects (multi-tenancy starts here, not before); create a Mission referencing an existing Product-1 `Satellite` or a new one; Spacecraft Builder (mass, power inputs, a small catalog: battery, panels, radio, one payload); Ground Station catalog + pass/contact-window prediction; simple orbit-averaged power budget; simple fuel/Δv budget (Tsiolkovsky); Mission Timeline (a sortable table first — not a Gantt chart, see below).

**Does not exist:** live telemetry, commands, real-time dashboards, AI, failure injection, subsystem *simulation* (this product is static budgeting/estimation, not dynamic simulation — that's Product 3/4's job).

## How to build it — architecture

Extends the existing `apps/api` monolith and `apps/web` shell — does not introduce a new deployable.

```
apps/api/Apsis.Api/Modules/Orgs/            NEW — organizations, membership, single "member" role
apps/api/Apsis.Api/Modules/MissionPlanner/  NEW — missions, spacecraft config, ground stations, budgets
apps/web/src/app/mission-planner/            NEW — Angular module, lazy-loaded sibling to orbital-simulator
```

Ground-station pass prediction calls the *same* `ISgp4Propagator` Product 1 already validated — do not reimplement propagation here. Power/fuel budget math is plain C#, no new infrastructure.

**On RBAC — build the lean version, not the full one:** ship a single "member" role per organization (everyone in an org can do everything). Do **not** build Viewer/Editor/Admin tiers yet — see `aspis-simplification-review.md` item #4. Add granular roles only when a paying customer with a real internal permissions conflict asks for them.

**On the Timeline view — build the lean version:** a sortable table of passes/operations, not a Gantt chart. See `aspis-simplification-review.md` item #5. Upgrade to a Gantt view only once the underlying data model is stable and the table genuinely feels limiting.

## Backend

- `Modules/Orgs/` — Organization, Project, membership. The `User.OrgId` field seeded-but-unused in Product 1 becomes load-bearing here.
- `Modules/MissionPlanner/` — Mission, SpacecraftComponent, GroundStation, Pass, budget calculators.
- REST: `/api/v1/orgs`, `/api/v1/projects`, `/api/v1/missions`, `/api/v1/missions/{id}/spacecraft`, `/api/v1/missions/{id}/ground-stations`, `/api/v1/missions/{id}/budget`.
- No WebSockets yet — this product is design-time, not real-time.

## Frontend

Org/Project switcher, Mission Dashboard, Spacecraft Builder, Ground Station manager (map + pass table), Power/Fuel Budget views, Mission Timeline (table). A "link to an existing Orbital Simulator simulation" picker is the literal UI expression of the zero-re-entry integration story.

## Database (additions to Product 1's schema — nothing re-modeled)

```
Organization (id, name)
Project (id, org_id FK)
Mission (id, project_id FK, satellite_id FK -> Product 1's Satellite table)
SpacecraftComponent (id, mission_id FK, subsystem_type, parameters jsonb)
GroundStation (id, mission_id FK, latitude, longitude)
Pass (id, ground_station_id FK, aos, los, max_elevation)
```

`Mission` is the row Product 3 attaches live telemetry to and Product 4 replays — same foreign key, never re-modeled going forward.

## Deployment / Testing / Documentation

Same Azure App Service pattern as Product 1 — still no Kubernetes. Testing: scenario tests for pass-prediction (known station + known TLE → known contact window, cross-checked against a reference tool). Documentation: extend `docs/physics/` with a link-budget/pass-geometry section; write `ADR-0002-organizations-in-product-2.md`.

## Timeline (2-week sprints, ~8 weeks)

| Sprint | Goals | Success criteria |
|---|---|---|
| 1–2 | Orgs/Projects, single-role auth extension | Two users in different orgs can't see each other's missions |
| 3–4 | Mission + Spacecraft Builder | A mission references a Product-1 simulation with zero re-entry |
| 5–6 | Ground Stations + Pass Prediction | Predicted AOS/LOS matches a reference tool |
| 7–8 | Budgets + Timeline table, deploy | A real CubeSat/university team completes one mission plan unassisted |

## Business Strategy

Per-seat team subscription ($30–60/seat/month) — this is collaborative software, unlike Product 1's solo use. Fully commercial, no OSS component. First segment: funded early-stage CubeSat startups. Revenue funds Product 3's larger real-time/event-system engineering lift.
