cmake_minimum_required(VERSION 3.1)
project(drivers)

file(GLOB_RECURSE source_file ${CMAKE_SOURCE_DIR}/mpu6050/src/*.cpp)
include_directories(${CMAKE_SOURCE_DIR}/mpu6050/include/)

add_executable(mpu6050 ${source_file})

target_link_libraries(mpu6050 -li2c)

