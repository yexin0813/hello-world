add_subdirectory(app_qspi)
add_subdirectory(app_spi)

target_include_directories(usr_inc
INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)