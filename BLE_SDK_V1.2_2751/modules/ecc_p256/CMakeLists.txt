set(SRC
${CMAKE_CURRENT_SOURCE_DIR}/src/ecc_p256.c
)

add_sources(USR_SRCS ${SRC})

target_include_directories(usr_inc
INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/api
)