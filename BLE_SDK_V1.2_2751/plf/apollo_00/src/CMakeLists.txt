add_subdirectory(arch)
#add_subdirectory(driver)
add_subdirectory(sleep)
add_subdirectory(jmp_table)
add_subdirectory(patch_list)
#add_subdirectory(app_driver)
add_subdirectory(peripheral_integration)
add_subdirectory(sys_integration)
target_include_directories(inc_00
INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/build/ble-full/reg/fw
INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

