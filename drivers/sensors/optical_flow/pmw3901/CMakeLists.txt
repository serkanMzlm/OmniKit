file(GLOB_RECURSE PMW3901_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_library(pmw3901Lib ${PMW3901_FILES})

target_include_directories(pmw3901Lib PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
    $<INSTALL_INTERFACE:include/pmw3901>
)

install(TARGETS pmw3901Lib
    EXPORT pmw3901LibTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY inc/ 
        DESTINATION include/pmw3901)

install(EXPORT pmw3901LibTargets
    FILE pmw3901LibConfig.cmake
    DESTINATION lib/cmake/pmw3901Lib
)