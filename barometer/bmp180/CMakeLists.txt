file(GLOB_RECURSE BMP180_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(bmp180Lib ${BMP180_FILES})
target_link_libraries(bmp180Lib PUBLIC i2c)

target_include_directories(bmp180Lib PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
    $<INSTALL_INTERFACE:include/bmp180>
)

install(TARGETS bmp180Lib
    EXPORT bmp180LibTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY inc/ 
        DESTINATION include/bmp180)

install(EXPORT bmp180LibTargets
    FILE bmp180LibConfig.cmake
    DESTINATION lib/cmake/bmp180Lib
)