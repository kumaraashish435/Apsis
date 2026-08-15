add_executable(App
    Source/App.cpp
)

target_link_libraries(App PRIVATE Core Propagate)
astrostds_configure_runtime(App)

target_compile_definitions(App PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:RELEASE>
    $<$<CONFIG:Dist>:DIST>
)
