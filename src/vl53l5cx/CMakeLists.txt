cmake_minimum_required(VERSION 3.8)
project(vl53l5cx)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(visualization_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(Boost REQUIRED system)
find_package(OpenCV REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(PCL 1.3 REQUIRED)
find_package(pcl_ros REQUIRED )
find_package(pcl_conversions REQUIRED )

file(GLOB_RECURSE MY_SOURCE 
    ${PROJECT_SOURCE_DIR}/src/*.cpp 
    ${PROJECT_SOURCE_DIR}/src/vl53l5cx/*.c
    )

include_directories(${PROJECT_SOURCE_DIR}/inc/vl53l5cx)
include_directories(${PROJECT_SOURCE_DIR}/inc)

add_executable(vl53l5cx_node ${MY_SOURCE})

ament_target_dependencies(vl53l5cx_node
  rclcpp
  sensor_msgs
  geometry_msgs
  Boost
  visualization_msgs
  OpenCV
  tf2_ros
  PCL
  pcl_ros
  pcl_conversions
)

install(TARGETS
  vl53l5cx_node
  DESTINATION 
  lib/${PROJECT_NAME}  
)

ament_package()