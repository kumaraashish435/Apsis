# earth-view — the 3D orbital viewer

This is Product 1's working 3D simulation: a Three.js scene (Earth, atmosphere, clouds,
starfield, camera + controls) with satellites orbiting on top, wrapped in an Angular
component with a HUD overlay (satellite list, selection detail, play/pause, time-scale).
It's what renders at `/simulator/simulations/:id/view` (see `viewer-3d/`, one level up).

Every file below is small and does one job — that's deliberate, not accidental
fragmentation: `earth-view.ts` is the only file that knows about all of them, so any one
module (say, swapping the camera's auto-fit math, or adding a new light) can be understood
and changed without reading the rest.

## How it fits together

```
earth-view.ts  (orchestrator — the only file that imports from every folder below)
 ├─ scene/scene.ts          → THREE.Scene + starfield (Moon helpers exist, unused for now)
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
 │                            SatelliteManager (add/remove/update/select many satellites)
 ├─ simulation/*             → sim clock: real seconds → scaled "mission" seconds, pause/resume
 ├─ input/*                  → mouse position, keyboard, click-to-pick raycasting
 └─ utils/*                  → shared constants, km↔scene-unit conversion, disposal, demo data
```

`earth-view.html`/`.scss` add the HUD overlay on top of the canvas. `earth-view.ts` is a
plain Angular component (`AfterViewInit`/`OnDestroy`) — Three.js itself doesn't know
Angular exists; the component is just the glue that creates the scene once, drives it every
frame, and tears it down when the route changes.

## File-by-file: what and why

### Orchestration

**`earth-view.ts`** — Creates every module above once in `ngAfterViewInit`, wires them
together, and owns the per-frame update (`onFrame`) and the HUD's reactive state (Angular
`signal`s). Three things worth understanding about *why* it's built the way it is:

- **The render loop runs outside Angular's zone.** `requestAnimationFrame` fires ~60 times a
  second; if that ran inside Angular's zone, every frame would trigger a full
  change-detection pass for the whole component tree, for no reason (nothing template-bound
  changes on every single frame). `zone.runOutsideAngular(...)` wraps only the
  `AnimationLoop.start()` call, so Three.js renders freely without Angular watching.
- **The HUD is refreshed on a timer, not every frame.** Reading `simSeconds` and updating
  signals 60×/sec would just re-introduce the same change-detection cost the zone trick
  avoids. `onFrame` accumulates real time and only calls `zone.run(() => this.refreshHud(...))`
  every `HUD_REFRESH_INTERVAL_SECONDS` (0.2s = 5×/sec) — smooth enough for a readable
  altitude/mission-time display, cheap enough to not matter.
- **Earth's tilt and spin are two separate nested `THREE.Group`s**, not one group's
  Euler rotation. `earthTilt` is set once (axial tilt, 23.44°) and never touched again;
  `earthSpin` is its *child* and gets `.rotation.y` updated every frame. Because a child's
  local Y axis is inherited from its already-tilted parent, spinning it can only ever
  rotate around the tilted axis — this was a real bug fix (see git history): mixing tilt
  and spin into one Euler's `x`/`y`/`z` components is rotation-order-fragile and produced a
  level-looking spin instead of a tilted one.

Satellites are parented directly to `scene`, **not** to `earthTilt`/`earthSpin` — their
state-vector positions are Earth-centered-inertial, so they must not inherit Earth's own
spin animation the way the atmosphere/clouds correctly do.

### scene/

**`scene.ts`** — Creates the `THREE.Scene` and a starfield (`THREE.Points`, random
directions on a large sphere — cheap and good enough, not a real star catalog). Also
exports `createMoon()`/`updateMoon()`, a simplified circular Moon — built but **not** wired
into `earth-view.ts` yet. It's excluded on purpose: at a true-to-scale distance (~60 scene
units) it would force the camera's auto-fit distance so far out that near-Earth satellite
orbits become indistinguishable from Earth itself. Worth adding once the viewer supports
toggling "what's currently tracked" rather than always fitting everything at once.

### camera/

**`camera.ts`** — `createCamera()` sets a sane default FOV/near/far and starting position.
`fitCameraToBoundingSphere()` is the auto-fit: given a bounding sphere of everything
currently in the scene worth looking at, it sets `controls.minDistance`/`maxDistance` so the
user can never zoom out past "everything fits" or in past "you're inside the Earth mesh."
`minDistanceFloor` is a separate parameter from the sphere's own radius — a satellite's
orbit (especially the GEO demo satellite, ~6.6 Earth radii out) inflates the bounding
sphere far past Earth's actual size, so deriving the near-zoom limit from that sphere would
let the camera clip straight through the planet. Passing `SCENE_EARTH_RADIUS * 1.05`
explicitly keeps the near limit anchored to Earth's real size regardless of how wide the
tracked orbits are.

