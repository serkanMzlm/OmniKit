file(GLOB_RECURSE MPU6050_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(mpu6050_lib ${MPU6050_FILES})
target_include_directories(mpu6050_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
target_link_libraries(mpu6050_lib PUBLIC i2c)
