set(VL_PLATFORM_PATH "${CMAKE_CURRENT_SOURCE_DIR}/en.STSW-IMG023/VL53L5CX_ULD_driver_1.3.10/Platform")
set(VL_API_PATH "${CMAKE_CURRENT_SOURCE_DIR}/en.STSW-IMG023/VL53L5CX_ULD_driver_1.3.10/VL53L5CX_ULD_API")

file(GLOB_RECURSE VL_PLATFORM_FILES "${VL_PLATFORM_PATH}/*.cpp")
file(GLOB_RECURSE VL_API_FILES "${VL_API_PATH}/src/*.cpp")

add_library(vl53l5cxLib 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/vl53l5cx.cpp 
    ${VL_PLATFORM_FILES} 
    ${VL_API_FILES}
)

target_include_directories(vl53l5cxLib PUBLIC 
                ${CMAKE_CURRENT_SOURCE_DIR}/inc
                ${VL_PLATFORM_PATH}
                ${VL_API_PATH}/inc
)

# target_include_directories(vl53l5cxLib PUBLIC 
#     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#     $<INSTALL_INTERFACE:include>
# )

# install(TARGETS vl53l5cxLib
#     EXPORT vl53l5cxLibTargets
#     LIBRARY DESTINATION lib
#     ARCHIVE DESTINATION lib
#     RUNTIME DESTINATION bin
#     INCLUDES DESTINATION include
# )

# install(DIRECTORY inc/ DESTINATION include)

# install(EXPORT vl53l5cxLibTargets
#     FILE vl53l5cxLibConfig.cmake
#     DESTINATION lib/cmake/vl53l5cxLib
# )