**`orbit-controls.ts`** — Wraps `OrbitControls` with damping enabled and panning disabled
(this is an "orbit a planet" viewer, not a free-fly camera). Its hardcoded
`minDistance`/`maxDistance` are just the *pre-auto-fit* fallback, immediately overwritten
by `fitCameraToBoundingSphere()` once `earth-view.ts` calls `refitCamera()` after satellites
are added.

### renderer/

**`renderer.ts`** — Creates the `WebGLRenderer`, sizes it to the container, appends its
canvas to the DOM. Deliberately minimal — no `setPixelRatio` call, so it renders at CSS
pixel resolution rather than device pixel resolution; a reasonable perf/sharpness
trade-off for a scene where you're rarely reading fine texture detail.

**`resize.ts`** — A `ResizeObserver` on the container div (not `window`) — correct even if
the canvas is resized by something other than the browser window (e.g. a future layout
change that shrinks the viewer panel without the window itself resizing). Updates the
camera's aspect ratio and the renderer's size, then calls an optional callback
(`earth-view.ts` uses it to re-run `controls.update()`).

**`animation-loop.ts`** — Owns the `requestAnimationFrame` bookkeeping (frame id,
start/stop, delta-time calculation) so `earth-view.ts` doesn't hand-roll it. `stop()` is
what `ngOnDestroy` calls — without it, navigating away from the viewer would leave the loop
running forever, rendering into a canvas nobody can see and leaking a WebGL context.

### earth/

**`earth.ts`** — The Earth mesh: a `SphereGeometry` at `SCENE_EARTH_RADIUS` (1 scene unit,
by convention — see `utils/constants.ts`) with the day-texture map.

**`atmosphere.ts`** — A slightly larger sphere, back-face rendered with additive blending
and low opacity — a standard cheap "atmosphere glow" trick, not a physically based
scattering model. Added to `earthTilt` (not `earthSpin`) since a symmetric translucent
shell looks identical whether or not it's spinning — no reason to pay for the extra
transform.

