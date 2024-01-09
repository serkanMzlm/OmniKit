cmake_minimum_required(VERSION 3.1)
project(drivers)

file(GLOB_RECURSE MPU6050_SRC_FILE ${CMAKE_SOURCE_DIR}/mpu6050/src/*.cpp)
file(GLOB_RECURSE BMP180_SRC_FILE ${CMAKE_SOURCE_DIR}/bmp180/src/*.cpp)
include_directories(${CMAKE_SOURCE_DIR}/mpu6050/include/)
include_directories(${CMAKE_SOURCE_DIR}/bmp180/include/)

add_executable(mpu6050 ${MPU6050_SRC_FILE})
add_executable(bmp180 ${BMP180_SRC_FILE})

target_link_libraries(mpu6050 -li2c)

