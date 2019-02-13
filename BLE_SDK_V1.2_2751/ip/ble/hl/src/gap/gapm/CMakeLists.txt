set(SRC
${CMAKE_CURRENT_SOURCE_DIR}/gapm.c
${CMAKE_CURRENT_SOURCE_DIR}/gapm_task.c
${CMAKE_CURRENT_SOURCE_DIR}/gapm_util.c
${CMAKE_CURRENT_SOURCE_DIR}/gapm_hci.c
)
add_sources(ROM_00_SRCS ${SRC})

target_include_directories(fw_inc
INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)