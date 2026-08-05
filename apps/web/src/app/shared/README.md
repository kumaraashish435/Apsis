# shared — grows with every product, currently empty

## What this module is for

Cross-product UI: components, chart wrappers, and HTTP services used by more than one product module. Nothing lives here that only one product needs — that belongs inside that product's own folder instead.

## What's genuinely shared today (Product 1)

Nothing yet — Product 1 is the only active product, so nothing has needed to be shared *between* products. Don't pre-populate this folder with speculative "might be useful later" components.

## Process: when something belongs here

Move a component/service here the moment a **second** product needs it, not before — e.g., when Product 2's `budgets/` view wants the same chart component Product 1's `comparison/` view built, that's the signal to extract it into `shared/` and have both import from there. Until then, duplication between two products' folders is fine and even preferable to a premature shared abstraction that guesses wrong about what both actually need.

## Likely future residents (not built yet, don't build ahead of need)

- A chart-wrapper component, once Product 1's `comparison/` and Product 3's `telemetry-charts/` both need the same charting primitive.
- Status-severity color/badge components, once Product 3's alerts and Product 5's anomaly feed both need consistent nominal/warning/critical styling.

Previously listed here: an HTTP interceptor for auth token attachment. That's built now (`Modules/Users` issues real JWTs) — it lives in `../auth/auth.interceptor.ts`, next to the `AuthService` it depends on, not here. See `../auth/README.md`.
