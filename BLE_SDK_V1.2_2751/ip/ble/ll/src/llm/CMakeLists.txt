set(SRC
${CMAKE_CURRENT_SOURCE_DIR}/llm_task.c
${CMAKE_CURRENT_SOURCE_DIR}/llm_hci.c
${CMAKE_CURRENT_SOURCE_DIR}/llm.c
${CMAKE_CURRENT_SOURCE_DIR}/llm_util.c
)
add_sources(ROM_00_SRCS ${SRC})
target_include_directories(fw_inc INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})