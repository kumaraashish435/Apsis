# Root convenience wrapper for the C++ engine (apps/engine). Other apps/
# directories are separate stacks (web, api, ...) with their own tooling
# and aren't covered here.
#
# Usage:
#   make engine              build engine with Premake (CONFIG=debug|release|dist)
#   make engine-cmake        build engine with CMake (CMAKE_CONFIG=Debug|Release|Dist)
#   make engine-clean        remove apps/engine/build (and root build/, if used)

CONFIG ?= debug
CMAKE_CONFIG ?= Debug

.PHONY: engine engine-cmake engine-clean

engine:
	premake5 gmake2
	$(MAKE) -C apps/engine/build/premake config=$(CONFIG)

engine-cmake:
	cmake -S . -B build -DCMAKE_BUILD_TYPE=$(CMAKE_CONFIG)
	cmake --build build

engine-clean:
	rm -rf build apps/engine/build
