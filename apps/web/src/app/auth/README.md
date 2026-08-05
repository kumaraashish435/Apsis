# auth — Product 1, active

Backend counterpart: `apps/api/Apsis.Api/Modules/Users/README.md` — read that first for how tokens are issued and validated server-side; this file covers what the Angular side does with them.

## What this module does

Sign-in, sign-up, session storage, and the plumbing that attaches the session to every API call and keeps unauthenticated users out of the app.

## How it works today

| File | Role |
|---|---|
| `auth.models.ts` | `AuthUser`, `AuthResponse` — mirrors the API's `UserResponse`/`AuthResponse` DTOs. |
| `auth.service.ts` | Session state: a `user` signal, `signIn`/`signUp`/`signInWithGoogle`/`signOut`, `googleClientId()`, and where the token lives. |
| `auth.guard.ts` | `CanActivateFn` — redirects to `/auth/sign-in` when there's no session. |
| `auth.interceptor.ts` | Attaches `Authorization: Bearer <token>` to outgoing requests; signs out on a `401`. |
| `sign-in/`, `sign-up/` | The two pages, each a standalone lazy-loaded route. |
| `google-sign-in-button/` | Renders Google's own "Sign in with Google" button once it knows the API's OAuth client id. Used identically from both pages. |
| `google-identity-loader.ts` | Loads `accounts.google.com/gsi/client` once, however many components ask for it. |
| `google-identity.types.ts` | Minimal ambient types for the bits of `window.google` this repo actually calls — not the full Google Identity Services SDK surface. |
| `_auth-form.scss` | Shared layout/styling `@use`'d by every page/component above (Sass partial — the leading `_` means it never compiles to its own CSS file). |

### Why these choices

