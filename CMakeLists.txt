cmake_minimum_required(VERSION 3.14)

project(omnikit
    VERSION 0.1.0
    DESCRIPTION "A reusable C++ utility library: math, logging, filters"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(OMNIKIT_BUILD_TESTS "Build the unit tests" ON)

add_subdirectory(src/log)
add_subdirectory(src/progress)
add_subdirectory(src/geometry)
add_subdirectory(src/time)
add_subdirectory(src/math)
add_subdirectory(src/controller)
add_subdirectory(src/filter)

if(OMNIKIT_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(DIRECTORY include/omnikit
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT omnikitTargets
    FILE omnikitTargets.cmake
    NAMESPACE omnikit::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/omnikit
)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/omnikitConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/omnikitConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/omnikit
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/omnikitConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/omnikitConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/omnikitConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/omnikit
)

if(NOT TARGET uninstall)
    add_custom_target(uninstall
        COMMAND ${CMAKE_COMMAND}
                -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/omnikitUninstall.cmake
        COMMENT "Uninstalling omnikit (removing installed files)..."
    )
endif()