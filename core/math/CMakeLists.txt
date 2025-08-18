file(GLOB_RECURSE OMNIKIT_MATH_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(omnikit_math ${OMNIKIT_MATH_SOURCES})

target_include_directories(omnikit_math PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
    $<INSTALL_INTERFACE:include>
)

install_library(omnikit_math ${CMAKE_CURRENT_SOURCE_DIR})