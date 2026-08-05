# Apsis.Api.Tests/Propagation — the most important test suite in the company

## What this folder does

Golden-file tests that check `Propagation/Sgp4` and `Propagation/Keplerian`'s output against published, independently-verified reference data. Every product from here forward — pass prediction (P2), simulated telemetry (P3), digital twin replay (P4), predictive maintenance training data (P5) — inherits its correctness from whatever this suite proves true about the propagators. If this suite is weak, every product built on top of it inherits that weakness silently.

## How it works today (SGP4)

| File | Role |
|---|---|
| `testdata/SGP4-VER.TLE` | 33 test TLEs, taken verbatim from Vallado's official SGP4 C++ distribution. Covers near-Earth (SGP4) and deep-space/resonant (SDP4) regimes, decaying orbits, near-singular low-inclination/low-eccentricity geometries, and one ~3.5-year-span stress test. |
| `testdata/tcppver.out` | The matching reference output — position/velocity at every published time step, computed by Vallado's own C++ reference implementation. |
| `Sgp4GoldenFile.cs` | Parses both files into `(TLE, expected states)` pairs. Cases are matched *positionally*, not by satellite number — satellite 20413 appears twice (a short run and the long stress-test run), so satellite number alone isn't a unique key. |
| `Sgp4VerificationTests.cs` | The actual assertions — see below. |

Both files were fetched from the `brandon-rhodes/python-sgp4` GitHub repo (`sgp4/SGP4-VER.TLE` and `sgp4/tcppver.out`), which mirrors Vallado's own distribution — this is the same pair of files virtually every independent SGP4 reimplementation validates against, not a bespoke or invented dataset.

### Where the data source's own comments matter

`SGP4-VER.TLE` tags several TLEs with comments describing exactly what edge case each one probes (e.g. "12h resonant ecc in 0.7 to 0.715 range", "Lyddane bug at &gt; 280.5 min for AcTan()", "check error code 4"). Those comments aren't decorative — they're why three satellites (33333, 33334, 33335) needed individual investigation rather than blanket inclusion in the accuracy suite:

- **33333, 33334** — genuinely degenerate elements. The C++ reference's own output block stops within a few rows of t=0 (33333: 5 rows instead of the 31 its declared range implies; 33334: 1 row, and that row is a stale artifact — it exactly duplicates 33333's *last* computed row, evidence the reference harness didn't clear its output buffer after an internal error). These are asserted as error cases (`Propagate_throws_for_intentionally_degenerate_elements`), not accuracy cases.
- **33335** — its TLE comment also claims to probe an error code ("try to check error code 3... looks like ep never goes below zero"), but unlike the two above, its reference block is a *complete* 73/73-row run. The C++ reference never actually errored for it. Trusting the comment over the actual data would have wrongly excluded a perfectly good accuracy test case — it's included in the normal accuracy suite instead.

**Lesson for anyone adding a test vector later:** verify against the actual reference *output*, not just the TLE file's comment describing the test author's intent — the two aren't always the same thing, as 33335 shows.

### The one real discrepancy this suite found

Satellite 23599 (comment: "Lyddane bug at &gt; 280.5 min for AcTan()") diverges from the reference starting around t≈460–480 min, growing to ~0.96 km of position error by t=720 min — while all 31 other satellites, including several *other* Lyddane-choice stress cases, match to ~1e-4 km. `SGP4.cs`'s `dpper()` does have the documented Lyddane quadrant-correction branch (`Math.Atan2` with wraparound, not a naive `asin`/`atan`) — this isn't a case of the fix being obviously missing. The actual cause needs isolating a specific atan2/quadrant call in the deep-space periodic path for this satellite's near-singular geometry (inclination 6.93°, low eccentricity) — not yet done. This is tracked, not hidden: `Propagate_matches_known_bounded_divergence_for_satellite_23599` asserts the divergence stays within a bound comfortably above what's currently observed, so a regression that makes it *worse* still fails the suite, while the bound itself documents that this is a known, open issue.

**SGP4 status: 627/627 tests passing** (626 real assertions + one exact duplicate xUnit dedupes automatically — satellite 25954's reference file happens to print its t=0 state twice, once as a header row and once as part of its regular step range; not a bug).

## How it works today (Keplerian)

`KeplerianPropagatorTests.cs` — a different validation strategy than SGP4's, appropriately: there's no independently-published reference dataset for an idealized two-body orbit the way there is for SGP4, but the physics is simple enough to check by closed form and by conservation law instead. Three checks: an equatorial circular orbit's position matches the exact analytical formula; specific energy and specific angular momentum stay constant (to floating-point precision) across a full period for three geometries including a near-singular one; and propagating elements to a state vector and back through `OrbitalElementsCalculator` reproduces the original input. See `Propagation/Keplerian/README.md` for the full writeup — all of it currently passes, no known exceptions (unlike SGP4's one documented 23599 case above).

**Combined status: 640/640 tests passing** across both propagators.

## The process, step by step

1. **Source of truth for SGP4:** Vallado's published SGP4 test vectors (the standard reference set used across the aerospace software community) — done, see above.
2. **For each test TLE:** propagate with the implementation under test, compare the resulting position/velocity at each published time step against the reference output, within a stated tolerance — done (`PositionToleranceKm = 1e-4`, `VelocityToleranceKmPerSec = 1e-6`, both with real margin above the ~1e-7 km / 1e-9 km/s actually observed for every satellite except the documented 23599 exception).
3. **Cover both regimes:** near-Earth (SGP4) and deep-space, period > 225 minutes (SDP4) test cases — done, and asserted directly (`Golden_file_has_the_expected_shape` fails if a future edit to the test data drops coverage of either regime).
4. **For Keplerian:** done — see above. No published reference dataset exists for this one, so it's validated by closed-form + conservation laws instead of a golden file, which is the right substitute here (per this file's own earlier guidance): the physics genuinely is simple enough for those checks to be trustworthy on their own.
5. **Regression discipline:** this suite runs on every PR, not just when propagation code changes. Any future refactor (e.g., extracting propagation into `apps/sim-engine` at Product 4+, if that's ever justified) must pass this exact suite unchanged before the refactor is considered safe.

## What "adding a test vector" means in practice

Each test case needs: the TLE (two lines), the epoch/time steps to check, and the expected state vector at each — sourced from the reference material, not generated by the code under test (a test that checks code against its own output proves nothing). And per the 33335 lesson above: verify what the reference *output* actually contains, don't just trust a comment describing intent.

## What NOT to do

Don't hand-wave this suite as "good enough" with a handful of easy cases. This is the one piece of the entire ASPIS roadmap where under-investing has compounding cost — every later product's simulated data, AI training data, and replay correctness assumes this was done right.
