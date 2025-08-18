if(NOT USE_DRIVERS)
  return()
endif()

message("${Cyan}##############################################")
message("${Cyan}#         Drivers Features Are Enabled       #")
message("${Cyan}##############################################")

add_subdirectory(comms)