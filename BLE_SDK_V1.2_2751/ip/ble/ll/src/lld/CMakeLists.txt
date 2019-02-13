set(SRC
${CMAKE_CURRENT_SOURCE_DIR}/lld.c
${CMAKE_CURRENT_SOURCE_DIR}/lld_pdu.c
${CMAKE_CURRENT_SOURCE_DIR}/lld_evt.c
${CMAKE_CURRENT_SOURCE_DIR}/lld_wlcoex.c
${CMAKE_CURRENT_SOURCE_DIR}/lld_util.c
)
add_sources(ROM_00_SRCS ${SRC})
add_sources(USR_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/bx_lld_sleep.c)
target_include_directories(fw_inc INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})