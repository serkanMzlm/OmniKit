file(GLOB_RECURSE SRC_FILES src/*.cpp)

add_library(mathToolsLib ${SRC_FILES})

target_include_directories(mathToolsLib PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
    $<INSTALL_INTERFACE:include/>
)

install(TARGETS mathToolsLib
    EXPORT mathToolsLibTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY inc/
        DESTINATION include/math_tools)

install(EXPORT mathToolsLibTargets
    FILE mathToolsLibConfig.cmake
    DESTINATION lib/cmake/mathToolsLib
)