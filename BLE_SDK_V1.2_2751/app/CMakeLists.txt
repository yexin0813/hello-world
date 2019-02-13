set(FW_USR_00_SCATTER_FILE_PATH ${PROJECT_SOURCE_DIR}/plf/apollo_00/config/linker_script/${CMAKE_C_COMPILER_ID}/fw_usr.txt)

macro(app_target_build tgt)
get_property(PLF_00_SRCS_LIST GLOBAL PROPERTY PLF_00_SRCS)
get_property(USR_SRCS_LIST GLOBAL PROPERTY USR_SRCS)
include_directories(./)
add_executable(${tgt} ${PLF_00_SRCS_LIST} ${USR_SRCS_LIST} ${ARGN})
add_dependencies(${tgt} reg_headers)
target_compile_definitions(${tgt}
PUBLIC HW_BX_VERSION=00 
)
target_link_libraries(${tgt} ${PROJECT_SOURCE_DIR}/plf/apollo_00/config/rom_syms_${CMAKE_C_COMPILER_ID}.txt fw_inc usr_inc inc_00 ${link_gc_option} ${linkscript_option}${FW_USR_00_SCATTER_FILE_PATH})
set_target_properties(${tgt} PROPERTIES
LINK_DEPENDS ${FW_USR_00_SCATTER_FILE_PATH}
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/
)

add_custom_command(TARGET ${tgt} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${tgt}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/output/
)

if(CMAKE_C_COMPILER_ID STREQUAL ARMCC)
add_custom_command(TARGET ${tgt} POST_BUILD
    COMMAND fromelf -c -a -d -e -v -o ${tgt}.asm ../${tgt}.elf
    COMMAND fromelf --bin --output=binary ../${tgt}.elf
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/output/${tgt}
    COMMENT "Disassembling elf to asm && Converting elf to binary file into folder binary"
)
else()
add_custom_command(TARGET ${tgt} POST_BUILD
    COMMAND fromelf -c -a -d -e -v -o ${tgt}.asm ../${tgt}.elf
    COMMAND ${CMAKE_COMMAND} -E make_directory binary
    COMMAND arm-none-eabi-objcopy -O binary -j RAM_CODE -j .copy.table -j .zero.table ../${tgt}.elf binary/RAM_CODE
    COMMAND arm-none-eabi-objcopy -O binary -j FLASH_CODE ../${tgt}.elf binary/FLASH_CODE
    COMMAND arm-none-eabi-objcopy -O binary -j NVDS_AREA ../${tgt}.elf binary/NVDS_AREA
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/output/${tgt}
    COMMENT "Disassembling elf to asm && Converting elf to binary file into folder binary"
)
endif()

add_custom_command(TARGET ${tgt} POST_BUILD
    COMMAND ${PROJECT_SOURCE_DIR}/tools/image_tool/bin_merge.exe ${PROJECT_SOURCE_DIR}/tools/image_tool/merge.ini binary/RAM_CODE binary/FLASH_CODE binary/NVDS_AREA ${PROJECT_SOURCE_DIR}/tools/image_tool/boot_ram.bin 
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/output/${tgt}
    COMMENT "Merging user image with boot_ram.bin to ota.bin & flash.hex"
)

endmacro(app_target_build)

add_subdirectory(freertos)
add_subdirectory(baremetal)
add_subdirectory(network_processor)
