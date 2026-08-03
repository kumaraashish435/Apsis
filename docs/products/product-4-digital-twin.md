# Product 4 — ASPIS Digital Twin

Status: **not yet built.** Build trigger: Product 3 (Mission Control) is live and validated.

## Why now

Products 1–3 exist independently and each generate revenue. Digital Twin is the first product that is *explicitly* an integration — it deepens and connects what already exists: real subsystem *simulation* (not just budgets), failure injection, replay, and scenario comparison, all sitting on top of Product 3's foundation.

## Product Vision

- **Problem:** none of Products 1–3 alone let you run a "what if" — what if the battery degrades, what if a command had been sent differently, what if two mission designs are compared against the same fault.
- **Who pays:** the same ops/engineering teams already on Products 2–3, wanting deeper simulation fidelity and training capability — a **Pro/Enterprise upsell**, not a new market.
- **Unique advantage:** impossible to build well without Products 1–3 already existing and sharing data — a direct competitor would have to build all four at once.

## MVP — exactly what exists, exactly what doesn't

**Exists:** real subsystem simulation (power/thermal/ADCS replacing Product 3's lightweight placeholders); Failure Injection (fault catalog + injection API); Mission Replay; Scenario Comparison (fork a mission, run two variants side by side).

**Does not exist:** AI (strictly Product 5); real-spacecraft ingestion (still deferred, same interface reused).

## How to build it — sequence matters here

Ship in this order within the product, don't bundle everything into one release:

1. **Replay first, as simple frame playback** — re-display already-computed telemetry frames from Postgres. Instant, simple, good enough for a UI scrubber. **Do not** start with exact deterministic re-simulation replay — that depends on Product 3's `MissionEvent` log being upgraded to true fold-over-log event sourcing (do that upgrade here, in Product 4, not before) plus serious determinism testing. Build the simple version first; upgrade to exact re-simulation only once a customer needs "replay with a modified input" — a specific, real ask, not a hypothetical one. See `aspis-simplification-review.md` item #12.
2. **Failure Injection second** — a fault catalog + injection API, tagged events in `MissionEvent`. Uniform with a normal command: it mutates a subsystem parameter, logged the same way.
3. **Scenario Comparison last, as its own fast-follow** — fork a mission, run two variants, diff the results. Real added complexity (concurrent forked simulations, a diff UI) on top of Replay + Failure Injection, which are the actual core value. See `aspis-simplification-review.md` item #13.

## Real subsystem physics — what "real" means here

Replacing Product 3's lightweight placeholders with:
- **Power:** equivalent-circuit battery model, temperature-dependent capacity.
- **Thermal:** multi-node lumped network (vs. Product 3's single node).
- **ADCS:** quaternion attitude dynamics, gravity-gradient/magnetic/aero/SRP torque model.

Validate each against a published reference test case before trusting it — same golden-file discipline as Product 1's propagator.

## On extracting a `TwinOrchestrator` service — don't, unless forced

The original architecture draft called for extracting twin lifecycle management (`start/stop/snapshot/fork/replay`) into its own deployable service the moment Product 4 begins, reasoning that "the boundary is stable." Stability alone isn't a reason to split a service — that's the exact "microservices only when justified" principle the whole roadmap is supposed to follow. **Keep it a module inside `apps/api`.** Only extract it into `apps/twin-orchestrator/` when there's an actual operational forcing function: independent scaling needs, independent deploy cadence, or a language change. See `aspis-simplification-review.md` item #11.

## How it communicates with everything else

- **Mission Planner:** the twin's starting configuration is a Mission Planner spacecraft/orbit — never re-entered.
- **Mission Control:** commands and failure injections arrive as events through the same Command Dispatch built in Product 3; telemetry the twin produces is displayed by the same dashboards.
- **Simulation:** gains real physics modules — still in-process C#, unless a genuine performance/multi-language need has appeared by this point (see `apps/sim-engine/README.md` for the trigger condition).
- **AI (Product 5):** deliberately **read-only** against the event log and telemetry, enforced at the database credential level — AI failures can never corrupt the twin.

## Database (additions — event sourcing upgrade happens here)

```
FailureScenario (id, mission_id FK, name, effect jsonb)
Mission.forked_from (self-referencing FK, nullable)
```

Plus: upgrade `MissionEvent` (from Product 3) from an audit-log-alongside-a-state-table into the *sole* source of truth, with periodic snapshots for replay performance — this is the one place the roadmap deliberately defers full event sourcing *to*, per `aspis-simplification-review.md` item #7.

## Deployment / Testing / Documentation

First genuinely multi-service deployment only if `TwinOrchestrator` was actually extracted (see above — don't do this preemptively). Testing: scenario tests — inject a known fault, assert the expected telemetry/alert sequence, exactly reproducible on replay. Documentation: `docs/simulation/failure-injection-catalog.md`, an ADR if/when the service split actually happens.

## Timeline (2-week sprints, ~9 weeks)

| Sprint | Goals | Success criteria |
|---|---|---|
| 1–2 | Real power/thermal models | Golden-file validated against a published reference case |
| 3–4 | ADCS module | Validated against a published attitude-dynamics test case |
| 5–6 | Frame-playback Replay + `MissionEvent` event-sourcing upgrade | Replay scrubber works over real recorded missions |
| 7–8 | Failure Injection | Instructor injects a fault mid-mission, correct downstream alerts fire |
| 9 | Scenario Comparison (fast-follow) + upsell rollout | First existing customer upgrades specifically for Digital Twin features |

## Business Strategy

Not sellable independently — it requires Products 2 and 3's data to mean anything, and that's fine, it's an upsell (Pro/Enterprise tier on top of the Mission Control subscription), not a standalone SKU. Funds Product 5's AI investment.
