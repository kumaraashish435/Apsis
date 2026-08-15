# C++ Project Starter Template

This is a little quick-start project template for C++ projects which utilise a Core/App project architecture. There are two included projects - one called _Core_, and one called _App_. Both [CMake](https://cmake.org/) and [Premake](https://github.com/premake/premake-core) build files are provided, kept in sync — use whichever you prefer.

Core builds into a static library and is meant to contain common code intended for use in multiple applications. App builds into an executable and links the Core static library, as well as provides an include path to Core's code.

All generated files (project files, object files, binaries) are written to `build/`, which is gitignored.

## Building with CMake

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/bin/App
```

Configurations `Debug`, `Release`, and `Dist` are supported via `-DCMAKE_BUILD_TYPE=<config>`, matching the Premake configurations below.

## Building with Premake

The `Vendor/` directory contains Premake binaries for Windows/macOS/Linux (currently version `5.0-beta2`); use those or a `premake5` from your PATH.

By default, generate GNU makefiles and build from the terminal — this works the same way on **macOS and Linux**, and always builds for the host's native architecture (e.g. arm64 on Apple Silicon, without forcing x86_64):

```sh
premake5 gmake2
make -C build/premake config=debug   # config=debug|release|dist
./build/bin/macosx/Debug/App/App     # macosx on Mac, linux on Linux
```

To generate an IDE project instead, pass a different Premake action:

```sh
premake5 vs2022   # Visual Studio 2022 solution (Windows) -> build/premake/*.sln
premake5 xcode4   # Xcode project (macOS)                -> build/premake/*.xcworkspace
```

See `premake5 --help` for the full list of supported actions.

## Cleaning

`make clean` (from here) or `make engine-clean` (from the repo root) removes `build/` entirely — this is the reliable way to reset either build system. CMake's own `cmake --build build --target clean` and Premake's `make -C build/premake clean` also work, but only clear the object/binary files for the currently configured build type, not generated project files.

## Building from the repo root

Both build systems can also be driven from the Apsis repo root, via the root `CMakeLists.txt` / `premake5.lua` / `Makefile` — these just wire into this directory (`apps/engine` remains the single source of truth for the actual build config). Output still lands in `apps/engine/build/`.

```sh
# from the repo root
make engine                       # Premake, config=debug|release|dist
make engine-cmake                 # CMake, CMAKE_CONFIG=Debug|Release|Dist
make engine-clean                 # remove apps/engine/build
```

## Getting Started
1. Clone this repository or use the "Use this template" button on GitHub to quickly set up your own repository based on this template
2. `App/` and `Core/` are the two projects - you can edit the names of these folders and their contents to suit
3. Update both build definitions when changing sources/configuration: the Premake files (`premake5.lua`, `Core/Build-Core.lua`, `App/Build-App.lua`) and the CMake files (`CMakeLists.txt`, `Core/CMakeLists.txt`, `App/CMakeLists.txt`)

## Included
- Some example code (in `App/Source` and `Core/Source`) to provide a starting point and test
- Simple `.gitignore` to ignore project files and binaries
- Premake binaries for Win/Mac/Linux (`v5.0-beta2`)

## License
- UNLICENSE for this repository (see `UNLICENSE.txt` for more details)
- Premake is licensed under BSD 3-Clause (see included LICENSE.txt file for more details)