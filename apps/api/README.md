# ASPIS API — Backend

ASP.NET Core (.NET 10), controller-based, one solution (`Apsis.slnx`), one deployable project (`Apsis.Api`) — a **modular monolith**, not microservices. See `docs/architecture/folder-structure.md` at the repo root for the full monorepo map and `docs/products/` for what each product actually does.

## What's here right now

```
Apsis.Api/
├── Program.cs                       Controllers + OpenAPI + /health. TODOs mark where
│                                     EF Core/Postgres and Identity/auth get wired in.
├── Modules/
│   ├── Users/          Product 1 — ACTIVE. UsersController (auth stub)
│   ├── Simulations/    Product 1 — ACTIVE. SimulationsController (CRUD/propagate/compare/export stubs)
│   ├── Orgs/            Product 2 — not built, see its README.md
│   ├── MissionPlanner/   Product 2 — not built, see its README.md
│   ├── MissionControl/   Product 3 — not built, see its README.md
│   ├── Commands/         Product 3 — not built, see its README.md
│   └── Reports/          Product 3 — not built, see its README.md
└── Propagation/
    ├── Sgp4/         ISgp4Propagator — Product 1 Sprint 1-2, in-process (no gRPC, see ADR-0001)
    └── Keplerian/     IKeplerianPropagator — Product 1 Sprint 7-8, comparison-view only

Apsis.Api.Tests/
└── Propagation/       Golden-file tests go here — the single most important test suite
                         in the company. Every propagator implementation must pass this
                         suite against Vallado's published SGP4 reference vectors before
                         it ships. Nothing else in the roadmap is trustworthy if this isn't.
```

Every `Modules/*` folder is a **namespace inside one project**, not a separate assembly or service. This stays true through Products 1–3; the only planned exception is `Apsis.TwinOrchestrator` in `apps/twin-orchestrator/`, extracted at Product 4 once its boundary is proven — see that folder's README.

## How to run it

```bash
dotnet restore                 # first time only
dotnet build Apsis.slnx         # verify it compiles
dotnet run --project Apsis.Api  # http://localhost:5xxx, /health, /openapi (dev only)
dotnet test Apsis.slnx           # runs Apsis.Api.Tests
```

## Why it's built this way

- **Controllers, not minimal APIs** — matches conventional ASP.NET Core patterns and keeps each module's HTTP surface in one obvious file (`<Module>Controller.cs`) as the module count grows across Products 1–3.
- **In-process propagation, no gRPC engine yet** — `ISgp4Propagator` is a plain C# interface. A separate C++ engine only gets introduced at Product 4, when real subsystem physics (ADCS/thermal/power) genuinely needs a dedicated process — see `aspis-simplification-review.md` item #1 and ADR-0001. Don't add `apps/sim-engine` code before then.
- **No database wired up yet** — Postgres/EF Core is a Sprint 3-4 task (see `docs/products/product-1-orbital-simulator.md`), deliberately not done in the initial scaffold so the propagation core (the actual IP) gets built and golden-file-tested first.
- **`org_id`-shaped thinking starts now, org enforcement doesn't** — when the `Users` EF Core entity is added in Sprint 3-4, give it a nullable `OrgId` even though `Orgs/` isn't built until Product 2. That's the one piece of Product 2 to anticipate now; everything else in `Orgs/`, `MissionPlanner/`, etc. should stay untouched until their turn.

## Next concrete step

Implement `ISgp4Propagator` for real and get it passing the golden-file suite in `Apsis.Api.Tests/Propagation` — see the Sprint 1-2 plan in `docs/products/product-1-orbital-simulator.md`. Everything else in the company depends on this being right.
