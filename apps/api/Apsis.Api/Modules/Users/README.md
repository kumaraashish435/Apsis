# Modules/Users — Product 1, active

Full product spec: `docs/products/product-1-orbital-simulator.md`.

## What this module does

Owns account identity for the whole platform: sign-up, sign-in, and "who is the current user." It's the one module every other module (present and future) trusts without re-checking — every other controller in the API assumes a request has already been authenticated by the time it runs.

## How it works today

`UsersController` (`GET /api/v1/users/me`) is currently a stub — it returns a placeholder message and does not yet issue or validate tokens. There is no `User` EF Core entity, no Postgres table, and no auth middleware wired into `Program.cs` yet.

## How it's meant to work once built out (Sprint 3-4)

1. Add a `User` EF Core entity: `Id`, `Email`, `PasswordHash` (if not using an external provider), and — important — a nullable `OrgId` column from the start, even though nothing reads it until Product 2's `Modules/Orgs` exists. This is the one field worth anticipating early; nothing else about Product 2 should be built here.
2. Wire up ASP.NET Core Identity for local email/password auth, plus Google OAuth as a second provider.
3. Issue JWTs on successful sign-in; add JWT bearer authentication middleware in `Program.cs` (`builder.Services.AddAuthentication(...)`) so `[Authorize]` becomes meaningful on other controllers.
4. `GET /api/v1/users/me` becomes a real endpoint returning the authenticated user's claims.

## Process: how this module gets extended safely

Any change to the `User` entity's shape is a schema change every later product depends on (`Modules/Orgs` in Product 2 adds the membership join table against this same `Id`). Treat additive-only changes as the default — add a column, don't rename or remove one, unless you've checked nothing downstream (there's nothing downstream yet, but there will be from Product 2 onward).

## Testing

Once real, this module needs integration tests for: sign-up → sign-in → token validation round-trip, and a negative test that an invalid/expired token is rejected. No unit tests are meaningful here yet since there's no logic beyond the stub.
