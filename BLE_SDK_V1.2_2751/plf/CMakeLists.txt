add_subdirectory(inc)
add_subdirectory(apollo_00)
#add_subdirectory(apollo_01)
add_subdirectory(bx_debug)
add_subdirectory(peripheral)

target_include_directories(usr_inc
INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)