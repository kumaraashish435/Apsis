# ASPIS Development Process

How work actually moves through this repo day to day. See `aspis-blueprint.md` (repo root) Section 15 for the reasoning behind these choices; this doc is the short, operational version.

## Branching and PRs

Trunk-based development: short-lived feature branches off `main`, PR review required before merge, CI must pass, `main` stays protected. No long-lived `develop`/`release` branches — that overhead doesn't pay for itself at a 2-person team size.

A branch can (and often should) touch both `apps/web` and `apps/api` in the same PR when a feature spans the stack — e.g., a new API endpoint plus the Angular component that calls it. That's the whole point of the monorepo (see `docs/architecture/folder-structure.md`'s note on this): one reviewable, atomic change instead of two PRs that have to land in the right order.

## Sprint cadence

2-week sprints, tracked as a lightweight Kanban board (GitHub Projects is enough for two people — no separate PM tool). Each product's sprint plan lives in its own doc under `docs/products/` — that's the actual source of truth for "what are we building this sprint," not a separate project-management artifact that can drift out of sync with it.

## What "done" means for a change

1. **Builds clean** — `dotnet build Apsis.slnx` and `ng build` both pass with no new warnings introduced.
2. **Tests pass** — `dotnet test Apsis.slnx` and `ng test`. If the change touches `Propagation/`, the golden-file suite in `Apsis.Api.Tests/Propagation` must pass — see that folder's README for what that suite actually checks and why it's non-negotiable.
3. **Docs updated in the same PR, not a follow-up** — if a change alters what a module does or how it's meant to be built, the module's own `README.md` gets updated alongside the code, not left to drift. A README that describes last month's plan instead of this month's code is worse than no README.
4. **No infrastructure introduced without a stated trigger.** Before adding a new piece of infrastructure (a message broker, a new database technology, a new deployment target, a new service), check `aspis-simplification-review.md` — if it's on that list, its documented trigger condition must actually be true, not just "it'll be useful eventually." This is the single most consistent judgment call across the whole roadmap, worth checking explicitly rather than re-deciding from scratch each time.

## Testing philosophy, briefly

Golden-file/scenario tests for anything physics- or correctness-critical (propagation, later subsystem models) matter more than raw coverage percentage. Integration tests for API surfaces. No end-to-end 3D visual regression suite — it's a poor time investment for a small team; rely on validated underlying data plus manual QA for visual pages.

## Deployment cadence

Merge to `main` → automatic deploy to staging → manual approval → production. See each product's own doc under `docs/products/` for what "production" means at that product's stage (Product 1: Azure App Service; Product 3+: Azure Container Apps — see `infra/k8s/README.md` for why not Kubernetes by default).

## Documentation ownership

Every folder that isn't trivially self-explanatory gets a `README.md` stating: what it does, how it works (or how it's meant to work, if not built yet), and the process for extending it safely. This isn't a one-time exercise — when a module's build trigger condition is met and work actually starts on it, its README should be the first thing re-read, and the first thing updated once the real implementation diverges from the plan.
