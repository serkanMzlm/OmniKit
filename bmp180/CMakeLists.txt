file(GLOB_RECURSE BMP180_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(bmp180_lib ${BMP180_FILES})
target_include_directories(bmp180_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
target_link_libraries(bmp180_lib PUBLIC i2c)