# apps/twin-orchestrator — Product 4, extracted service

Status: **not yet built as a separate service — and may never need to be.** Full product spec: `docs/products/product-4-digital-twin.md`.

## What this folder is for

The twin lifecycle: start/stop/snapshot/fork/replay a running mission twin. This is currently just a *set of responsibilities*, not a service — implement it as a module inside `apps/api/Apsis.Api` (e.g. `Modules/DigitalTwin/`) when Product 4 begins.

## The one rule that governs this folder

**Do not put code here until there is an actual operational forcing function** — independent scaling needs, an independent deploy cadence, or a language change — not just because "the boundary looks clean." The original architecture draft justified extracting this into its own deployable the moment Product 4 began, reasoning that "the boundary is stable." Stability alone isn't a reason to split a service; that reasoning broke the roadmap's own "microservices only when justified" principle. See `aspis-simplification-review.md` item #11.

## If and when extraction is actually justified

1. Confirm the specific forcing function (write it down — e.g., "twin simulation sessions need to scale independently of the CRUD API because of X measured load pattern").
2. Move the twin-lifecycle module's code from `apps/api/Apsis.Api/Modules/DigitalTwin/` into a new `Apsis.TwinOrchestrator` project here, referencing `libs/proto` if a gRPC contract to the main API is needed.
3. Write an ADR documenting the forcing function and the migration — don't split silently.

Until that day, this folder should stay empty except for this README.
