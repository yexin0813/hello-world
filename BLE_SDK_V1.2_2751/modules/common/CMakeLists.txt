set(SRC
${CMAKE_CURRENT_SOURCE_DIR}/src/co_utils.c
${CMAKE_CURRENT_SOURCE_DIR}/src/co_list.c
)
add_sources(ROM_00_SRCS ${SRC})
set(SRC_USR
${CMAKE_CURRENT_SOURCE_DIR}/src/bx_ring_queue.c
)
add_sources(USR_SRCS ${SRC_USR})
target_include_directories(fw_inc
INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/api
)