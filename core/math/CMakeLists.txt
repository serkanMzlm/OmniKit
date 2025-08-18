file(GLOB_RECURSE OMNIKIT_MATH_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(omnikit_math ${OMNIKIT_MATH_SOURCES})

target_include_directories(omnikit_math PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
    $<INSTALL_INTERFACE:include>
)

install(TARGETS omnikit_math
    EXPORT omnikit_math-targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include 
)

install(DIRECTORY inc/
    DESTINATION include/omnikit/math
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)

install(EXPORT omnikit_math-targets
    FILE omnikit_math-config.cmake
    DESTINATION lib/cmake/omnikit_math
)