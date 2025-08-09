if(NOT USE_CORE)
  return()
endif()

message("${Cyan}##############################################")
message("${Cyan}#         Core Features Are Enabled          #")
message("${Cyan}##############################################")

add_subdirectory(math)
add_subdirectory(utils)
add_subdirectory(geometry)
add_subdirectory(filters)
