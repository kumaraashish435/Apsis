# Modules/Users — Product 1, active

Full product spec: `docs/products/product-1-orbital-simulator.md`.

## What this module does

Owns account identity for the whole platform: sign-up, sign-in, and "who is the current user." It's the one module every other module (present and future) trusts without re-checking — every other controller in the API assumes a request has already been authenticated by the time it runs.

## How it works today

Auth is real, two ways in: email/password, and Google sign-in. Both end with the same thing — a JWT that `[Authorize]` on other controllers validates. Frontend counterpart: `apps/web/src/app/auth/README.md`.

### Pieces

| File | Role |
|---|---|
| `User.cs` | The EF Core entity — `Id`, `Email`, `PasswordHash` (nullable — see below), `GoogleId` (nullable), `DisplayName`, `Role`, `OrgId` (nullable, unused until Product 2), `CreatedAt`. |
| `AuthDtos.cs` | Request/response shapes: `RegisterRequest`, `LoginRequest`, `GoogleAuthRequest`, `GoogleClientIdResponse`, `AuthResponse`, `UserResponse`. |
| `AuthController.cs` | `POST /api/v1/auth/register`, `POST /api/v1/auth/login`, `POST /api/v1/auth/google`, `GET /api/v1/auth/google/client-id`. All public — no `[Authorize]`. |
| `UsersController.cs` | `GET /api/v1/users/me` (any authenticated user), `GET /api/v1/users` (Admin role only — demonstrates role-based authorization). |
| `../../Security/PasswordHasher.cs` | PBKDF2-SHA256 hashing, 210k iterations. Not tied to any entity — pure `string → string`. |
| `../../Security/JwtOptions.cs`, `JwtTokenService.cs` | Reads the `Jwt` config section, mints signed JWTs with the user's id/email/name/role as claims. |
| `../../Security/GoogleOptions.cs` | Reads the `Google` config section — just `ClientId`, which is public (not a secret), see below. |
| `../../Data/ApsisDbContext.cs` | The `DbSet<User>`, unique index on `Email`, filtered unique index on `GoogleId` (filtered so multiple `NULL`s — every email/password-only account — don't collide). |
| `../../Data/DbSeeder.cs` | Creates the schema and seeds the default admin account on startup. |

### Why these choices

- **SQLite, not Postgres.** The original plan here said Postgres via EF Core. There's no Postgres infra in the repo yet (`infra/docker/` is empty), and standing one up is out of scope for wiring up auth. SQLite is a single `apsis.db` file, needs no setup, and EF Core's provider swap is one line (`UseSqlite` → `UseNpgsql` + connection string) whenever `infra/docker` grows a Postgres service. `Program.cs` and `appsettings.json` both call this out inline.
- **Hand-rolled PBKDF2 hasher, not ASP.NET Core Identity.** Identity's `UserManager`/`SignInManager`/`IdentityDbContext` bring a lot of machinery (lockout, email confirmation, its own migrations) that Product 1 doesn't need. `PasswordHasher.cs` is ~30 lines using the same primitive (`Rfc2898DeriveBytes.Pbkdf2`) Identity uses internally. Adding Google as a second provider didn't end up needing Identity either — see below.
- **Google sign-in verifies an ID token; it never sees a client secret.** `AuthController.GoogleSignIn` calls `GoogleJsonWebSignature.ValidateAsync` (the `Google.Apis.Auth` package) on a credential the *frontend* already obtained from Google Identity Services — the API's job is just to check Google's signature and pull `email`/`sub`/`name` out of the verified payload. There's no OAuth "exchange an authorization code for a token" step anywhere in this flow, which is why `GoogleOptions.ClientId` is the only config value needed and why it's safe to expose over `GET /api/v1/auth/google/client-id` — a client id is a public identifier, not a secret, in this particular OAuth flow (the "implicit"/ID-token flow, as opposed to the authorization-code flow which does have a secret).
- **Find-or-create, keyed by `GoogleId` first, then `Email`.** First-time Google sign-in with an email that already has a password account **links** `GoogleId` onto the existing row rather than creating a duplicate — so someone who registered with email/password can add "Sign in with Google" later without ending up with two accounts. `PasswordHash` had to become nullable for this: a Google-only account has nothing to hash. `Login` (email/password) treats a null `PasswordHash` the same as a wrong password — there's nothing to verify against, and the error message doesn't distinguish the two cases (same anti-enumeration reasoning as the existing wrong-password path).
- **Google sign-in is optional infrastructure, not a hard dependency.** With `Google:ClientId` unset (the checked-in default), `GET /api/v1/auth/google/client-id` returns an empty string and `POST /api/v1/auth/google` returns `501 Not Implemented` instead of crashing or 500ing. The frontend reads the empty client id and hides the button entirely — see `apps/web/src/app/auth/README.md`. This means the repo works out of the box with zero Google Cloud setup; Google sign-in is additive once someone configures it.
- **JWT bearer, not cookies.** The frontend is a separate Angular SPA hitting the API cross-origin in dev (`:4200` → `:5073`) and the token needs to survive to future non-browser clients (mobile, CLI) without CSRF-token plumbing. `JwtTokenService` signs with HMAC-SHA256 using a symmetric key from config (`Jwt:SigningKey`) — fine for a single API instance; move to asymmetric keys only if/when the API is split into multiple services that each need to *verify* but not *issue* tokens.
- **Default admin user, seeded not documented-only.** `DbSeeder.SeedAsync` runs once at startup (`Program.cs`), calls `EnsureCreated()` then inserts `admin@apsis.dev` / `Apsis#2026!` (see `DbSeeder.cs` constants) if no user with that email exists yet. This means the sign-in page always has something to log into on a fresh clone, with no manual seeding step.
- **`EnsureCreated()`, not migrations.** `Microsoft.EntityFrameworkCore.Design` is already referenced so `dotnet ef migrations add` works whenever the schema needs versioned history (e.g. once a second developer needs to evolve the `User` table without dropping data). Until then, `EnsureCreated()` is one line and matches the fact that Product 1 doesn't have a deployed database to migrate yet.

### Request flow

1. `POST /api/v1/auth/register` — validates the DTO (`[Required]`, `[EmailAddress]`, min-length password), 409s on a duplicate email, otherwise hashes the password, inserts the `User`, and returns the same `AuthResponse` shape as login (token + user) so the frontend can sign the user in immediately after sign-up.
2. `POST /api/v1/auth/login` — looks up by normalized (trimmed, lowercased) email, verifies the password hash in constant time (`CryptographicOperations.FixedTimeEquals`), and returns 401 with an identical message for "no such user", "wrong password", and "this account has no password" (Google-only) so the API never confirms which case applies.
3. `GET /api/v1/auth/google/client-id` — returns the configured Google OAuth client id (or `""` if unset). The frontend calls this before rendering the Google button at all.
4. `POST /api/v1/auth/google` — verifies the ID token against Google's public keys and the configured client id (`GoogleJsonWebSignature.ValidateAsync`), 501s if Google sign-in isn't configured, 401s if the token doesn't verify, otherwise finds-or-creates the `User` (by `GoogleId`, falling back to `Email` to link an existing account) and returns the same `AuthResponse` shape as the other two.
5. Every other request — `UseAuthentication()` then `UseAuthorization()` in `Program.cs` (in that order, before `MapControllers()`) validates the bearer token's signature, issuer, audience, and expiry. `[Authorize]` on `UsersController` means an invalid/missing token gets a 401 before the action method ever runs; `[Authorize(Roles = Roles.Admin)]` on `GET /api/v1/users` layers a 403 on top for non-admins.

### Configuration

`appsettings.json` holds non-secret shape (`Jwt:Issuer`, `Jwt:Audience`, `Jwt:AccessTokenMinutes`, `Cors:AllowedOrigins`, `Google:ClientId` — empty by default, the SQLite connection string). `appsettings.Development.json` holds `Jwt:SigningKey` — a dev-only placeholder value, checked in on purpose because it signs nothing that matters outside a local sandbox. **Before any real deployment**, override `Jwt:SigningKey` via user-secrets or an environment variable (`Jwt__SigningKey`) — never commit a production signing key.

**To turn on Google sign-in:** create an OAuth 2.0 Client ID (type "Web application") in the Google Cloud Console, add `http://localhost:4200` under Authorized JavaScript origins for local dev, and set `Google:ClientId` to it (`appsettings.Development.json`, user-secrets, or the `Google__ClientId` env var). No client secret is needed anywhere in this repo — see "Google sign-in verifies an ID token" above for why.

**Schema note:** `GoogleId` and the now-nullable `PasswordHash` were added after the database had already been seeded once via `EnsureCreated()` (see below) — `EnsureCreated()` doesn't alter an existing schema, so a pre-existing local `apsis.db` needs deleting (it's gitignored and disposable) before these columns show up. Not an issue for a fresh clone.

## Process: how this module gets extended safely

Any change to the `User` entity's shape is a schema change every later product depends on (`Modules/Orgs` in Product 2 adds the membership join table against this same `Id`). Treat additive-only changes as the default — add a column, don't rename or remove one. `PasswordHash` and `GoogleId` both went from non-nullable/absent to nullable/present exactly this way when Google sign-in was added.

If you add a password-reset flow or a third identity provider, that's worth reassessing the "no full Identity package" decision above — this module was built to make that swap easy (`User`/`PasswordHasher`/`JwtTokenService`/`GoogleOptions` are independent pieces, not entangled with a specific auth framework), not to resist it forever. Adding Google as a second provider didn't require it, for what that's worth.

## Testing

Manually verified end-to-end while building this: register → login → `/me` → `/users` as admin vs. non-admin → duplicate-email 409 → wrong-password 401 → Google client-id endpoint (both unconfigured-empty and populated) → Google sign-in 501-when-unconfigured. Still needs: automated integration tests for the same round-trips (including a real Google ID token fixture, which means mocking `GoogleJsonWebSignature.ValidateAsync` or its HTTP dependencies — Google's keys aren't something a test can call out to live), plus a negative test that an expired/tampered JWT is rejected — see `Apsis.Api.Tests/`.
