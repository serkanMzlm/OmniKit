file(GLOB_RECURSE MPU6050_FILES mpu6050/*.cpp)

add_library(omnikit_imu ${MPU6050_FILES})
target_link_libraries(omnikit_imu PUBLIC i2c omnikit_comms)

target_include_directories(omnikit_imu PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include/>
)

install_library(omnikit_imu ${CMAKE_CURRENT_SOURCE_DIR})