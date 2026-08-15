add_library(Propagate STATIC
    Source/propagate/GpDataLoader.cpp
    Source/propagate/SatelliteTracker.cpp
)

target_include_directories(Propagate PUBLIC
    Source
)

target_include_directories(Propagate PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../Vendor/AstroStds/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../Vendor/AstroStds/include/wrappers
    ${CMAKE_CURRENT_SOURCE_DIR}/../Vendor/AstroStds/include/services
)

# --- AstroStds (SGP4Prop) prebuilt library location, per host platform ---
if(APPLE)
    set(ASTROSTDS_PLATFORM_DIR "macos-arm64")
elseif(WIN32)
    set(ASTROSTDS_PLATFORM_DIR "windows-x64")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|arm64")
    set(ASTROSTDS_PLATFORM_DIR "linux-arm64")
else()
    set(ASTROSTDS_PLATFORM_DIR "linux-x64")
endif()

# Normalized (no "Propagate/../" left in the string): this gets baked into
# the binary and read back at runtime, where the Propagate/ source
# directory this path is computed relative to may not even exist (e.g. a
# trimmed Docker runtime image that only carries the built binary and the
# vendored libs) -- an un-normalized path would only resolve if every
# intermediate ".." component still existed on disk to traverse through.
get_filename_component(ASTROSTDS_LIB_DIR_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/../Vendor/AstroStds/lib/${ASTROSTDS_PLATFORM_DIR}"
    ABSOLUTE)
set(ASTROSTDS_LIB_DIR_PATH "${ASTROSTDS_LIB_DIR_PATH}"
    CACHE INTERNAL "Absolute path to the vendored AstroStds libraries for this host platform")

# The SGP4 engine's Sgp4InitSat() looks for SGP4_Open_License.txt via
# LD_LIBRARY_PATH at call time (GpDataLoader.cpp sets that env var itself
# using this macro), so this is needed at compile time regardless of platform.
target_compile_definitions(Propagate PRIVATE
    ASTROSTDS_LIB_DIR="${ASTROSTDS_LIB_DIR_PATH}"
)

# Header-only wrappers dlopen() the vendored libraries by their bare file
# name at runtime, so every *executable* that links Propagate (App, Tests,
# ...) needs to call this on itself so those libraries are discoverable:
#   - macOS/Linux: bake an rpath pointing at the vendored lib directory
#     (Linux additionally needs old-style DT_RPATH via --disable-new-dtags
#     so it's honored transitively when a dlopen()'d library pulls in its
#     own further dependencies, e.g. libgfortran).
#   - Windows: no rpath concept: copy the DLLs next to the .exe after each
#     build instead, so LoadLibraryA()'s default search order finds them.
function(astrostds_configure_runtime target)
    if(APPLE OR (UNIX AND NOT WIN32))
        set_target_properties(${target} PROPERTIES
            BUILD_RPATH "${ASTROSTDS_LIB_DIR_PATH}"
        )
    endif()
    if(UNIX AND NOT APPLE)
        target_link_options(${target} PRIVATE "-Wl,--disable-new-dtags")
    endif()
    if(WIN32)
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                    "${ASTROSTDS_LIB_DIR_PATH}"
                    "$<TARGET_FILE_DIR:${target}>"
        )
    endif()
endfunction()
