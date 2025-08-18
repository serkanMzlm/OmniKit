file(GLOB_RECURSE OMNIKIT_FILTER_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

find_package (Eigen3 3.3 REQUIRED)

add_library(omnikit_filter ${OMNIKIT_FILTER_SOURCES})

target_link_libraries(omnikit_filter PUBLIC Eigen3::Eigen)

target_include_directories(omnikit_filter PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
    $<INSTALL_INTERFACE:include/>
)

install(TARGETS omnikit_filter
    EXPORT omnikit_filter-targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY inc/
        DESTINATION include/omnikit/geometry
        FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)
        
install(EXPORT omnikit_filter-targets
    FILE omnikit_filter-config.cmake
    DESTINATION lib/cmake/omnikit_filter
)