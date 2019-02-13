add_sources(USR_SRCS
${CMAKE_CURRENT_SOURCE_DIR}/app_dmac.c
${CMAKE_CURRENT_SOURCE_DIR}/app_dmac_wrapper.c

)
target_include_directories(usr_inc
INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)