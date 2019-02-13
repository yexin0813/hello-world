set(SRC
${CMAKE_CURRENT_SOURCE_DIR}/rwble.c
)
add_sources(ROM_00_SRCS ${SRC})
add_sources(USR_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/bx_rwble.c)

target_include_directories(fw_inc INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})