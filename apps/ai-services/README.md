# apps/ai-services — Product 5 (Python/FastAPI)

Status: **not yet built.** Build trigger: Products 1–4 exist and are generating real telemetry/event-log/failure-injection data. Full product spec: `docs/products/product-5-ai.md`. AI must never exist independently of that data.

## What this folder will contain

```
anomaly_detection/         Rule-based v1 (can start as early as Product 3/4), then ML v2
predictive_maintenance/     LSTM/temporal models — trained on Digital Twin synthetic degradation data
orbit_optimization/          Optimization service over the propagation engine
assistant/                   LLM + RAG — conversational assistant, NL query, root-cause narration
training_pipelines/          Synthetic dataset generation from Digital Twin failure-injection sweeps
```

## Hard rule, enforced at the database credential level, not just convention

Every service in this folder connects to Postgres/Timescale with a **read-only** role. AI services never write to simulation or telemetry state — this is what guarantees an AI failure or a bad model can never corrupt the digital twin. Set this up as the very first thing when this folder gets real content: create the read-only Postgres role before writing a single line of model code.

## Build order (see `docs/products/product-5-ai.md` for the full reasoning)

1. `anomaly_detection/` rule-based v1 — fast, proves the AI→Mission-Control integration path with zero ML risk.
2. `assistant/` — LLM + RAG over existing Postgres/Timescale data. This is "call an API," not model training — ship it early for fast, visible value; don't let it wait on the ML tracks below.
3. `anomaly_detection/` ML v2 (isolation forest/autoencoder) — its own separately-paced track, trained on `training_pipelines/`'s synthetic data from Digital Twin's failure injection sweeps.
4. `predictive_maintenance/` (LSTM) — the longest, most data-science-heavy track; don't let it gate the assistant launch.
5. `orbit_optimization/` and autonomous recommendations (narrow scope) — last.

## What NOT to do

Don't start any of this before Product 4 exists — there's no labeled fault data, no event log, no telemetry to be intelligent about, and an AI feature built on a demo dataset will need to be rebuilt anyway once real synthetic data is available.
