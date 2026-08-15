include(GoogleTest)

add_executable(Tests
    Source/EigenTests.cpp
    Source/PropagateTests.cpp
)

target_link_libraries(Tests PRIVATE
    Core
    Propagate
    GTest::gtest_main
)

astrostds_configure_runtime(Tests)

gtest_discover_tests(Tests)
