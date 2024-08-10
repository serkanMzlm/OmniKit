file(GLOB_RECURSE SRC_FILES src/*.cpp)

find_package (Eigen3 3.3 REQUIRED)

add_library(geometryToolsLib ${SRC_FILES})

target_include_directories(geometryToolsLib PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
    $<INSTALL_INTERFACE:include/>
)

target_link_libraries(geometryToolsLib PUBLIC Eigen3::Eigen)

install(TARGETS geometryToolsLib
    EXPORT geometryToolsLibTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY inc/
        DESTINATION include/geometry_tools)

install(EXPORT geometryToolsLibTargets
    FILE geometryToolsLibConfig.cmake
    DESTINATION lib/cmake/geometryToolsLib
)