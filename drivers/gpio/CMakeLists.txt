file(GLOB_RECURSE GPIO_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_library(gpioLib ${GPIO_FILES})

target_include_directories(gpioLib PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
    $<INSTALL_INTERFACE:include>
)

install(TARGETS gpioLib
    EXPORT gpioLibTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY inc/ 
        DESTINATION include/gpio)

install(EXPORT gpioLibTargets
    FILE gpioLibConfig.cmake
    DESTINATION lib/cmake/gpioLib
)