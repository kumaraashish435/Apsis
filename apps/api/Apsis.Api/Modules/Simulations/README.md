# Modules/Simulations — Product 1, active

Full product spec: `docs/products/product-1-orbital-simulator.md`.

## What this module does

CRUD for saved simulations, plus the endpoints that trigger propagation, propagator comparison, export, and the shareable read-only link. This is the core surface a user actually interacts with in Product 1 — auth and the frontend pages exist to support this module.

## How it works today

`SimulationsController` is fully implemented — every endpoint below is real, not a stub.

| Endpoint | Auth | What it does |
|---|---|---|
| `GET /api/v1/simulations` | owner | List the current user's simulations |
| `POST /api/v1/simulations` | owner | Create (TLE or classical elements + a propagation window) |
| `GET/PUT/DELETE /api/v1/simulations/{id}` | owner | Read/update/delete one |
| `GET .../{id}/propagate` | owner | Propagate and return state vectors + osculating elements at each point |
| `GET .../{id}/compare` | owner | SGP4 vs. Keplerian divergence (TLE-based simulations only) |
| `GET .../{id}/export?format=csv\|json\|czml` | owner | Download the propagated series |
| `POST/DELETE .../{id}/share` | owner | Turn the share link on/off |
| `GET .../shared/{token}` `.../shared/{token}/propagate` `.../shared/{token}/compare` `.../shared/{token}/export` | **none** | Read-only mirror of the above, gated by `IsPublic` instead of ownership |

### Pieces

| File | Role |
|---|---|
| `Simulation.cs` | The EF Core entity. `InputType` (`Tle` \| `Elements`) selects which of two field groups is populated — `TleLine1`/`TleLine2`, or the six classical-element columns. `ShareToken` is generated at creation time regardless of whether the simulation is ever shared; `IsPublic` is the actual gate the public endpoints check. |
| `SimulationDtos.cs` | `SaveSimulationRequest` (create/update), `SimulationResponse`, `StateVectorResponse` (adds `latDeg`/`lonDeg` via `GeodeticConverter` on top of the raw `StateVector`), `PropagateResponse`, `CompareResponse`/`DivergencePoint`, `ShareResponse`. |
| `SimulationsController.cs` | Everything above. |

### Why these choices

- **Only inputs are persisted; propagated output never is.** A `Simulation` row is a TLE or six numbers plus a time window — every `/propagate`, `/compare`, and `/export` call recomputes state vectors fresh from `ISgp4Propagator`/`IKeplerianPropagator`. This means a future propagator bug fix or accuracy improvement applies retroactively to every saved simulation with zero migration — there's no stale cached output anywhere to invalidate.
- **A capped sample count (`MaxSamplePoints = 20_000`), not an unbounded window.** Nothing stopped an authenticated caller from requesting a 50-year window at a 1-second step and forcing this endpoint to allocate, propagate, and serialize tens of millions of points from one request. `ValidateInput` rejects that before propagation ever starts — the cap is generous (13+ days at 1-minute steps) for what this product actually needs, not a workaround for a real use case anyone's hit.
- **The share link is always-generated, gate-checked, not created-on-demand.** `ShareToken` exists on every row from creation; `POST .../share` just flips `IsPublic`. This means unsharing and re-sharing keeps the *same* URL working again rather than invalidating a previously-distributed link — a deliberate choice given "share it, then temporarily take it down, then share the identical link again" is a plausible real workflow and "every share is a new URL" would surprise anyone who bookmarked the old one.
- **Compare only works for TLE-based simulations, and says why.** SGP4 needs a TLE; there's no honest way to synthesize one from six classical elements (a real TLE also encodes a drag term — `BSTAR` — that elements alone don't have). Rather than silently degrading or faking a comparison, `CompareCore` returns a 400 with the actual reason. For a TLE-based simulation, the Keplerian series is seeded from the *osculating* elements at SGP4's first propagated point (via `OrbitalElementsCalculator`), not from the TLE's own mean elements — so both series start from the same instantaneous state and the divergence shown is purely what SGP4's perturbation terms add from there, not an artifact of two different starting orbits.
- **Export builds CSV/CZML by hand, not via a library.** Both formats are simple enough (a flat table; a two-packet CZML document) that a dependency would cost more than it saves. CSV explicitly uses `CultureInfo.InvariantCulture` for every number — without that, a server running under a locale that uses `,` as a decimal separator would silently corrupt the file. CZML uses `referenceFrame: "INERTIAL"` because the propagators output ECI/TEME, not an Earth-fixed frame — the honest CZML term for that, even though this app's own 3D viewer is Three.js, not Cesium (CZML is Cesium's format, but it's a real interop win for anyone who wants to open the export in an actual Cesium-based tool).
- **The public `/shared/*` routes are a straight mirror of the owner routes, not a cut-down subset.** A shareable link that can view the globe but not export or compare would be a worse product than "everything the owner sees, minus the ability to change it" — so `GetShared`/`PropagateShared`/`CompareShared`/`ExportShared` exist as `[AllowAnonymous]` counterparts calling the exact same `PropagateCore`/`CompareCore`/`ExportCore` private methods the owner routes use, gated by `FindPublic(token)` (requires `IsPublic == true`) instead of `FindOwned(id)`.

### Request flow

1. `SimulationEditor` (frontend) submits `SaveSimulationRequest` → `POST /api/v1/simulations` validates (`ValidateInput`: TLE parses via the real `TLE` class, or elements are in physically sane ranges; window produces ≤ 20,000 points) → persists.
2. Viewer loads → `GET .../{id}/propagate` → `TryPropagate` picks `ISgp4Propagator` or `IKeplerianPropagator` based on `InputType` → each `StateVector` becomes one `StateVectorResponse` (adds lat/lon) and one `OrbitalElementsReadout` (the classical-elements readout panel) → both arrays, same length, same order, returned together.
3. Comparison page → `GET .../{id}/compare` → runs SGP4 and Keplerian over the *same* time grid, returns both series plus a `Divergence` array (Euclidean distance in km at each matching time step).
4. Export button → `GET .../{id}/export?format=...` → same propagation as above, serialized to the requested format, returned as a file download (`Content-Disposition` via `File(...)`).
5. Share button → `POST .../{id}/share` → `IsPublic = true` → frontend builds `https://.../shared/{shareToken}` → anyone with that URL hits the `[AllowAnonymous]` routes, no session required.

## Process: how this module gets extended safely

`Simulation`'s two input-type field groups (`TleLine1`/`TleLine2` vs. the six `*Deg`/`Km` columns) are mutually exclusive by convention (`ApplyInput` nulls out whichever group doesn't match `InputType`), not by a database constraint — if a third input type is ever added (e.g. a NORAD catalog number that fetches its own TLE), keep that same "only one group populated" discipline rather than adding cross-field validation complexity.

## Testing

Manually verified end-to-end against a running instance with a real ISS TLE (fetched from Celestrak): create → propagate (values cross-checked: ~419 km altitude, ~93 min period, 51.6° inclination — all correct for ISS) → compare (divergence grows from 0 to tens of km over 2 hours, consistent with real drag/J2 perturbation magnitude) → export in all three formats → share → fetch via the public endpoint with no auth header → unshare → confirm the public endpoint then 404s. Also verified: a second user cannot list, read, or act on the first user's simulations (404, not 403 — this module doesn't reveal that a simulation exists to a non-owner). Automated coverage is currently at the propagator layer (`Apsis.Api.Tests/Propagation/`, 640+ assertions) — this controller's CRUD/ownership/validation logic doesn't have its own integration test suite yet.
