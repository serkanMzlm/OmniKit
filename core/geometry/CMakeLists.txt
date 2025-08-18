file(GLOB_RECURSE OMNIKIT_GEOMETRY_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

find_package (Eigen3 3.3 REQUIRED)

add_library(omnikit_geometry ${OMNIKIT_GEOMETRY_SOURCES})

target_include_directories(omnikit_geometry PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
    $<INSTALL_INTERFACE:include/>
)

target_link_libraries(omnikit_geometry PUBLIC Eigen3::Eigen)

install_library(omnikit_geometry ${CMAKE_CURRENT_SOURCE_DIR})