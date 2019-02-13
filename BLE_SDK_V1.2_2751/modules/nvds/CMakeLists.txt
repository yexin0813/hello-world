set(SRC
${CMAKE_CURRENT_SOURCE_DIR}/src/nvds_in_ram.c
${CMAKE_CURRENT_SOURCE_DIR}/src/nvds_tag.c
${CMAKE_CURRENT_SOURCE_DIR}/src/nvds_task.c
)

add_sources(USR_SRCS ${SRC})

target_include_directories(usr_inc
INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/api
)