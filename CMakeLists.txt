cmake_minimum_required(VERSION 3.1)
project(OmniKit)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -pedantic)
  add_compile_options(-Werror)
  add_compile_options(-Werror=return-type)
  add_compile_options(-Werror=address)
  add_compile_options(-Werror=sequence-point)
  add_compile_options(-Werror=format)
  add_compile_options(-Werror=array-bounds)
  add_compile_options(-Werror=enum-compare)
  add_compile_options(-Werror=init-self)
  add_compile_options(-Werror=logical-op)
endif()

list(APPEND CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/cmake
)

include(InstallLibrary)
include(ColorMessage)

option(TEST_MODE   "Examples-only build"           OFF)
option(USE_CORE    "Enable core features"          ON)
option(USE_DRIVERS "Enable drivers"                ON)
option(USE_CONTROL "Enable control features"       OFF)

list(APPEND FEATURES "USE_CORE"     "core")
list(APPEND FEATURES "USE_DRIVERS"  "drivers")
list(APPEND FEATURES "USE_CONTROL"  "control")

if(TEST_MODE)
  message("Test mode is enabled")
  add_subdirectory(examples)
  return()
endif()

add_subdirectory(core) 
add_subdirectory(drivers) 
add_subdirectory(control) 


####################################################################
##  The code was written with if to make it more understandable   ##
####################################################################

# set(FEATURES)
# list(LENGTH FEATURES LIST_LENGTH)
# math(EXPR ADJUSTED_LENGTH "${LIST_LENGTH} - 2")
# foreach(i RANGE 0 ${ADJUSTED_LENGTH} 2)
#     list(GET FEATURES ${i} FEATURE)
#     math(EXPR j "${i} + 1") 
#     list(GET FEATURES ${j} DIR)
#     if(${${FEATURE}})
#         add_subdirectory(${DIR})
#     endif()
# endforeach()