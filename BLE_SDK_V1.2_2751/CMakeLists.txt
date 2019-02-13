cmake_minimum_required(VERSION 3.7)
project(bxble C ASM)

include_directories(./)

set(HW_VERSION audio)

#set(CMAKE_VERBOSE_MAKEFILE TRUE)

if(CMAKE_C_COMPILER_ID STREQUAL ARMCC)
set(CMAKE_C_FLAGS "-O2 -Ospace -D__MICROLIB --cpu=cortex-m0+ --bss_threshold=0 --c99 --split_sections -g")
set(CMAKE_ASM_FLAGS "--cpu=cortex-m0+ --pd \"__MICROLIB SETA 1\" --thumb -g")
set(CMAKE_EXE_LINKER_FLAGS "--cpu=cortex-m0+ --library_type=microlib --map --symbols --verbose --xref --info any,architecture,exceptions,inline,inputs,libraries,merge,pltgot,sizes,stack,summarysizes,summarystack,tailreorder,totals,unused,unusedsymbols,veneers,veneercallers,veneerpools,visibility,weakrefs --callgraph --datacompressor off ")
set(linkscript_option "--scatter=")
set(partial_link_option "--partial")
set(link_gc_option "--remove")
set(sym_option symdefs)
message(${CMAKE_C_COMPILER_ID})
else()
set(CMAKE_C_FLAGS "-O2 -Os -mabi=aapcs -mthumb -mcpu=cortex-m0plus -std=c99 -g -ffunction-sections -fdata-sections -fstrict-volatile-bitfields -Wno-attributes")
set(CMAKE_ASM_FLAGS "-mthumb -mcpu=cortex-m0plus -std=c99 -g -ffunction-sections -fdata-sections -Wno-attributes")
set(CMAKE_EXE_LINKER_FLAGS " --specs=nano.specs --specs=nosys.specs")
set(linkscript_option "-T ")
set(partial_link_option "-Wl,-r")
set(link_gc_option "-Wl,--gc-sections")
set(sym_option )
set(CMAKE_EXECUTABLE_SUFFIX ".elf")
set(CMAKE_C_LINK_EXECUTABLE "<CMAKE_C_COMPILER> <FLAGS> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> -Wl,-Map=<TARGET_BASE>.map <OBJECTS> -lm -o <TARGET> <LINK_LIBRARIES>")
message(${CMAKE_C_COMPILER_ID})
endif()
#message(${CMAKE_C_LINK_EXECUTABLE})
#message(${CMAKE_EXECUTABLE_SUFFIX})
function(add_sources)
    set(dst ${ARGV0})
    get_property(srcs GLOBAL PROPERTY ${dst})
    list(REMOVE_ITEM ARGV ${dst})
    foreach(arg ${ARGV})
        list(APPEND srcs ${arg})
    endforeach()
    set_property(GLOBAL PROPERTY ${dst} ${srcs})
endfunction(add_sources)
add_custom_target(mk_dir_then_cp_jlinkscript
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/output/fw_usr
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/output/fw_full
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/output/fw_full_01
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/output/fw_ram
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/output/boot_rom
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/output/boot_ram
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/output/libmesh
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/tools/image_tool/JLinkSettings.JLinkScript ${CMAKE_BINARY_DIR}/
)

execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/tools/image_tool/JLinkSettings.JLinkScript ${CMAKE_BINARY_DIR}/)
add_library(fw_inc INTERFACE)
add_library(usr_inc INTERFACE)

add_library(inc_00 INTERFACE)
add_library(inc_01 INTERFACE)
set_property(GLOBAL PROPERTY ROM_00_SRCS)
set_property(GLOBAL PROPERTY PLF_00_SRCS)
set_property(GLOBAL PROPERTY USR_SRCS)
set_property(GLOBAL PROPERTY PLF_01_SRCS)
add_subdirectory(modules)
add_subdirectory(plf)
add_subdirectory(ip)
add_subdirectory(app)



get_property(ROM_00_SRCS_LIST GLOBAL PROPERTY ROM_00_SRCS)
get_property(PLF_00_SRCS_LIST GLOBAL PROPERTY PLF_00_SRCS)
get_property(USR_SRCS_LIST GLOBAL PROPERTY USR_SRCS)
get_property(PLF_01_SRCS_LIST GLOBAL PROPERTY PLF_01_SRCS)



