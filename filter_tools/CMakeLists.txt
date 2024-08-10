file(GLOB_RECURSE SRC_FILES src/*.cpp)

find_package (Eigen3 3.3 REQUIRED)

add_library(filterToolsLib ${SRC_FILES})

target_include_directories(filterToolsLib PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
    $<INSTALL_INTERFACE:include/>
)

target_link_libraries(filterToolsLib PUBLIC Eigen3::Eigen)

install(TARGETS filterToolsLib
    EXPORT filterToolsLibTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY inc/
        DESTINATION include/filter_tools)

install(EXPORT filterToolsLibTargets
    FILE filterToolsLibConfig.cmake
    DESTINATION lib/cmake/filterToolsLib
)