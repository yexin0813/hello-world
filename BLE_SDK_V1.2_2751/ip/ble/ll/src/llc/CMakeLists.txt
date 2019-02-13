set(SRC
${CMAKE_CURRENT_SOURCE_DIR}/llc_task.c
${CMAKE_CURRENT_SOURCE_DIR}/llc.c
${CMAKE_CURRENT_SOURCE_DIR}/llc_util.c
${CMAKE_CURRENT_SOURCE_DIR}/llc_llcp.c
${CMAKE_CURRENT_SOURCE_DIR}/llc_hci.c
${CMAKE_CURRENT_SOURCE_DIR}/llc_ch_asses.c
)
add_sources(ROM_00_SRCS ${SRC})

target_include_directories(fw_inc INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})