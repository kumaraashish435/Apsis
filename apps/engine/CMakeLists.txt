cmake_minimum_required(VERSION 3.20)
project(Engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
   set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
endif()

# Match Premake's Debug/Release/Dist configuration names for multi-config generators
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Dist" CACHE STRING "" FORCE)

# Keep output next to Premake's, all under build/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

option(ENGINE_BUILD_TESTS "Build the Tests target (requires Vendor/googletest submodule)" ON)

# Eigen is header-only; expose it as an interface target rather than pulling
# in its own CMakeLists (which builds Eigen's test/doc targets by default).
add_library(Eigen3 INTERFACE)
add_library(Eigen3::Eigen ALIAS Eigen3)
target_include_directories(Eigen3 INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/Vendor/Eigen
)

add_subdirectory(Core)
add_subdirectory(Propagate)
add_subdirectory(App)

if(ENGINE_BUILD_TESTS)
    enable_testing()
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
    add_subdirectory(Vendor/googletest)
    add_subdirectory(Tests)
endif()
