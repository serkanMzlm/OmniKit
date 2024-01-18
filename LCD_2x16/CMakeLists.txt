file(GLOB_RECURSE LCD_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
add_library(lcd_lib ${LCD_FILES})
target_include_directories(lcd_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
target_link_libraries(lcd_lib PUBLIC i2c)
