file(GLOB_RECURSE OMNIKIT_UTILS_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(omnikit_utils ${OMNIKIT_UTILS_SOURCES})

target_include_directories(omnikit_utils PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
    $<INSTALL_INTERFACE:include>
)

install(TARGETS omnikit_utils
    EXPORT omnikit_utils-targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include 
)

install(DIRECTORY inc/
    DESTINATION include/omnikit/utils
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)

install(EXPORT omnikit_utils-targets
    FILE omnikit_utils-config.cmake
    DESTINATION lib/cmake/omnikit_utils
)