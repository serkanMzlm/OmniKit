cmake_minimum_required(VERSION 3.1)
project(drivers)

add_subdirectory(mpu6050)
add_executable(mpu6050_sensor ${CMAKE_SOURCE_DIR}/test/mpu6050_sensor.cpp)
target_link_libraries(mpu6050_sensor mpu6050_lib)

add_subdirectory(vl53l5cx)
add_executable(vl53l5cx_sensor ${CMAKE_SOURCE_DIR}/test/vl53l5cx_sensor.cpp)
target_link_libraries(vl53l5cx_sensor vl53l5cx_lib)
