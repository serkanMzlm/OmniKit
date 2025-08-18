file(GLOB_RECURSE OMNIKIT_FILTER_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

find_package (Eigen3 3.3 REQUIRED)

add_library(omnikit_filter ${OMNIKIT_FILTER_SOURCES})

target_link_libraries(omnikit_filter PUBLIC Eigen3::Eigen)

target_include_directories(omnikit_filter PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
    $<INSTALL_INTERFACE:include/>
)

install_library(omnikit_filter ${CMAKE_CURRENT_SOURCE_DIR})