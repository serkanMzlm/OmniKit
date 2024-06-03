file(GLOB_RECURSE MPU6050_FILES src/*.cpp)

add_library(mpu6050Lib ${MPU6050_FILES})
target_link_libraries(mpu6050Lib PUBLIC i2c)
# target_include_directories(mpu6050Lib PUBLIC inc)

target_include_directories(mpu6050Lib PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
    $<INSTALL_INTERFACE:include/mpu6050>
)

install(TARGETS mpu6050Lib
    EXPORT mpu6050LibTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY inc/
        DESTINATION include/mpu6050)

install(EXPORT mpu6050LibTargets
    FILE mpu6050LibConfig.cmake
    DESTINATION lib/cmake/mpu6050Lib
)
