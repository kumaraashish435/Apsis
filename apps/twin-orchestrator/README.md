# Status: not yet built

This folder belongs to **Product 4 — Digital Twin**.

**Contains:** the twin lifecycle service (start/stop/snapshot/fork/replay a running twin).

**Build trigger:** Extract this out of the API monolith ONLY once the twin lifecycle boundary is proven stable through Product 3's usage. Do not build this as a separate deployable service before then — see `aspis-simplification-review.md`, item #11 ("split only when justified, not because a boundary looks clean on paper").

See `docs/architecture/folder-structure.md` for the full monorepo map and build order.
