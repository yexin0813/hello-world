set(SRC
${CMAKE_CURRENT_SOURCE_DIR}/osapp_1M2M_master.c
)

add_sources(USR_SRCS ${SRC})
target_include_directories(usr_inc
INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)
