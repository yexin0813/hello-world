
set(SRC
${CMAKE_CURRENT_SOURCE_DIR}/portable/${CMAKE_C_COMPILER_ID}/arm_cm0/port.c
${CMAKE_CURRENT_SOURCE_DIR}/tasks.c
${CMAKE_CURRENT_SOURCE_DIR}/queue.c
${CMAKE_CURRENT_SOURCE_DIR}/list.c
${CMAKE_CURRENT_SOURCE_DIR}/timers.c
${CMAKE_CURRENT_SOURCE_DIR}/event_groups.c
${CMAKE_CURRENT_SOURCE_DIR}/croutine.c
${CMAKE_CURRENT_SOURCE_DIR}/stream_buffer.c
${CMAKE_CURRENT_SOURCE_DIR}/portable/memmang/heap_4.c
)

add_sources(RTOS_APP_SRCS ${SRC})

target_include_directories(rtos_app_inc
INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/portable/${CMAKE_C_COMPILER_ID}/arm_cm0/
)
