set(SRC
${CMAKE_CURRENT_SOURCE_DIR}/bx_dbg.c
${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_C_COMPILER_ID}/bx_dbg_asm.s
)

add_sources(USR_SRCS ${SRC})

target_include_directories(usr_inc
INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(log)
add_subdirectory(internal_log)