- **Signals, not a `BehaviorSubject`/`Observable` store.** `AuthService.user` is a plain writable signal, exposed read-only via `.asReadonly()`, with `isAuthenticated` as a `computed()`. Nothing else in this codebase reaches for RxJS state containers for simple synchronous state, and signals are what Angular 22's own reactivity (`@if`, template bindings) is built around — no `| async` needed anywhere a component reads `auth.user()`.
- **`localStorage`, not a cookie.** The token is opaque to the browser (no server-set cookie, no CSRF token dance) and needs to survive a full page reload without a round-trip — `AuthService` reads it back out synchronously in its field initializer (`readStoredUser()`) so `isAuthenticated()` is correct on the very first render, before any HTTP call completes.
- **A functional interceptor, not an `HttpInterceptor` class.** Matches how the rest of the app config is written (`app.config.ts` already uses `provideRouter`/functional providers, no NgModules anywhere) — `withInterceptors([authInterceptor])` is the Angular 22-idiomatic form.
- **The interceptor also handles 401 globally.** Rather than every component that calls the API separately checking for an expired/invalid token, one `catchError` in `auth.interceptor.ts` calls `auth.signOut()` on any `401` (except from the login/register calls themselves, where a 401 just means "wrong password" — see `PUBLIC_PATHS`). This is the one piece of cross-cutting auth logic in the frontend; everything else is local to a component.
- **Dev-server proxy (`proxy.conf.json`) instead of a hardcoded API origin.** `ng serve` forwards `/api/*` to `http://localhost:5073`, so the frontend code just calls relative URLs (`/api/v1/auth/login`) — the same code works unmodified once frontend and API are served from the same origin in production. The backend's CORS policy (`Cors:AllowedOrigins` in `appsettings.json`) is a second line of defense for anyone hitting the API directly from `:4200` without the proxy (e.g. hitting the API from a REST client during dev).
- **The Google client id comes from the API, not a hardcoded frontend constant.** `GoogleSignInButton` calls `GET /api/v1/auth/google/client-id` on init rather than importing a config value — one source of truth (the API's config) instead of keeping two copies in sync. It's safe to fetch unauthenticated because a Google OAuth client id isn't a secret (see the backend README for why this flow has no client secret at all).
- **Google sign-in degrades to "hidden," not "broken."** If the API returns an empty client id (Google sign-in unconfigured) or the script fails to load, `GoogleSignInButton` shows a one-line hint instead of a button that can never work — see `unavailable` in `google-sign-in-button.ts`. The rest of the page (email/password) is completely unaffected either way, which is why this repo works out of the box with zero Google Cloud setup.
- **The Google script is loaded dynamically, not via a `&lt;script&gt;` tag in `index.html`.** `google-identity-loader.ts` injects `accounts.google.com/gsi/client` lazily, only once a sign-in/sign-up page actually renders — pages that never touch auth (nothing else exists yet, but this matters as more products land) never pay for it.

### Request flow

1. **Sign-up** (`sign-up/sign-up.ts`) — a `ReactiveFormsModule` form (name, email, password ≥ 8 chars) posts to `AuthService.signUp()`, which calls `POST /api/v1/auth/register`, stores the returned token + user via `setSession()`, and the component navigates to `/`. A `409` from the API renders as "An account with this email already exists."
2. **Sign-in** (`sign-in/sign-in.ts`) — same shape, `POST /api/v1/auth/login`. A `401` renders as "Invalid email or password." The sign-in page also displays the seeded default account's credentials directly (`admin@apsis.dev` / `Apsis#2026!`) so there's always something to log in with on a fresh checkout.
3. **Google sign-in** (`google-sign-in-button/google-sign-in-button.ts`, used from both pages) — on init, fetches the client id; if present, loads the Google script and calls `google.accounts.id.initialize()` + `renderButton()`. Google's own button, once clicked, hands back a signed ID token via the `callback` — that goes to `AuthService.signInWithGoogle()`, which `POST`s it to `/api/v1/auth/google`, stores the session the same way sign-in/sign-up do, and navigates to `/`. The backend finds-or-creates the account either way, so there's no separate "Google sign-up" — the button is identical on both pages.
4. **Every route change** — `app.routes.ts` puts `canActivate: [authGuard]` on the root path (the 3D viewer). No session → redirected to `/auth/sign-in` before the route's component ever loads.
5. **Every API call** — `authInterceptor` reads `AuthService.token` and adds the `Authorization` header, except for the register/login/google calls themselves (they have no token yet).
6. **Sign-out** — `TopBar` (see `../components/top-bar/`) shows the signed-in user's name and a sign-out button once `userName` is set; `Viewer3d` wires `auth.user()?.displayName` and `auth.signOut()` into it. `signOut()` clears `localStorage` and the `user` signal, then navigates to `/auth/sign-in`.

## Process: how this module gets extended safely

If a second product needs auth state (e.g. Mission Planner's org switcher reading `auth.user()`), that's fine to import directly from here — this module is meant to be depended on by every future product, unlike `shared/` which only holds things two *product* modules both independently built and then converged on. Don't move `AuthService` into `shared/` just because it's used from multiple places; it already lives at the right altitude.

`shared/README.md` previously predicted the auth interceptor would land in `shared/` once JWTs were real — it lives in `auth/` instead, next to `AuthService` it depends on. `shared/README.md` has been updated to match.

## Setting up Google sign-in locally

The button renders "isn't configured yet" out of the box — nothing to do unless you specifically want to test the Google flow. To turn it on: get a Google OAuth Client ID (see the backend README's "To turn on Google sign-in" section) and set `Google:ClientId` in the API's config. The frontend needs no config of its own — it reads the client id from the API.

## What's not built yet

- Refresh tokens — the JWT just expires (`Jwt:AccessTokenMinutes`, default 60) and the user is signed out on the next `401`. Fine for Product 1; revisit if session length becomes a real complaint.
- Route-level role checks in the frontend (e.g. hiding an "Admin" nav item for non-admins) — nothing in the UI needs it yet since `GET /api/v1/users` (the only admin-only endpoint) has no frontend page calling it.
