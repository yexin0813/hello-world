add_definitions(-DNDEBUG)
include_directories(inc)
include_directories(../)
include_directories(../../)
include_directories(../../../)
include_directories(../../../../app/config)
include_directories(../../../../ip)
add_definitions(-D__BOOT_ROM__)
add_library(boot_rom_asm OBJECT startup_bootrom.s)
add_executable(boot_rom
boot_rom.c
clk_gen_rom.c
dma_for_qspi_rom.c
flash_rom.c
pshare_rom.c
qspi_rom.c
uart_rom.c
crc16ccitt.c 
$<TARGET_OBJECTS:boot_rom_asm>
)
add_dependencies(boot_rom reg_headers)
set(BOOT_CODE_SCATTER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/boot_code_scatter.txt)
target_compile_options(boot_rom PUBLIC -O2 -Otime)
set_property(TARGET boot_rom APPEND_STRING
PROPERTY LINK_FLAGS " ${link_gc_option} --scatter=${BOOT_CODE_SCATTER_FILE} --symdefs=boot_rom_syms.txt" 
)
target_compile_definitions(boot_rom PUBLIC HW_BX_VERSION=2400)
#string(REPLACE "--bss_threshold=0" " " CMAKE_C_FLAGS ${CMAKE_C_FLAGS} ) # for compatibility with old project

set_target_properties(boot_rom PROPERTIES
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/boot_rom
)
add_custom_command(TARGET boot_rom POST_BUILD
    COMMAND fromelf -c -a -d -e -v -o boot_rom.asm boot_rom.elf
    COMMAND fromelf --bin --output=boot_rom.bin boot_rom.elf
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/output/boot_rom
    COMMENT "Disassembling boot_rom.elf to boot_rom.asm && Producing boot_rom binary file: boot_rom.bin"
)