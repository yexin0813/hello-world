set(SRC
${CMAKE_CURRENT_SOURCE_DIR}/em_buf.c
)
add_sources(ROM_00_SRCS ${SRC})

target_include_directories(fw_inc 
INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)