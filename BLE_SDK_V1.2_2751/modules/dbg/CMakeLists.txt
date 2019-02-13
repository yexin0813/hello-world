set(SRC
${CMAKE_CURRENT_SOURCE_DIR}/src/dbg_task.c
${CMAKE_CURRENT_SOURCE_DIR}/src/dbg_swdiag.c
${CMAKE_CURRENT_SOURCE_DIR}/src/dbg_mwsgen.c
${CMAKE_CURRENT_SOURCE_DIR}/src/dbg.c

)
add_sources(ROM_00_SRCS ${SRC})

target_include_directories(fw_inc
INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/api
)