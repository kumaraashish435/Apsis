cmake_minimum_required(VERSION 3.25)
project(Apsis VERSION 0.1.0
    DESCRIPTION "Orbital simulation and astrodynamics platform"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


option(ORBITCORE_BUILD_TESTS
    "Build OrbitCore tests"
    ON
)

add_subdirectory(engine)