if(CMAKE_C_COMPILER_ID STREQUAL ARMCC)
add_executable(rom_partial_00 ${ROM_00_SRCS_LIST})
add_dependencies(rom_partial_00 reg_headers)
target_compile_definitions(rom_partial_00 PUBLIC HW_BX_VERSION=00)
target_link_libraries(rom_partial_00 
PRIVATE ${partial_link_option}
PUBLIC fw_inc usr_inc inc_00)
set_target_properties(rom_partial_00 PROPERTIES
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/
)
add_executable(fw_full ${PLF_00_SRCS_LIST} ${USR_SRCS_LIST})
add_dependencies(fw_full reg_headers rom_partial_00 mk_dir_then_cp_jlinkscript)
target_compile_definitions(fw_full PUBLIC HW_BX_VERSION=00)
set(FW_FULL_00_SCATTER_FILE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/plf/apollo_00/config/linker_script/${CMAKE_C_COMPILER_ID}/fw_full.txt)
target_link_libraries(fw_full fw_inc usr_inc inc_00 ${link_gc_option} ${linkscript_option}${FW_FULL_00_SCATTER_FILE_PATH} --${sym_option}=rom_syms_${CMAKE_C_COMPILER_ID}.txt ${CMAKE_BINARY_DIR}/output/rom_partial_00.elf)
set_target_properties(fw_full PROPERTIES
LINK_DEPENDS ${FW_FULL_00_SCATTER_FILE_PATH}
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/fw_full
)
add_custom_command(TARGET fw_full POST_BUILD
    COMMAND fromelf -c -a -d -e -v -o fw_full.asm fw_full.elf
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/output/fw_full
    COMMENT "Disassembling fw_full.elf to fw_full.asm"
)
add_dependencies(boot_ram mk_dir_then_cp_jlinkscript)
#add_dependencies(boot_rom mk_dir_then_cp_jlinkscript)

add_executable(fw_ram EXCLUDE_FROM_ALL ${PLF_00_SRCS_LIST} ${USR_SRCS_LIST})
add_dependencies(fw_ram reg_headers rom_partial_00 mk_dir_then_cp_jlinkscript)
target_compile_definitions(fw_ram PUBLIC HW_BX_VERSION=00 ALL_IN_RAM)
set(FW_RAM_00_SCATTER_FILE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/plf/apollo_00/config/linker_script/${CMAKE_C_COMPILER_ID}/fw_ram.txt)
target_link_libraries(fw_ram fw_inc usr_inc inc_00 ${link_gc_option} ${linkscript_option}${FW_RAM_00_SCATTER_FILE_PATH} ${CMAKE_BINARY_DIR}/output/rom_partial_00.elf)
set_target_properties(fw_ram PROPERTIES
LINK_DEPENDS ${FW_RAM_00_SCATTER_FILE_PATH}
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/fw_ram
)
add_custom_command(TARGET fw_ram POST_BUILD
    COMMAND fromelf -c -a -d -e -v -o fw_ram.asm fw_ram.elf
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/output/fw_ram
    COMMENT "Disassembling fw_ram.elf to fw_ram.asm"
)
add_dependencies(fw_ram mk_dir_then_cp_jlinkscript)
else()
add_executable(rom_partial_01 ${ROM_00_SRCS_LIST})
add_dependencies(rom_partial_01 reg_headers)
target_compile_definitions(rom_partial_01 PUBLIC HW_BX_VERSION=01)
target_link_libraries(rom_partial_01 
PRIVATE ${partial_link_option} "-nostartfiles"
PUBLIC fw_inc usr_inc usr_inc_01 inc_01)
set_target_properties(rom_partial_01 PROPERTIES
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/
)
add_executable(fw_full_01 ${PLF_01_SRCS_LIST} ${USR_SRCS_LIST})
add_dependencies(fw_full_01 rom_partial_01 mk_dir_then_cp_jlinkscript)
target_compile_definitions(fw_full_01 PUBLIC HW_BX_VERSION=01)
set(FW_FULL_01_SCATTER_FILE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/plf/apollo_01/config/linker_script/${CMAKE_C_COMPILER_ID}/fw_full.txt)
target_link_libraries(fw_full_01 fw_inc usr_inc usr_inc_01 inc_01 ${link_gc_option} ${linkscript_option}${FW_FULL_01_SCATTER_FILE_PATH} ${CMAKE_BINARY_DIR}/output/rom_partial_01.elf)
set_target_properties(fw_full_01 PROPERTIES
LINK_DEPENDS ${FW_FULL_01_SCATTER_FILE_PATH}
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/fw_full_01
)
add_custom_command(TARGET fw_full_01 POST_BUILD
    COMMAND fromelf -c -a -d -e -v -o fw_full_01.asm fw_full_01.elf
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/output/fw_full_01
    COMMENT "Disassembling fw_full_01.elf to fw_full_01.asm"
)
endif()

