set(SRC
${CMAKE_CURRENT_SOURCE_DIR}/osapp_advintv_change.c
)

add_sources(USR_SRCS ${SRC})
target_include_directories(usr_inc
INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)
