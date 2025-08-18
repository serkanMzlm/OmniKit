function(install_library lib_name source_dir) 
    install(TARGETS ${lib_name}
        EXPORT ${lib_name}Targets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
    )
    
    install(DIRECTORY ${source_dir}
        DESTINATION include/omnikit
        FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
    )
    
    install(EXPORT ${lib_name}Targets
        FILE ${lib_name}Config.cmake
        DESTINATION lib/cmake/${lib_name}
    )
endfunction()