add_subdirectory(ble)
add_subdirectory(hci)
add_subdirectory(ahi)
add_subdirectory(ea)
add_subdirectory(em)

target_include_directories(usr_inc
INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

