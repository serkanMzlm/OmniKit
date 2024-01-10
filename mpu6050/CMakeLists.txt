set(MPU6050_SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)
file(GLOB_RECURSE mpu6050_file ${MPU6050_SRC_PATH}/*.cpp)

add_library(mpu6050_lib ${mpu6050_file})
target_include_directories(mpu6050_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
target_link_libraries(mpu6050_lib i2c)
