add_sources(PLF_00_SRCS
${CMAKE_CURRENT_SOURCE_DIR}/sysctrl.c
${CMAKE_CURRENT_SOURCE_DIR}/rf_reg_settings_apollo_00.c
${CMAKE_CURRENT_SOURCE_DIR}/io_ctrl.c
${CMAKE_CURRENT_SOURCE_DIR}/touch.c
)

target_include_directories(inc_00
INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)  