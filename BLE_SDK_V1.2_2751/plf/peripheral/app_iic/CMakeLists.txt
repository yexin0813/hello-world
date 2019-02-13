add_sources(USR_SRCS
${CMAKE_CURRENT_SOURCE_DIR}/app_iic.c
)
target_include_directories(usr_inc
INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)