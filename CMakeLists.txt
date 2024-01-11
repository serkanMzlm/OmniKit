cmake_minimum_required(VERSION 3.1)
project(drivers)

add_subdirectory(mpu6050)
add_executable(mpu6050_sensor ${CMAKE_SOURCE_DIR}/test/mpu6050_sensor.cpp)
target_link_libraries(mpu6050_sensor mpu6050_lib)

