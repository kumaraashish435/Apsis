# ai-assistant — Product 5 frontend module

Status: **not yet built.** Build trigger: Products 1–4 exist and are generating real telemetry/event-log/failure-injection data. Full product spec: `docs/products/product-5-ai.md`.

## What this folder will contain

```
ai-assistant/
├── ai-assistant.routes.ts
├── anomaly-feed/       List of AI-flagged anomalies, clickable into mission-control's timeline
└── chat-panel/          Conversational assistant — docked alongside telemetry, not a separate page
```

## How to build it, in order

1. **`anomaly-feed/` first** — even the rule-based v1 anomaly detector (which may ship as early as Product 3/4's backend) needs somewhere to surface in the UI. This is a plain list, each entry linking into `mission-control/timeline` at the relevant event.
2. **`chat-panel/` second**, built as a docked panel component embeddable inside `mission-control/dashboard`, not a standalone route — the assistant should always have the current mission's context in view rather than requiring the user to navigate away from ops to ask a question. Backed by the `ai-services/assistant` FastAPI service (see `apps/ai-services/README.md`).

## What NOT to build here yet

Don't build UI for Predictive Maintenance, Orbit Optimization, or Autonomous Recommendations as separate pages until their backend models exist and have been validated — see the sequencing in `docs/products/product-5-ai.md`. Surfacing an AI feature in the UI before its model is trustworthy erodes trust in every other AI feature shipped after it.
