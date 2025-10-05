if(NOT USE_DRIVERS)
  return()
endif()

message("${Cyan}##############################################")
message("${Cyan}#         Drivers Features Are Enabled       #")
message("${Cyan}##############################################")

add_subdirectory(comms)
add_subdirectory(display)
add_subdirectory(gpio)
# add_subdirectory(pwm)
add_subdirectory(sensors)