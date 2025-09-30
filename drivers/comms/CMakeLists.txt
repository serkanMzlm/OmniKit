file(GLOB_RECURSE BASE_FILE ${CMAKE_CURRENT_SOURCE_DIR}/base/*.cpp)
file(GLOB_RECURSE UART_FILE ${CMAKE_CURRENT_SOURCE_DIR}/uart/*.cpp)
file(GLOB_RECURSE UDP_FILE ${CMAKE_CURRENT_SOURCE_DIR}/udp/*.cpp)
file(GLOB_RECURSE I2C_FILE ${CMAKE_CURRENT_SOURCE_DIR}/i2c/*.cpp)
file(GLOB_RECURSE SPI_FILE ${CMAKE_CURRENT_SOURCE_DIR}/spi/*.cpp)

add_library(omnikit_comms STATIC
    ${BASE_FILE}
    ${UART_FILE}
    ${UDP_FILE}
    ${I2C_FILE}
    ${SPI_FILE}
)

target_include_directories(omnikit_comms PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/base>
    $<INSTALL_INTERFACE:include/>
)

install_library(omnikit_comms ${CMAKE_CURRENT_SOURCE_DIR})