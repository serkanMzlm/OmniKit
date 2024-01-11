cmake_minimum_required(VERSION 3.1)
project(drivers)

<<<<<<< HEAD

add_subdirectory(mpu6050)


add_executable(mpu6050_sensor ${CMAKE_SOURCE_DIR}/examples/mpu6050_sensor.cpp)
target_link_libraries(mpu6050_sensor mpu6050_lib)

# set(VL53L5CX_INC ${CMAKE_SOURCE_DIR}/vl53l5cx/en.STSW-IMG023/VL53L5CX_ULD_driver_1.3.10/VL53L5CX_ULD_API/inc)
# set(VL53L5CX_PLATFORM_INC ${CMAKE_SOURCE_DIR}/vl53l5cx/en.STSW-IMG023/VL53L5CX_ULD_driver_1.3.10/Platform)
# file(GLOB_RECURSE source_file ${CMAKE_SOURCE_DIR}/mpu6050/src/*.cpp)
# include_directories(${CMAKE_SOURCE_DIR}/mpu6050/inc/)
# target_link_libraries(mpu6050 -li2c)
=======
add_subdirectory(mpu6050)
add_executable(mpu6050_sensor ${CMAKE_SOURCE_DIR}/test/mpu6050_sensor.cpp)
target_link_libraries(mpu6050_sensor mpu6050_lib)
>>>>>>> origin/main

