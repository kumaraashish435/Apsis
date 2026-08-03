# Modules/Orgs — Product 2 backend module

Status: **not yet built.** Build trigger: Product 1 (Orbital Simulator) is live and validated. Full product spec: `docs/products/product-2-mission-planner.md`.

## What this folder will contain

`Organization`, `Project`, and org membership — the first multi-tenancy in the codebase. `OrgsController`, `ProjectsController`.

## How to build it

1. Add `Organization` and `Project` EF Core entities. Give `Project` an `OrgId` FK.
2. Extend the existing `Modules/Users` `User` entity: the `OrgId` field it's carried since Product 1 (nullable, unused) becomes load-bearing here via an `OrgMembership` join table (`user_id`, `org_id`, `role`).
3. **Ship a single role: `"member"`.** Every member of an org can do everything within it. Do **not** build Viewer/Editor/Admin tiers now — see `aspis-simplification-review.md` item #4. Add granular roles only when a real paying customer has an actual internal permissions conflict; building it speculatively means testing and maintaining a permission matrix nobody asked for.
4. Auth middleware: once a request is authenticated, resolve the user's `org_id`/`project_id` from membership and scope every subsequent query to it. This is the pattern `Modules/Commands` (Product 3) later extends with a dedicated elevated permission — keep this module's role model simple so that extension is additive, not a rework.
5. REST: `GET/POST /api/v1/orgs`, `GET/POST /api/v1/projects`.

## What NOT to build here

No billing/plan-tier enforcement logic yet (manual invoicing is fine through Product 2 and 3 — see the business strategy sections in the product docs). No SSO/SAML (that's Product 8/Enterprise territory, years away).
