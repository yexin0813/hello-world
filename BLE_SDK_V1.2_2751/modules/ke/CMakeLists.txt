set(SRC
${CMAKE_CURRENT_SOURCE_DIR}/src/ke_timer.c
${CMAKE_CURRENT_SOURCE_DIR}/src/ke_msg.c
${CMAKE_CURRENT_SOURCE_DIR}/src/ke_queue.c
${CMAKE_CURRENT_SOURCE_DIR}/src/ke_mem.c
${CMAKE_CURRENT_SOURCE_DIR}/src/ke_task.c
${CMAKE_CURRENT_SOURCE_DIR}/src/ke_event.c
${CMAKE_CURRENT_SOURCE_DIR}/src/ke.c
)
add_sources(ROM_00_SRCS ${SRC})

target_include_directories(fw_inc 
INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/api
)