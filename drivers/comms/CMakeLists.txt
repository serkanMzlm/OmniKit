file(GLOB_RECURSE BASE_FILE ${CMAKE_CURRENT_SOURCE_DIR}/base/*.cpp)
# file(GLOB_RECURSE UART_FILE ${CMAKE_CURRENT_SOURCE_DIR}/uart/*.cpp)
# file(GLOB_RECURSE UDP_FILE ${CMAKE_CURRENT_SOURCE_DIR}/udp/*.cpp)

add_library(omnikit_comms STATIC
    ${BASE_FILE}

)

target_include_directories(omnikit_comms PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/base>
    $<INSTALL_INTERFACE:include/>
)

install_library(omnikit_comms ${CMAKE_CURRENT_SOURCE_DIR})