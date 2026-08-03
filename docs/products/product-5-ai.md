# Product 5 — ASPIS AI

Status: **not yet built.** Build trigger: Products 1–4 exist and are generating real telemetry/event-log/failure-injection data. AI must never exist independently — without that data it's a demo, not a product.

## AI Features — data, source, and model choice

| Feature | Data needed | Source | Model type | Why |
|---|---|---|---|---|
| Anomaly Detection | Nominal + off-nominal telemetry | Mission Control (P3) + Digital Twin failure injection (P4) | Traditional ML (rules → isolation forest/autoencoder) | Doesn't need deep learning's capacity at this data scale; simpler models are more explainable, which ops teams need to trust an alert |
| Predictive Maintenance | Long-horizon degradation trajectories | Digital Twin simulated degradation sweeps (P4) | Deep learning (LSTM/temporal transformer) | Genuinely sequential, long-horizon pattern recognition — where DL's capacity earns its cost |
| Orbit Optimization | Orbit/constellation design space | Orbital Simulator (P1) + Mission Planner (P2) | Traditional optimization; RL only for constellation-scale search | Most of this is a well-posed numerical optimization problem |
| Mission Planning Assistant | Component catalog, budget rules | Mission Planner (P2) schema + curated knowledge base | LLM + RAG | Advisory, conversational, reasons over unstructured "why" |
| Telemetry Summaries | Event log + telemetry over a range | Mission Control (P3) | LLM (summarization) | Natural-language generation over structured data |
| Root Cause Analysis | Anomaly candidates + correlated event log | Anomaly Detection output + Digital Twin event log | Hybrid: ML surfaces candidates, LLM explains | Neither model type alone solves this |
| Natural Language Queries | Full relational + time-series schema | All products' databases | LLM + RAG / text-to-SQL | Query interface, not a prediction task |
| Autonomous Recommendations | Everything above, composed | All products | RL/optimization + LLM narration | Longest-horizon feature, ships last |

## How to build it — sequence, don't bundle

The original plan treated all seven features as one 14-week bundle at equal effort. That's wrong: the LLM-based features (Assistant, NL Query, RCA narration) are "call an API + RAG over data already on hand" — a couple of weeks each. The from-scratch ML features (Anomaly Detection v2, Predictive Maintenance) are genuinely longer, separately-paced model-training investments. **Ship the LLM-based features first** for fast, visible AI value, and treat ML-from-scratch as its own track that doesn't gate the AI launch. See `aspis-simplification-review.md` item #14.

Recommended order:
1. Rule-based Anomaly Detection v1 (proves the AI → Mission Control integration path with zero ML risk — arguably can even start in Product 3/4 rather than waiting for this product).
2. LLM-based Assistant + NL Query (fast, visible value).
3. ML Anomaly Detection v2 (isolation forest/autoencoder, trained on Digital Twin synthetic data) — separately paced from step 2.
4. Predictive Maintenance (LSTM) — the longest, most data-science-heavy track.
5. Root Cause Analysis (hybrid ML + LLM).
6. Orbit Optimization.
7. Autonomous Recommendations — narrowest possible scope for v1 (e.g., one recommendation flow like contact scheduling), last.

## Architecture

```
apps/ai-services/          Python/FastAPI, NEW at this product
├── anomaly_detection/
├── predictive_maintenance/
├── orbit_optimization/
├── assistant/               LLM + RAG
└── training_pipelines/      synthetic dataset generation from Digital Twin failure-injection sweeps
```

**Hard boundary, not a convention:** every AI service connects to Postgres/Timescale with a **read-only** database role, enforced at the credential level. AI services never write to simulation or telemetry state — AI failures can never corrupt the twin.

## Training strategy

The core advantage: because ASPIS owns the Simulation Engine and Failure Injection module, it can generate labeled fault data at arbitrary scale — something a real satellite operator with only rare real fault history cannot do. Public benchmark datasets (e.g., NASA's SMAP/MSL telemetry anomaly set) are used only to validate that models generalize beyond simulator-specific bias, never as primary training data. Real customer telemetry, once Product 3/4 customers have real spacecraft (with explicit consent), becomes a genuine proprietary moat layered on top — never assumed to exist before then.

## Timeline (2-week sprints, ~14 weeks, shipped incrementally per the sequencing above)

| Sprint | Goals | Success criteria |
|---|---|---|
| 1–2 | Rule-based Anomaly Detection v1 | Proves the AI→Mission-Control integration path |
| 3–4 | LLM Assistant + NL Query | Answers real user questions about their own mission data correctly |
| 5–6 | ML Anomaly Detection v2 | Detects injected faults it wasn't explicitly trained on |
| 7–8 | Predictive Maintenance (LSTM) | Forecast within acceptable error on held-out simulated degradation runs |
| 9–10 | Root Cause Analysis | Given an injected multi-subsystem fault, produces a correct causal explanation |
| 11–12 | Orbit Optimization | Produces a demonstrably better orbit/constellation than manual design for a test case |
| 13–14 | Autonomous Recommendations (v1, narrow scope) | Recommendation accepted by a real ops team without modification in a pilot |

## Business Strategy

Not sellable independently — an add-on module, priced on top of Products 3/4 (usage-based or per-model, e.g., "+$X/mo per satellite" for anomaly detection, metered usage for LLM features). By this point the company should be cash-flow positive from Products 1–4; Product 5 is funded by the company, not by needing its own bootstrap revenue.
