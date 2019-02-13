set(SRC
${CMAKE_CURRENT_SOURCE_DIR}/task_init.c
${CMAKE_CURRENT_SOURCE_DIR}/os_bridge.c
${CMAKE_CURRENT_SOURCE_DIR}/osapp_task.c
${CMAKE_CURRENT_SOURCE_DIR}/ble_task.c
${CMAKE_CURRENT_SOURCE_DIR}/osapp_utils.c
)

add_sources(RTOS_APP_SRCS ${SRC})
target_include_directories(rtos_app_inc
INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)


