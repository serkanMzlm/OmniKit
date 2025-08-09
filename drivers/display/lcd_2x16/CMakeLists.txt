file(GLOB_RECURSE LCD_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_library(lcdLib ${LCD_FILES})
target_link_libraries(lcdLib PUBLIC i2c)

target_include_directories(lcdLib PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
    $<INSTALL_INTERFACE:include/lcd_2x16>
)

install(TARGETS lcdLib
    EXPORT lcdLibTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY inc/ 
        DESTINATION include/lcd_2x16)

install(EXPORT lcdLibTargets
    FILE lcdLibConfig.cmake
    DESTINATION lib/cmake/lcdLib
)