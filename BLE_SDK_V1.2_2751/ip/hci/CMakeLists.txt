set(SRC
${CMAKE_CURRENT_SOURCE_DIR}/src/hci.c
${CMAKE_CURRENT_SOURCE_DIR}/src/hci_tl.c
${CMAKE_CURRENT_SOURCE_DIR}/src/hci_msg.c
${CMAKE_CURRENT_SOURCE_DIR}/src/hci_util.c
${CMAKE_CURRENT_SOURCE_DIR}/src/hci_fc.c
)
add_sources(ROM_00_SRCS ${SRC})

target_include_directories(fw_inc
INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/api)