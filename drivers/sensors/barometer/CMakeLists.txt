file(GLOB_RECURSE BMP180_FILES ${CMAKE_CURRENT_SOURCE_DIR}/bmp180/*.cpp)

add_library(omnikit_barometer ${BMP180_FILES})
target_link_libraries(omnikit_barometer PUBLIC i2c omnikit_comms)

target_include_directories(omnikit_barometer PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

install_library(omnikit_barometer ${CMAKE_CURRENT_SOURCE_DIR})