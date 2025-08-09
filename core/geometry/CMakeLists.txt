file(GLOB_RECURSE OMNIKIT_GEOMETRY_SOURCES src/*.cpp)

find_package (Eigen3 3.3 REQUIRED)

add_library(omnikit_geometry ${OMNIKIT_GEOMETRY_SOURCES})

target_include_directories(omnikit_geometry PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
    $<INSTALL_INTERFACE:include/>
)

target_link_libraries(omnikit_geometry PUBLIC Eigen3::Eigen)

install(TARGETS omnikit_geometry
    EXPORT omnikit_geometry-targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY inc/
        DESTINATION include/omnikit/geometry
        FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)
        
install(EXPORT omnikit_geometry-targets
    FILE omnikit_geometry-config.cmake
    DESTINATION lib/cmake/omnikit_geometry
)