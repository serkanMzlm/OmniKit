file(GLOB_RECURSE PMW3901_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_library(pmw3901_lib ${PMW3901_FILES})
target_include_directories(pmw3901_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
