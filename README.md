# Apsis

A satellite orbit-tracking platform: a C++ SGP4 propagation engine, a .NET
web API, and an Angular frontend.

| App | What it is | Status |
|---|---|---|
| [`apps/engine`](apps/engine) | C++ SGP4 propagator. Loads satellite orbital data (TLE, 3LE, CSV, JSON, OMM KVN, OMM XML) and computes real-time position/velocity/lat-lon-altitude. | Working, tested, cross-platform |
| [`apps/server/api`](apps/server/api) | ASP.NET Core (.NET 10) Web API. | Default scaffold — not yet wired to the engine |
| [`apps/web`](apps/web) | Angular 22 SSR frontend. | Default scaffold + an in-progress 3D orbit view — not yet wired to the API |

`apps/db`, `apps/proto`, `apps/python`, `apps/bindings`, `apps/validation`,
`deploy/`, and `docs/` exist as empty placeholders for future work and
aren't covered below. The engine, API, and web app are independent stacks
right now (each runs and is tested on its own) — they aren't calling each
other yet.

## Prerequisites

You only need the toolchain for the app(s) you're working on, or just
Docker (see [Docker](#docker)) to skip local toolchains entirely.

| | macOS | Linux | Windows |
|---|---|---|---|
| **Engine** | Xcode Command Line Tools (`xcode-select --install`), [CMake](https://cmake.org/) ≥ 3.20, GNU Fortran runtime — see [note](#macos-gfortran-runtime) | `build-essential`, `cmake` | Visual Studio 2022 (Desktop C++ workload) or CMake + MSVC Build Tools |
| **API** | [.NET 10 SDK](https://dotnet.microsoft.com/download) | .NET 10 SDK | .NET 10 SDK |
| **Web** | Node.js ≥ 22, [pnpm](https://pnpm.io/) (`corepack enable` ships it) | Node.js ≥ 22, pnpm | Node.js ≥ 22, pnpm |
| **Docker path** | [Docker Desktop](https://www.docker.com/products/docker-desktop/) | Docker Engine | Docker Desktop (WSL2 backend) |

## Clone

```sh
git clone --recurse-submodules https://github.com/kumaraashish435/Apsis.git
cd Apsis
```

The engine vendors [Eigen](https://gitlab.com/libeigen/eigen) and
[GoogleTest](https://github.com/google/googletest) as git submodules — if
you cloned without `--recurse-submodules`, run:

```sh
git submodule update --init --recursive
```

---

## Engine (`apps/engine`)

A C++ library (`Propagate`) that parses satellite orbital element data in
six formats and propagates it with the vendored
[AstroStds/SGP4Prop](apps/engine/Vendor/AstroStds) library (the same SGP4
propagator used by STK) — plus a CLI (`App`) and a test suite (`Tests`,
GoogleTest) that validates the propagated output against known-good
reference values.

Both CMake and Premake build files are provided and kept in sync; use
whichever you prefer.

### macOS gfortran runtime

The vendored macOS libraries need GNU Fortran's runtime, which isn't part
of Xcode. One-time setup:

```sh
brew install gcc
```

(Skip this on Linux and Windows — those vendored libraries don't need it.)

### Build

CMake and Premake put binaries in **different layouts** — CMake outputs
flatly to `build/bin/`, Premake nests under
`build/bin/<system>/<config>/<project-name>/`. Pick one system and use its
matching Run/Test commands below; don't mix paths between them.

#### CMake

```sh
# from apps/engine/
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build

# from the repo root
make engine-cmake      # = cmake -S . -B build && cmake --build build
```

Binary: `build/bin/App` (flat, no OS/config subfolders), relative to
wherever you ran the `cmake` commands from.

#### Premake

```sh
# from apps/engine/ (macOS/Linux)
premake5 gmake2
make -C build/premake config=debug

# from the repo root
make engine             # = premake5 gmake2 && make -C apps/engine/build/premake config=debug
```

```powershell
# Premake (Windows) -- generates a Visual Studio solution
.\Vendor\Binaries\Premake\Windows\premake5.exe vs2022
```

Binary: `build/bin/<system>/<config>/App/App` — e.g. on macOS Debug that's
`build/bin/macosx/Debug/App/App` (Linux: `linux`, Windows:
`windows/.../App.exe`), relative to wherever you ran `premake5`/`make`
from. `<config>` is capitalized (`Debug`, `Release`, `Dist`) even though
`config=debug` on the command line is lowercase.

```sh
make engine-clean      # remove build/ (both systems, both apps/engine/build and root build/)
```

### Run

`SampleInputs/` is always under `apps/engine/`, regardless of where you
built from. Match the binary path to which system you built with, above:

```sh
# CMake, built from apps/engine/
cd apps/engine
./build/bin/App                          # built-in demo (ISS, CSV format)
./build/bin/App SampleInputs/iss.tle      # propagate a specific file

# CMake, built from the repo root (`make engine-cmake`)
./build/bin/App
./build/bin/App apps/engine/SampleInputs/iss.tle

# Premake, built from apps/engine/
cd apps/engine
./build/bin/macosx/Debug/App/App
./build/bin/macosx/Debug/App/App SampleInputs/iss.tle

# Premake, built from the repo root (`make engine`)
./apps/engine/build/bin/macosx/Debug/App/App
./apps/engine/build/bin/macosx/Debug/App/App apps/engine/SampleInputs/iss.tle
```

[`SampleInputs/`](apps/engine/SampleInputs) has one real, live-fetched
example per supported format — all describing the same ISS orbit, plus one
GEO satellite (GOES-16) and one sun-synchronous LEO satellite (Sentinel-1A)
for variety:

| File | Format |
|---|---|
| `iss.tle`, `iss.3le` | Classic 2-line / 3-line element set |
| `iss.csv` | Celestrak CSV |
| `iss.json`, `iss.json-pp.json` | Celestrak JSON (compact and pretty-printed) |
| `iss.kvn` | CCSDS OMM, key-value text |
| `iss.xml` | CCSDS OMM, XML |
| `goes16.tle` | Real GEO satellite (~35,786 km altitude) |
| `sentinel-1a.csv` | Real sun-synchronous LEO satellite |

The format is auto-detected — no flag needed. Every satellite loaded gets
propagated to the current time and printed with a `[PASS]`/`[FAIL]` tag (a
loose "is this physically plausible" sanity check — inside Earth's radius
and below ~45,000 km). Real validation against exact expected numbers is
what the test suite below does. (Example below uses the CMake path, built
from `apps/engine/`; substitute your own path from [Run](#run) above if
you used Premake or built from the repo root.)

```
$ ./build/bin/App SampleInputs/goes16.tle
Loaded 1 satellite(s) from SampleInputs/goes16.tle

[PASS] satKey=2269035922600418661  lat=-0.4619 deg  lon=255.2735 deg  alt=35789.9296 km  pos=(37788.4181, -18710.0430, -339.5641) km

1/1 satellite(s) propagated to a physically plausible position.
```

### Test

```sh
# CMake, from wherever you ran `cmake -S . -B build` (or repo root for `make engine-cmake`)
cmake --build build --target Tests   # if not already built
ctest --test-dir build --output-on-failure
# or run the binary directly:
./build/bin/Tests

# Premake, from apps/engine/
./build/bin/macosx/Debug/Tests/Tests

# Premake, from the repo root (`make engine`)
./apps/engine/build/bin/macosx/Debug/Tests/Tests
```

11 tests, all currently passing:
- **Format detection** for all 6 formats.
- **Garbage/empty input** handled gracefully (no crash).
- **One correctness test per format** (TLE, CSV, JSON, KVN, XML): each
  loads the same real ISS orbit, propagates to a fixed UTC time, and
  checks position (km), velocity (km/s), latitude/longitude/altitude
  against reference values generated by this same code — proving every
  parser feeds SGP4 identically, and that the whole pipeline (parsing,
  dynamic library loading, license lookup) genuinely works, not just that
  each format "loads without throwing."
- **Auto-detect** produces the same answer as an explicit format.
- **`PropagateToNow`** sanity-checked against a plausible LEO altitude
  range (can't hardcode an exact value for "now").

### Cross-platform notes

Vendored AstroStds libraries live under
[`Vendor/AstroStds/lib/`](apps/engine/Vendor/AstroStds/lib), one directory
per platform (`macos-arm64`, `linux-x64`, `linux-arm64`, `windows-x64`);
CMake/Premake auto-detect which one to use. They're loaded at runtime via
`dlopen`/`LoadLibrary`, not linked at compile time.

- **macOS / Linux**: verified — built and run for real on macOS (Apple
  Silicon) and inside Linux containers (both x64 and arm64, both CMake and
  Premake), all 11 tests passing on each.
- **Windows**: the libraries are vendored and the build files are wired up
  (DLLs get copied next to `App.exe`/`Tests.exe` after each build, since
  Windows has no rpath equivalent), but this hasn't been run on an actual
  Windows machine — treat it as best-effort until someone verifies it.
- Only Apple Silicon is vendored for macOS (not Intel). Add
  `Vendor/AstroStds/lib/macos-x64` from the original AstroStds download's
  `Lib/MacOS/x86/GFORTRAN` if you need it.

---

## API (`apps/server/api`)

Default ASP.NET Core Web API scaffold (still has the sample
`/weatherforecast` endpoint) — not yet connected to the engine or web app.

### Run

```sh
cd apps/server/api
dotnet run
```

```sh
curl http://localhost:5234/weatherforecast
```

In development mode (`ASPNETCORE_ENVIRONMENT=Development`, the default for
`dotnet run`), interactive API docs are at `http://localhost:5234/scalar/v1`.

---

## Web (`apps/web`)

Angular 22 (SSR) frontend, using [pnpm](https://pnpm.io/) as the package
manager (`pnpm-lock.yaml`/`pnpm-workspace.yaml` are the source of truth,
not the `npm` setting in `angular.json`). Includes a `Sidebar`,
`Navigation`, and route sections (`dashboard`, `analytics`, `settings`,
`orbit`) — `orbit` hosts an in-progress 3D `Orbitcanvas` component built on
[three.js](https://threejs.org/).

### Run

```sh
cd apps/web
pnpm install
pnpm start        # ng serve, http://localhost:4200
```

```sh
pnpm build         # production build -> dist/Apsis.client
pnpm test           # unit tests (vitest)
```

### Fixes applied to get this building

Two pre-existing gaps in the scaffold were blocking a clean build/run and
were fixed as part of setting this up:
- `three` was imported by `Orbitcanvas` but never added as a dependency —
  added `three` + `@types/three`.
- `angular.json`'s `security.allowedHosts` was `[]` (Angular's SSRF
  protection denying every `Host` header, including `localhost`) — added
  `"localhost"`. **Add your real domain here before deploying anywhere
  else** (this allowlist is a real security control, not boilerplate).

---

## Docker

Dockerfiles exist for all three apps — [`apps/engine/Dockerfile`](apps/engine/Dockerfile),
[`apps/server/api/Dockerfile`](apps/server/api/Dockerfile),
[`apps/web/Dockerfile`](apps/web/Dockerfile) — plus a
[`docker-compose.yml`](docker-compose.yml) at the repo root. All three were
built and run for real (not just written) as part of setting this up.

### Individually

```sh
# Engine -- the build itself runs ctest; a failing test fails the build.
docker build -t apsis-engine -f apps/engine/Dockerfile apps/engine
docker run --rm apsis-engine                        # built-in demo
docker run --rm apsis-engine SampleInputs/iss.json    # any sample file
docker run --rm -v "$PWD/mydata:/data" apsis-engine /data/mysat.csv

# API
docker build -t apsis-api -f apps/server/api/Dockerfile apps/server/api
docker run --rm -p 8080:8080 apsis-api
curl http://localhost:8080/weatherforecast

# Web
docker build -t apsis-web -f apps/web/Dockerfile apps/web
docker run --rm -p 4000:4000 apsis-web
open http://localhost:4000
```

### docker compose

```sh
docker compose up --build              # api (:8080) + web (:4000)
docker compose up --build api          # just the API
docker compose up --build web          # just the web frontend
```

The engine is a one-shot CLI tool, not a server, so it's excluded from the
default `up` (it's behind the `tools` profile) — run it directly instead:

```sh
docker compose run --rm engine                          # built-in demo
docker compose run --rm engine SampleInputs/iss.json      # any sample file
docker compose build engine                                # also runs the test suite
```

### Cleanup

`docker run --rm` / `docker compose run --rm` already remove containers on
exit, so there's normally nothing to do. Images and build cache stick
around (harmless, just disk space) — remove them if you want a clean
slate:

```sh
docker rmi apsis-engine apsis-api apsis-web   # remove the built images
docker builder prune                           # clear dangling build cache
docker compose down                            # stop/remove compose-managed containers
```

---

## Project structure

```
Apsis/
├── apps/
│   ├── engine/            C++ SGP4 propagation engine (working, tested)
│   │   ├── Core/           shared utility static lib
│   │   ├── Propagate/      GP data loader + SGP4 propagation wrapper (static lib)
│   │   ├── App/             CLI entry point
│   │   ├── Tests/           GoogleTest suite
│   │   ├── SampleInputs/    real sample data, one file per supported format
│   │   ├── Vendor/AstroStds/  vendored SGP4Prop libraries (per platform)
│   │   └── Dockerfile
│   ├── server/api/         ASP.NET Core Web API (scaffold)
│   │   └── Dockerfile
│   ├── web/                 Angular SSR frontend (scaffold + orbit view WIP)
│   │   └── Dockerfile
│   ├── db/, proto/, python/, bindings/, validation/   empty -- future work
├── deploy/, docs/            empty -- future work
├── docker-compose.yml
├── CMakeLists.txt, premake5.lua, Makefile   root wrappers for apps/engine
└── README.md                  this file
```

## License

`apps/engine`'s own code is under [UNLICENSE](apps/engine/UNLICENSE.txt).
The vendored AstroStds/SGP4Prop libraries are separately licensed —
publicly releasable/redistributable, see
[`Vendor/AstroStds/SGP4_Open_License.txt`](apps/engine/Vendor/AstroStds/SGP4_Open_License.txt).
