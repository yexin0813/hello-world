target_include_directories(fw_inc 
INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/api
INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/inc)
add_subdirectory(src)


