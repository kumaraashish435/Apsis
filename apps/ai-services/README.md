# Status: not yet built

This folder belongs to **Product 5 — AI**.

**Contains:** Python/FastAPI services — anomaly detection, predictive maintenance, orbit optimization, the LLM assistant. All read-only against Postgres/Timescale, enforced at the database credential level.

**Build trigger:** AI must never exist independently — do not start this folder until Products 1-4 exist and are generating real telemetry/event-log/failure-injection data to learn from.

See `docs/architecture/folder-structure.md` for the full monorepo map and build order.
