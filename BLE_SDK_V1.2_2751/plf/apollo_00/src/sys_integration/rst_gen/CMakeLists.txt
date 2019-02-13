add_sources(PLF_00_SRCS
${CMAKE_CURRENT_SOURCE_DIR}/rst_gen.c
)

target_include_directories(inc_00
INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)  