**`clouds.ts`** — An alpha-mapped sphere just outside the Earth mesh. It starts `visible =
false` and only flips to `true` once `public/textures/earth_clouds.png` actually loads —
so a missing texture file degrades gracefully to "no clouds" instead of showing a broken
opaque sphere covering the whole planet. (No cloud texture is checked into the repo yet;
drop one at that path and it'll appear automatically, no code change needed.)

**`lights.ts`** — Ambient light (so the night side isn't pure black) plus one directional
"sun" light.

**`textures.ts`** — Two loader functions: `loadColorTexture()` sets `colorSpace =
THREE.SRGBColorSpace` explicitly (Three.js 0.150+ requires this or color maps render
washed out) and logs load errors. `loadDataTexture()` is for non-color maps (like the cloud
alpha map) — no color-space conversion, and it fails *silently*, because that texture is
allowed to not exist yet.

### satellite/

**`orbit.ts`** — `SatelliteStateVector` is deliberately shaped to match the backend's
`StateVector` record (`apps/api/.../Propagation/Sgp4/ISgp4Propagator.cs`) so swapping demo
data for a real backend response requires no reshaping. `Orbit` converts a state-vector
series into scene-unit points and linearly interpolates position between the two nearest
samples for any given simulated time, wrapping around the orbit's own period. Good enough
for visualization at typical sample spacing; not a substitute for re-querying the backend
if a use case ever needs sub-sample precision client-side.

**`orbit-renderer.ts`** — Builds the visible orbit-path line from an `Orbit`'s points.

**`satellite.ts`** — One satellite: an orbit line + a moving marker mesh. `update(simSeconds)`
repositions the marker each frame. `setSelected()` is the click-to-inspect visual
feedback — scales the marker up and brightens both the marker and its orbit line; the HUD
and the 3D click-pick both call it through `SatelliteManager`, so "selected" always means
the same thing regardless of which UI element triggered it. `altitudeKmAt()` converts the
current scene-unit position back to a km altitude above Earth's surface — used by the HUD's
live altitude readout.

**`satellite-manager.ts`** — Add/remove/update/select many satellites without
`earth-view.ts` touching individual `Satellite` instances directly. `addSatellite()` is
where real propagated data plugs in later: once `GET /api/v1/simulations/{id}/propagate`
returns real `StateVector[]` arrays, calling this with that data instead of
`generateDemoCircularOrbit()`'s output is the entire integration — nothing downstream
(rendering, HUD, camera fit) needs to change, because the shape is identical by design.

### simulation/

**`clock.ts`** — Tracks elapsed *simulated* seconds, independent of wall-clock time;
supports pause/resume.

**`time.ts`** — `toSimSeconds()` scales real elapsed seconds by a `timeScale` multiplier
(e.g. `timeScale=600` means one real second plays back ten simulated minutes — that's why a
GEO satellite with an ~24h real period visibly completes an orbit in under two and a half
real minutes). `formatMissionTime()` renders elapsed seconds as `HH:MM:SS` for the HUD.

**`simulation.ts`** — Combines the two: real delta-time in, scaled sim-time out. Both
Earth's rotation and every satellite's position are driven from this single clock, not
from wall-clock time directly — pausing or changing speed moves everything in the scene
together, consistently.

### input/

**`mouse.ts`** — Tracks pointer position in normalized device coordinates (needed by
Three.js's raycaster).

**`keyboard.ts`** — A simple key → handler registry. `earth-view.ts` binds the space bar to
pause/resume.

**`raycaster.ts`** — `pickObject()`: given the camera, pointer NDC, and a list of
candidate objects, returns whichever one the ray actually hits (or `null`).

**`input-manager.ts`** — Wires mouse + keyboard + raycaster together and exposes an
`onObjectClick` event. Picking is intentionally lazy — `pickableObjects()` is a callback
invoked at click time, not a cached list, so it always sees the current set of satellite
groups even as they're added or removed later.

`earth-view.ts` interprets a click hit by checking the object's `name` for the
`satellite-marker:<id>` prefix set in `satellite.ts` — clicking an orbit *line* (which has
no name) or empty space both resolve to "no satellite," which deselects whatever was
selected. Clicking the *same* satellite twice also deselects it (see `selectSatellite()`).

### utils/

**`constants.ts`** — Every shared number in one place: Earth's real radius in km, the
scene-unit convention (`SCENE_EARTH_RADIUS = 1`, so `KM_TO_SCENE` converts real km straight
to scene units), axial tilt, sidereal day length, Moon constants (currently unused, kept
for when the Moon is wired in), and the texture base path.

**`math.ts`** — `kmToScene()` (the one place km→scene-unit conversion actually happens) and
a generic `clamp()`.

**`helpers.ts`** — `disposeObject3D()` walks an object tree and disposes every
geometry/material/texture it finds — called from `ngOnDestroy` so navigating away from the
viewer doesn't leak GPU memory. `generateDemoCircularOrbit()` is explicitly marked **DEMO
DATA ONLY** in its own doc comment: it fabricates a simple circular-orbit state-vector
series so the whole pipeline (SatelliteManager → Orbit → rendered line + moving marker →
camera auto-fit → HUD altitude readout) can be seen working end-to-end before the backend's
SGP4 propagator is implemented. `earth-view.ts` currently seeds three satellites this way
(`DEMO_SATELLITES`: a LEO, MEO, and GEO example) purely to have something realistic-looking
to look at.

## What's demo vs. real right now

- **Earth, camera, controls, lighting, atmosphere/clouds, the HUD, click-to-select, and the
  whole rendering pipeline are real** — nothing about them changes once real orbital data
  is available.
- **The three satellites' orbits are fabricated** (`generateDemoCircularOrbit`), not SGP4
  output. The backend's propagator (`apps/api/Apsis.Api/Propagation/Sgp4/`) is being built
  separately; once `ISgp4Propagator`/`Sgp4Propagator` and the
  `GET /api/v1/simulations/{id}/propagate` endpoint exist, replacing the
  `DEMO_SATELLITES` block in `earth-view.ts`'s `seedDemoSatellites()` with a real HTTP call
  is the only change needed — `SatelliteManager.addSatellite()` already accepts exactly the
  shape a real backend response would produce.
- **The Moon exists but isn't rendered** — `scene.ts` has working `createMoon()`/
  `updateMoon()` functions, just not called from `earth-view.ts` yet (see the note in
  `scene/`'s section above for why).

## Routing / where this lives in the app

`app.routes.ts` → `Shell` (top nav) → `orbital-simulator.routes.ts` →
`/simulator/simulations/:id/view` → `viewer-3d/viewer-3d.ts`, which just renders
`<app-earth-view>` inside a page header. The dashboard page
(`orbital-simulator/dashboard/`) has a demo link straight to
`/simulator/simulations/demo/view` — the `:id` isn't read by anything yet (demo data is
hardcoded regardless of id), so any id value works; it exists so there's a real,
click-through path to the viewer through normal navigation rather than a hardcoded
temporary route.
