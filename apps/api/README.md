# ASPIS API — Backend

ASP.NET Core (.NET 10), controller-based, one solution (`Apsis.slnx`), one deployable project (`Apsis.Api`) — a **modular monolith**, not microservices. See `docs/architecture/folder-structure.md` at the repo root for the full monorepo map and `docs/products/` for what each product actually does.

## What's here right now

```
Apsis.Api/
├── Program.cs                       Controllers + OpenAPI + /health + EF Core + JWT auth wiring.
├── Data/
│   ├── ApsisDbContext.cs             EF Core context (SQLite for now — see Modules/Users/README.md)
│   └── DbSeeder.cs                   Creates the schema + seeds the default admin user on startup
├── Security/
│   ├── PasswordHasher.cs             PBKDF2 password hashing
│   ├── JwtOptions.cs, JwtTokenService.cs   JWT issuance
│   └── GoogleOptions.cs              Google OAuth client id config
├── Propagation/
│   ├── Sgp4/                         ISgp4Propagator + Sgp4Propagator, golden-file tested. In-process (no gRPC, see ADR-0001).
│   ├── Keplerian/                    IKeplerianPropagator + KeplerianPropagator, tested by closed-form + conservation laws.
│   ├── OrbitalElementsCalculator.cs  State vector -> classical elements (the "readout" feature)
│   └── GeodeticConverter.cs          ECI -> lat/lon via GMST (the ground-track feature)
├── Modules/
│   ├── Users/          Product 1 — ACTIVE. AuthController (register/login/Google) + UsersController (me, admin list)
│   ├── Simulations/    Product 1 — ACTIVE. SimulationsController — full CRUD, propagate, compare, export (CSV/JSON/CZML), share
│   ├── Orgs/            Product 2 — not built, see its README.md
│   ├── MissionPlanner/   Product 2 — not built, see its README.md
│   ├── MissionControl/   Product 3 — not built, see its README.md
│   ├── Commands/         Product 3 — not built, see its README.md
│   └── Reports/          Product 3 — not built, see its README.md

Apsis.Api.Tests/
└── Propagation/       Golden-file (SGP4) + closed-form/conservation-law (Keplerian) suite —
                         the single most important test suite in the company. 640+ assertions
                         checking both propagators against independent ground truth, not each
                         other. See that folder's README.
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
- **Database is wired up, on SQLite rather than Postgres for now** — `infra/docker/` has no Postgres service yet, so `ApsisDbContext` runs on SQLite (`apsis.db`, gitignored) until that infra exists. See `Modules/Users/README.md` for the full reasoning and the one-line provider swap when Postgres lands.
- **`org_id`-shaped thinking starts now, org enforcement doesn't** — the `User` EF Core entity carries a nullable `OrgId` from day one even though `Orgs/` isn't built until Product 2. That's the one piece of Product 2 to anticipate now; everything else in `Orgs/`, `MissionPlanner/`, etc. should stay untouched until their turn.

## Next concrete step

Product 1's backend MVP is functionally complete — both propagators are implemented and tested, `SimulationsController` covers CRUD/propagate/compare/export/share, and auth (email/password + Google) is wired end to end. What's not built: automated integration tests for `SimulationsController` itself (see `Modules/Simulations/README.md`'s Testing section — currently verified manually, not by an automated suite), and Postgres (still SQLite — see "Why it's built this way" above).
