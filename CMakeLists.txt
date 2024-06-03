cmake_minimum_required(VERSION 3.1)
project(drivers)

set(USE_MPU6050 true)
set(USE_BMP180 true)
set(USE_LCD true)
set(USE_VL53L5CX false) # there are mistakes
set(USE_PMW3901 true)
set(USE_GPIO true)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

if(USE_BMP180)
add_subdirectory(barometer)
endif()

if(USE_LCD)
add_subdirectory(display)
endif()

if(USE_MPU6050)
add_subdirectory(imu)
endif()

if(USE_VL53L5CX)
add_subdirectory(distance)
endif()

if(USE_PMW3901)
add_subdirectory(optical_flow)
endif()

if(USE_GPIO)
add_subdirectory(gpio)
endif()