find_package(Eigen3 3.3 REQUIRED NO_MODULE)

add_library(omnikit_filter
    lowpass.cpp
    highpass.cpp
    complementary.cpp
    kalman.cpp
)

add_library(omnikit::filter ALIAS omnikit_filter)

target_include_directories(omnikit_filter
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(omnikit_filter
    PUBLIC omnikit::math  Eigen3::Eigen
)

target_compile_features(omnikit_filter PUBLIC cxx_std_17)

target_compile_options(omnikit_filter PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

install(TARGETS omnikit_filter
    EXPORT omnikitTargets
    ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)