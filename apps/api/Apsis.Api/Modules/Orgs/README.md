# Status: not yet built

This folder belongs to **Product 2 — Mission Planner**.

**Contains:** Organizations, Projects, membership, RBAC.

**Build trigger:** Introduced in Product 2, not before — Product 1 is deliberately single-user. The `User` table already carries an unused `org_id` so this is an addition, not a migration.

See `docs/architecture/folder-structure.md` for the full monorepo map and build order.
