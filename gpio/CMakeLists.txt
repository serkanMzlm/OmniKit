file(GLOB_RECURSE GPIO_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_library(gpio_lib ${GPIO_FILES})
target_include_directories(gpio_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
