add_definitions(-DNDEBUG)
include_directories(
../../../../app/config
../../../../ip
../../../inc/cmsis/
../../../
../../
../
./ 
)

add_definitions(-D__BOOT_RAM__)
add_executable(boot_ram
boot_ram.c
flash.c
dma_for_qspi.c
qspi.c
#flash_cache.c
#clk_gate.c
#rst_gen.c
)
add_dependencies(boot_ram reg_headers)
set(BOOT_RAM_SCATTER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_C_COMPILER_ID}/boot_ram_link.txt)
set_property(TARGET boot_ram APPEND_STRING
PROPERTY LINK_FLAGS " ${link_gc_option} --entry=boot_ram_entry ${linkscript_option}${BOOT_RAM_SCATTER_FILE}" 
)
target_compile_definitions(boot_ram PUBLIC HW_BX_VERSION=00)
set_target_properties(boot_ram PROPERTIES
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/boot_ram
)
add_custom_command(TARGET boot_ram POST_BUILD
    COMMAND fromelf -c -a -d -e -v -o boot_ram.asm boot_ram.elf
    COMMAND fromelf --bin --output=boot_ram.bin boot_ram.elf
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/output/boot_ram/boot_ram.bin  ${PROJECT_SOURCE_DIR}/tools/image_tool
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/output/boot_ram/boot_ram.asm  ${PROJECT_SOURCE_DIR}/tools/image_tool
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/output/boot_ram
    COMMENT "Disassembling boot_ram.elf to boot_ram.asm && Producing boot_ram binary file: boot_ram.bin"
)