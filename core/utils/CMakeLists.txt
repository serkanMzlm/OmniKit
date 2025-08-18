file(GLOB_RECURSE OMNIKIT_UTILS_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(omnikit_utils ${OMNIKIT_UTILS_SOURCES})

target_include_directories(omnikit_utils PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

install_library(omnikit_utils ${CMAKE_CURRENT_SOURCE_DIR})