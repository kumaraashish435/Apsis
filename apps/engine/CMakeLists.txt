add_library(orbitcoreEngine)

target_sources(orbitcoreEngine
    PRIVATE
        src/math/vector3d.cpp
        src/math/matrix3d.cpp
)

target_include_directories(orbitcore
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(orbitcoreEngine
    PUBLIC
        cxx_std_20
)

target_link_libraries(orbitcoreEngine
    PUBLIC
        Eigen3::Eigen
)

# ---------------------------------------------------------
# Tests
# ---------------------------------------------------------

if(ORBITCORE_BUILD_TESTS)

    include(GoogleTest)

    add_executable(orbitcore_tests
        tests/math/vector3d_test.cpp
        tests/math/matrix3d_test.cpp
    )

    target_link_libraries(orbitcore_tests
        PRIVATE
            orbitcore
            GTest::gtest_main
    )

    gtest_discover_tests(orbitcore_tests)

endif()