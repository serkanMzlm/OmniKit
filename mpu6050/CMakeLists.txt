file(GLOB_RECURSE mpu_src ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(mpu6050_lib ${mpu_src})
target_include_directories(mpu6050_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
target_link_libraries(mpu6050_lib PUBLIC i2c)