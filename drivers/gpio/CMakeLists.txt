file(GLOB_RECURSE GPIO_FILE ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_library(omnikit_gpio ${GPIO_FILE})

target_include_directories(omnikit_gpio PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

install_library(omnikit_gpio ${CMAKE_CURRENT_SOURCE_DIR})