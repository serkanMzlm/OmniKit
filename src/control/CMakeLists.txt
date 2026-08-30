add_library(omnikit_controller
    pid.cpp
)

add_library(omnikit::controller ALIAS omnikit_controller)

target_include_directories(omnikit_controller
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(omnikit_controller
    PUBLIC omnikit::math
)

target_compile_features(omnikit_controller PUBLIC cxx_std_17)

target_compile_options(omnikit_controller PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

install(TARGETS omnikit_controller
    EXPORT omnikitTargets
    ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)