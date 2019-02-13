set(SRC
${CMAKE_CURRENT_SOURCE_DIR}/ble_peripheral_simple.c
)

add_sources(USR_SRCS ${SRC})
target_include_directories(usr_inc
INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)


