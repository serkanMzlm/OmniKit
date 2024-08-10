cmake_minimum_required(VERSION 3.1)
project(drivers)

set(USE_MPU6050 false)
set(USE_BMP180 false)
set(USE_LCD false)
set(USE_VL53L5CX false) # there are mistakes
set(USE_PMW3901 false)
set(USE_GPIO true)
set(MATH_TOOLS true)
set(GEOMETRY_TOOLS true)

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

if(MATH_TOOLS)
add_subdirectory(math_tools)
endif()

if(GEOMETRY_TOOLS)
add_subdirectory(geometry_tools)
endif()