# apps/engine: SGP4 propagation engine (C++/CMake).
#
# Build stage compiles the engine AND runs the test suite (ctest) as part
# of the image build -- if the tests fail, `docker build` fails, so a
# built image is by construction one whose propagation output has been
# validated against known-good reference values (see
# Tests/Source/PropagateTests.cpp).
#
# Runtime stage only carries the built App binary, the vendored AstroStds
# libraries for whatever platform this was built on (auto-detected by
# CMake), and the sample input data -- no compiler toolchain.
#
# Build:  docker build -t apsis-engine -f apps/engine/Dockerfile apps/engine
# Run:    docker run --rm apsis-engine                       # built-in demo
#         docker run --rm apsis-engine SampleInputs/iss.json  # any sample file
#         docker run --rm -v "$PWD/mydata:/data" apsis-engine /data/mysat.csv

FROM ubuntu:22.04 AS build

RUN apt-get update \
    && apt-get install -y --no-install-recommends cmake g++ make ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /engine
COPY . .

RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DENGINE_BUILD_TESTS=ON \
    && cmake --build build --config Release -j \
    && ctest --test-dir build --output-on-failure


FROM debian:bookworm-slim AS runtime

# Keep runtime small: only install certs and C++ runtime libraries required by App.
RUN apt-get update \
    && apt-get install -y --no-install-recommends ca-certificates libstdc++6 libgcc-s1 \
    && rm -rf /var/lib/apt/lists/*

# Same absolute path as the build stage: the App binary has its AstroStds
# library directory baked in as an rpath at that exact path (see
# Propagate/CMakeLists.txt). That rpath covers direct link-time
# dependencies, but glibc's dlopen() (which is how the AstroStds wrappers
# actually load libdllmain.so etc at runtime) does not reliably honor it
# for an executable-only rpath across a fresh process in a separate image
# layer -- LD_LIBRARY_PATH is what's actually authoritative here. Listing
# both arch subdirectories is harmless: only the one matching this image's
# actual architecture contains loadable libraries, the other is just
# skipped by the linker.
WORKDIR /engine

COPY --from=build /engine/build/bin/App ./build/bin/App
COPY --from=build /engine/Vendor/AstroStds ./Vendor/AstroStds
COPY --from=build /engine/SampleInputs ./SampleInputs

ENV LD_LIBRARY_PATH=/engine/Vendor/AstroStds/lib/linux-x64:/engine/Vendor/AstroStds/lib/linux-arm64

ENTRYPOINT ["./build/bin/App"]
CMD []
