cmake_minimum_required(VERSION 3.0)
project(test)

set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};$ENV{OMNIKIT_PATH}")

find_package(omnikit_math REQUIRED)
find_package(omnikit_utils REQUIRED)
find_package(omnikit_geometry REQUIRED)

find_package (Eigen3 3.3 REQUIRED)

add_executable(main ${CMAKE_SOURCE_DIR}/main.cpp)
target_link_libraries(main omnikit_math omnikit_utils omnikit_geometry)
