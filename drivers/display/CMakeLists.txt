file(GLOB_RECURSE LCD_2_16_FILE ${CMAKE_CURRENT_SOURCE_DIR}/lcd_2x16/*.cpp)
add_library(omnikit_display ${LCD_2_16_FILE})
target_link_libraries(omnikit_display PUBLIC i2c)

target_link_libraries(omnikit_display PUBLIC omnikit_comms)

target_include_directories(omnikit_display PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include/>
)

install_library(omnikit_display ${CMAKE_CURRENT_SOURCE_DIR})