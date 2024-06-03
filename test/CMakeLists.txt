cmake_minimum_required(VERSION 3.0)
project(test)

if(CMAKE_PREFIX_PATH)
    set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${CMAKE_CURRENT_SOURCE_DIR}/MyLibrary/install/lib/cmake")
    # set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}; $ENV{SECURE_DRIVE_VEHICLE_PATH}")
else()
    # set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/MyLibrary/install/lib/cmake")
    set(CMAKE_PREFIX_PATH "$ENV{SECURE_DRIVE_VEHICLE_PATH}/install/lib/cmake")
    message("Path: ${CMAKE_PREFIX_PATH}")
endif()

find_package(bmp180Lib REQUIRED)
find_package(gpioLib REQUIRED)
find_package(lcdLib REQUIRED)
find_package(mpu6050Lib REQUIRED)
find_package(pmw3901Lib REQUIRED)

add_executable(bmp180_sensor ${CMAKE_SOURCE_DIR}/bmp180_sensor.cpp)
target_link_libraries(bmp180_sensor bmp180Lib)

# add_executable(gpio_control ${CMAKE_SOURCE_DIR}/gpio.cpp)
# target_link_libraries(gpio_control gpioLib)

add_executable(lcd_display ${CMAKE_SOURCE_DIR}/lcd_display.cpp)
target_link_libraries(lcd_display lcdLib)

add_executable(mpu6050_sensor ${CMAKE_SOURCE_DIR}/mpu6050_sensor.cpp)
target_link_libraries(mpu6050_sensor mpu6050Lib)

add_executable(pmw3901_sensor ${CMAKE_SOURCE_DIR}/pmw3901_sensor.cpp)
target_link_libraries(pmw3901_sensor pmw3901Lib)
