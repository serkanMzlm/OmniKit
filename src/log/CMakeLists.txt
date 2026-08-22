add_library(omnikit_log
    log_level.cpp
    console_sink.cpp
    file_sink.cpp
    logger.cpp
)

add_library(omnikit::log ALIAS omnikit_log)

target_include_directories(omnikit_log
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(omnikit_log PUBLIC cxx_std_17)

target_compile_options(omnikit_log PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

install(TARGETS omnikit_log
    EXPORT omnikitTargets
    ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
