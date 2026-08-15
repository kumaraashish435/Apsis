add_library(Core STATIC
    Source/Core/Core.cpp
    Source/Core/Core.h
)

target_include_directories(Core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Source
)

target_link_libraries(Core PUBLIC
    Eigen3::Eigen
)

target_compile_definitions(Core PUBLIC
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:RELEASE>
    $<$<CONFIG:Dist>:DIST>
)
