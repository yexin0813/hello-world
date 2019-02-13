set(SRC

#${CMAKE_CURRENT_SOURCE_DIR}/patch_test.c
#${CMAKE_CURRENT_SOURCE_DIR}/rf_calib_sw_patch.c
${CMAKE_CURRENT_SOURCE_DIR}/tx_test_end.c
${CMAKE_CURRENT_SOURCE_DIR}/adv_int.c
${CMAKE_CURRENT_SOURCE_DIR}/h4tl_patch.c
${CMAKE_CURRENT_SOURCE_DIR}/set_adv_payload_31Byte_patch.c
${CMAKE_CURRENT_SOURCE_DIR}/prog_latency_patch.c
#${CMAKE_CURRENT_SOURCE_DIR}/patch_list.s
${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_C_COMPILER_ID}/h4tl_patch_asm.s
${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_C_COMPILER_ID}/h4tl_read_start_patch_asm.s
${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_C_COMPILER_ID}/task_id_patch_asm.s
${CMAKE_CURRENT_SOURCE_DIR}/llm_end_evt_defer_patch.c
${CMAKE_CURRENT_SOURCE_DIR}/llm_stop_ind_patch.c
${CMAKE_CURRENT_SOURCE_DIR}/h4tl_read_start_skip_patch.c
${CMAKE_CURRENT_SOURCE_DIR}/task_id_patch.c

)

add_sources(PLF_00_SRCS ${SRC})

target_include_directories(inc_00
INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)