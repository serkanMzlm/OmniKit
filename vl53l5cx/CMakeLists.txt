set(VL_PLATFORM_PATH "${CMAKE_CURRENT_SOURCE_DIR}/en.STSW-IMG023/VL53L5CX_ULD_driver_1.3.10/Platform")
set(VL_API_PATH "${CMAKE_CURRENT_SOURCE_DIR}/en.STSW-IMG023/VL53L5CX_ULD_driver_1.3.10/VL53L5CX_ULD_API")

file(GLOB_RECURSE VL_PLATFORM_FILES "${VL_PLATFORM_PATH}/*.cpp")
file(GLOB_RECURSE VL_API_FILES "${VL_API_PATH}/src/*.cpp")

add_library(vl53l5cx_lib 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/vl53l5cx.cpp 
    ${VL_PLATFORM_FILES} 
    ${VL_API_FILES}
)

target_include_directories(vl53l5cx_lib PUBLIC 
                ${CMAKE_CURRENT_SOURCE_DIR}/inc
                ${VL_PLATFORM_PATH}
                ${VL_API_PATH}/inc
)
