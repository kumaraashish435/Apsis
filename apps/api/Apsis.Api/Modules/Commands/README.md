# Modules/Commands — Product 3 backend module

Status: **not yet built.** Build trigger: Product 2 (Mission Planner) is live and validated. Full product spec: `docs/products/product-3-mission-control.md`.

## What this folder will contain

Command dispatch, authorization, and the audit log. `CommandsController`.

## How to build it — with more rigor than any other module in Products 1-3

This is a real safety-relevant subsystem even though it only ever talks to simulated spacecraft right now, because the same interface will one day be extended toward a real one (far in the future, Product 7+, with a legal/export-control review gating that step — see `aspis-blueprint.md`'s risk notes). Build the discipline now, cheaply, rather than retrofitting it later:

1. **A dedicated `"can-send-commands"` permission, separate from `"can-view-telemetry"`.** Never bundle these by default in `Modules/Orgs`' single-role model — a project member who can watch a mission should not automatically be able to command it. This is the one deliberate exception to "keep RBAC simple" (item #4): the read/write split on commands specifically is worth the extra permission check from day one.
2. **Every command is validated, authorized, then dispatched as a `MissionEvent`** (via `Modules/MissionControl`'s event log — see that module's README for the schema), never applied directly. The in-process simulation tick loop picks up pending commands from the event log on its next tick.
3. **Full audit trail, non-negotiable:** who requested a command, when, whether it was authorized, and its execution result — all queryable, never deleted.
4. REST: `GET/POST /api/v1/missions/{id}/commands`. `CommandStatusHub` for live status push.

## What NOT to build here

No command *infrastructure* beyond what's above — no message broker, no separate command-processing service. The rigor is in the authorization/audit logic, not in distributed-systems machinery around it. See `aspis-simplification-review.md` item #6.

## Testing requirement specific to this module

A scenario test that proves an unauthorized user is provably blocked from sending a command must exist and pass before this module is considered done — this is called out explicitly in `docs/products/product-3-mission-control.md`'s Sprint 5-6 success criteria.