add_executable(fw_usr ${PLF_00_SRCS_LIST} ${USR_SRCS_LIST})
add_dependencies(fw_usr reg_headers mk_dir_then_cp_jlinkscript)
target_compile_definitions(fw_usr PUBLIC HW_BX_VERSION=00)
set(FW_USR_00_SCATTER_FILE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/plf/apollo_00/config/linker_script/${CMAKE_C_COMPILER_ID}/fw_usr.txt)
target_link_libraries(fw_usr ${CMAKE_CURRENT_SOURCE_DIR}/plf/apollo_00/config/rom_syms_${CMAKE_C_COMPILER_ID}.txt fw_inc usr_inc inc_00 ${link_gc_option} ${linkscript_option}${FW_USR_00_SCATTER_FILE_PATH})
set_target_properties(fw_usr PROPERTIES
LINK_DEPENDS ${FW_USR_00_SCATTER_FILE_PATH}
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/fw_usr
)
if(CMAKE_C_COMPILER_ID STREQUAL ARMCC)
add_custom_command(TARGET fw_usr POST_BUILD
    COMMAND fromelf -c -a -d -e -v -o fw_usr.asm fw_usr.elf
    COMMAND fromelf --bin --output=binary fw_usr.elf
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/output/fw_usr
    COMMENT "Disassembling fw_usr.elf to fw_usr.asm && Converting fw_usr.elf to binary file into folder binary"
)
else()
add_custom_command(TARGET fw_usr POST_BUILD
    COMMAND fromelf -c -a -d -e -v -o fw_usr.asm fw_usr.elf
    COMMAND ${CMAKE_COMMAND} -E make_directory binary
    COMMAND arm-none-eabi-objcopy -O binary -j RAM_CODE -j .copy.table -j .zero.table fw_usr.elf binary/RAM_CODE
    COMMAND arm-none-eabi-objcopy -O binary -j FLASH_CODE fw_usr.elf binary/FLASH_CODE
    COMMAND arm-none-eabi-objcopy -O binary -j NVDS_AREA fw_usr.elf binary/NVDS_AREA
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/output/fw_usr
    COMMENT "Disassembling fw_usr.elf to fw_usr.asm && Converting fw_usr.elf to binary file into folder binary"
)
endif()


add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/output/ota.bin ${CMAKE_BINARY_DIR}/output/flash.hex
    COMMAND ${PROJECT_SOURCE_DIR}/tools/image_tool/bin_merge.exe ${PROJECT_SOURCE_DIR}/tools/image_tool/merge.ini fw_usr/binary/RAM_CODE fw_usr/binary/FLASH_CODE fw_usr/binary/NVDS_AREA ${PROJECT_SOURCE_DIR}/tools/image_tool/boot_ram.bin 
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/output
    DEPENDS fw_usr ${PROJECT_SOURCE_DIR}/tools/image_tool/merge.ini ${PROJECT_SOURCE_DIR}/tools/image_tool/boot_ram.bin 
    COMMENT "Merging fw_usr.bin with boot_ram.bin to ota.bin & flash.hex"
)
add_custom_target(flash_img DEPENDS ${CMAKE_BINARY_DIR}/output/ota.bin ${CMAKE_BINARY_DIR}/output/flash.hex)











