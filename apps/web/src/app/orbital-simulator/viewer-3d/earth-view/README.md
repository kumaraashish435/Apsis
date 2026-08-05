# earth-view — the 3D orbital viewer

Product 1's working 3D scene: a Three.js Earth (textures, atmosphere, clouds, starfield,
camera + damped orbit controls) with real propagated satellite tracks rendered on top,
wrapped in an Angular component. It's what renders at
`/simulator/simulations/:id/view` (see `viewer-3d.ts`, one level up, and
`../ground-track/` for this same page's 2D alternative view).

Every file below is small and does one job — that's deliberate, not accidental
fragmentation: `earth-view.ts` is the only file that knows about all of them, so any one
module (say, swapping the camera's auto-fit math, or adding a new light) can be understood
and changed without reading the rest.

## How it fits together

```
earth-view.ts  (orchestrator — the only file that imports from every folder below)
 ├─ scene/scene.ts          → THREE.Scene + starfield (Moon helpers exist, unused — see below)
 ├─ camera/camera.ts        → PerspectiveCamera + fitCameraToBoundingSphere()
 ├─ camera/orbit-controls.ts→ OrbitControls (drag-to-orbit, scroll-to-zoom)
 ├─ renderer/renderer.ts    → WebGLRenderer, sized to the container
 ├─ renderer/resize.ts      → ResizeObserver → keeps camera/renderer in sync with the div
 ├─ renderer/animation-loop.ts → owns requestAnimationFrame, hands onFrame() a delta
 ├─ earth/earth.ts          → the textured Earth sphere
 ├─ earth/atmosphere.ts     → translucent glow shell
 ├─ earth/clouds.ts         → optional alpha-mapped cloud layer
 ├─ earth/lights.ts         → ambient + directional "sun" light
 ├─ earth/textures.ts       → texture loading (color-space handling, missing-file fallback)
 ├─ satellite/*             → Orbit (state-vector interpolation), Satellite (mesh + line),
 │                            SatelliteManager (add/remove/update many satellites)
 ├─ simulation/*             → sim clock: real seconds → scaled "mission" seconds, pause/resume
 ├─ input/*                  → mouse/keyboard/raycaster building blocks — not wired into
 │                            earth-view.ts yet, see "What's not built" below
 └─ utils/*                  → shared constants, km↔scene-unit conversion, disposal, demo data
```

`earth-view.ts` is a plain Angular component (`AfterViewInit`/`OnDestroy`) — Three.js
itself doesn't know Angular exists; the component is just the glue that creates the scene
once, drives it every frame, and tears it down when the route changes.

## File-by-file: what and why

### Orchestration

**`earth-view.ts`** — Creates every module above once in `ngAfterViewInit`, wires them
together, and owns the per-frame update (`onFrame`). Takes real satellite data via an
`input()` signal (`satellites: SatelliteEntry[]`) — `viewer-3d.ts` fetches
`GET /api/v1/simulations/{id}/propagate` and passes the result straight in; this component
has no knowledge of the API, HTTP, or routing, it just renders whatever track it's handed.
Three things worth understanding about *why* it's built the way it is:

- **The render loop runs outside Angular's zone.** `requestAnimationFrame` fires ~60 times a
  second; if that ran inside Angular's zone, every frame would trigger a full
  change-detection pass for the whole component tree, for no reason (nothing template-bound
  changes on every single frame). `zone.runOutsideAngular(...)` wraps only the
  `AnimationLoop.start()` call, so Three.js renders freely without Angular watching.
- **Earth's tilt and spin are two separate nested `THREE.Group`s**, not one group's
  Euler rotation. `earthTilt` is set once (axial tilt, 23.44°) and never touched again;
  `earthSpin` is its *child* and gets `.rotation.y` updated every frame. Because a child's
  local Y axis is inherited from its already-tilted parent, spinning it can only ever
  rotate around the tilted axis — mixing a constant tilt and a per-frame spin into one
  group's Euler `x`/`y`/`z` components is rotation-order-fragile and was previously
  producing a level-looking spin instead of a tilted one (see git history).
- **Satellites are parented to `earthTilt`, not `earthSpin`.** Their state-vector positions
  are Earth-centered-inertial (ECI/TEME) — they must inherit Earth's constant axial tilt
  (so the orbital plane is oriented correctly relative to the equator) but must **not**
  inherit Earth's per-frame rotation, or a satellite's rendered position would incorrectly
  spin along with the ground beneath it.
- **The camera auto-fits to whatever's tracked, forcefully, every time the satellite set
  changes.** `applySatellites()` computes the scene-unit distance of every point in every
  track and calls `fitCameraToBoundingSphere()` with `forceFit=true` (the default — see
  `camera/camera.ts`'s doc comment for why a "just clamp" mode wouldn't be enough here):
  the default camera position is tuned for a near-Earth view, so loading a GEO-altitude
  simulation without forcing a reposition would leave the satellite far outside the frame.

### scene/

**`scene.ts`** — Creates the `THREE.Scene` and a starfield (`THREE.Points`, random
directions on a large sphere — cheap and good enough, not a real star catalog). Also
exports `createMoon()`/`updateMoon()`, a simplified circular Moon — built but **not** called
from `earth-view.ts`. Excluded on purpose: at a true-to-scale distance (~60 scene units) it
would force the camera's auto-fit distance so far out that near-Earth satellite orbits
become indistinguishable from Earth itself. Worth adding once the viewer supports toggling
"what's currently tracked" rather than always fitting everything at once.

### camera/

**`camera.ts`** — `createCamera()` sets a sane default FOV/near/far and starting position.
`fitCameraToBoundingSphere()` is the auto-fit: given a bounding sphere of everything
currently tracked, it sets `controls.minDistance`/`maxDistance` so the user can never zoom
out past "everything fits" or in past "you're inside the Earth mesh," and (with
`forceFit=true`, the default) repositions the camera to frame that sphere immediately
rather than only when the camera happens to already be out of bounds. `minDistanceFloor` is
a separate parameter from the sphere's own radius — a satellite's orbit (a GEO satellite
sits ~6.6 Earth radii out) inflates the bounding sphere far past Earth's actual size, so
deriving the near-zoom limit from that sphere would let the camera clip straight through
the planet. `earth-view.ts` passes `SCENE_EARTH_RADIUS * 1.3` explicitly, keeping the near
limit anchored to Earth's real size regardless of how wide the tracked orbit is.

**`orbit-controls.ts`** — Wraps `OrbitControls` with damping enabled and panning disabled
(this is an "orbit a planet" viewer, not a free-fly camera). Its hardcoded
`minDistance`/`maxDistance` are just the *pre-auto-fit* fallback, immediately overwritten by
`fitCameraToBoundingSphere()` the first time satellites load.

### renderer/

**`renderer.ts`** — Creates the `WebGLRenderer`, sizes it to the container, appends its
canvas to the DOM. Deliberately minimal — no `setPixelRatio` call, so it renders at CSS
pixel resolution rather than device pixel resolution; a reasonable perf/sharpness
trade-off for a scene where you're rarely reading fine texture detail.

**`resize.ts`** — A `ResizeObserver` on the container div (not `window`) — correct even if
the canvas is resized by something other than the browser window. Updates the camera's
aspect ratio and the renderer's size, then calls an optional callback (`earth-view.ts` uses
it to re-run `controls.update()`).

**`animation-loop.ts`** — Owns the `requestAnimationFrame` bookkeeping (frame id,
start/stop, delta-time calculation) so `earth-view.ts` doesn't hand-roll it. `stop()` is
what `ngOnDestroy` calls — without it, navigating away from the viewer would leave the loop
running forever, rendering into a canvas nobody can see and leaking a WebGL context.

### earth/

**`earth.ts`** — The Earth mesh: a `SphereGeometry` at `SCENE_EARTH_RADIUS` (1 scene unit,
by convention — see `utils/constants.ts`) with the day-texture map.

**`atmosphere.ts`** — A slightly larger sphere, back-face rendered with additive blending
and low opacity — a standard cheap "atmosphere glow" trick, not a physically based
scattering model.

**`clouds.ts`** — An alpha-mapped sphere just outside the Earth mesh. Starts `visible =
false` and only flips to `true` once `public/textures/earth_clouds.png` actually loads —
so a missing texture file degrades to "no clouds" instead of a broken opaque sphere.

**`lights.ts`** — Ambient light (so the night side isn't pure black) plus one directional
"sun" light.

**`textures.ts`** — `loadColorTexture()` sets `colorSpace = THREE.SRGBColorSpace`
explicitly (Three.js 0.150+ requires this or color maps render washed out) and logs load
errors. `loadDataTexture()` is for non-color maps (like the cloud alpha map) — no
color-space conversion, and it fails *silently*, because that texture is allowed to not
exist yet.

### satellite/

**`orbit.ts`** — `SatelliteStateVector` (`epochSeconds`/`xKm`/`yKm`/`zKm`) is deliberately
shaped to match the *position* subset of the backend's `StateVectorResponse` — see
`../viewer-3d.ts`'s `toSceneStates()`, which strips velocity/lat/lon before handing data in
here, since this component only needs position. `Orbit` converts a state-vector series into
scene-unit points and linearly interpolates position between the two nearest samples for
any given simulated time, wrapping around the orbit's own period.

**`orbit-renderer.ts`** — Builds the visible orbit-path line from an `Orbit`'s points.

**`satellite.ts`** — One satellite: an orbit line + a moving marker mesh. `update(simSeconds)`
repositions the marker each frame.

**`satellite-manager.ts`** — `SatelliteEntry` (`{ name, states }`) is the shape
`earth-view.ts`'s `satellites` input expects. `addSatellite()`/`removeSatellite()`/`dispose()`
let `earth-view.ts` swap the whole tracked set (e.g. when the user navigates to a different
simulation) without touching individual `Satellite` instances directly.

### simulation/

**`clock.ts`** — Tracks elapsed *simulated* seconds, independent of wall-clock time;
supports pause/resume (not currently exposed in the UI — the clock always runs).

**`time.ts`** — `toSimSeconds()` scales real elapsed seconds by a `timeScale` multiplier
(`earth-view.ts` uses 600: one real second plays back ten simulated minutes). Also exports
`formatMissionTime()` (`HH:MM:SS` formatting) — written for a future on-screen mission-time
readout, not currently called from anywhere.

**`simulation.ts`** — Combines the two: real delta-time in, scaled sim-time out. Both
Earth's rotation and every satellite's position are driven from this single clock, not
from wall-clock time directly — pausing or changing speed (if exposed later) would move
everything in the scene together, consistently.

### input/

**`mouse.ts`**, **`keyboard.ts`**, **`raycaster.ts`**, **`input-manager.ts`** — Building
blocks for pointer tracking, a key→handler registry, and object picking. **Not currently
wired into `earth-view.ts`** — the only picking that happens today is `OrbitControlsGate`'s
own internal raycast (camera/orbit-controls-gate.ts, gating drag/zoom to "pointer is over
Earth or a satellite" so scrolling over empty space doesn't zoom the camera). Click-to-select
a specific satellite for a detail readout would build on these files, but that's not built.

### utils/

**`constants.ts`** — Every shared number in one place: Earth's real radius in km, the
scene-unit convention (`SCENE_EARTH_RADIUS = 1`, so `KM_TO_SCENE` converts real km straight
to scene units), axial tilt, sidereal day length, Moon constants (unused, kept for when the
Moon is wired in), and the texture base path.

**`math.ts`** — `kmToScene()` (the one place km→scene-unit conversion actually happens) and
a generic `clamp()`.

**`helpers.ts`** — `disposeObject3D()` walks an object tree and disposes every
geometry/material/texture it finds — called from `ngOnDestroy` so navigating away from the
viewer doesn't leak GPU memory. `generateDemoCircularOrbit()` is marked **DEMO DATA ONLY**
in its own doc comment — it was used to exercise this rendering pipeline before the
backend's propagator existed. It's no longer called from anywhere now that
`GET /api/v1/simulations/{id}/propagate` is real (see `../viewer-3d.ts`); kept in place as
a documented, honestly-labeled artifact rather than deleted, in case a future demo/storybook
context wants a satellite with no backend call.

## What's real vs. not built yet

- **Earth, camera + auto-fit, controls, lighting, atmosphere/clouds, and the whole
  rendering/satellite-track pipeline are real**, driven by actual propagated data from
  `Modules/Simulations` — not fabricated.
- **No HUD overlay, no click-to-select-a-satellite detail panel, no play/pause/time-scale
  controls in the UI.** The orbital-parameters readout the viewer page shows
  (`../viewer-3d.html`) is a separate Angular template reading the API's response directly —
  it doesn't come from anything in this folder. The `input/` building blocks above exist for
  a future click-to-select feature but aren't connected to anything yet.
- **The Moon exists but isn't rendered** — `scene.ts` has working `createMoon()`/
  `updateMoon()`, just not called (see `scene/`'s section above for why).

## Routing / where this lives in the app

`app.routes.ts` → `simulator/simulations/:id/view` (a top-level route, deliberately *not*
nested under `Shell` — see `app.routes.ts`'s comment for why) → `viewer-3d.ts`, which fetches
the simulation + its propagated states, then renders `<app-earth-view [satellites]="...">`
alongside the 2D `<app-ground-track>` alternative and the orbital-parameters readout panel.
