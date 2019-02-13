file(STRINGS config/reglist_ble.txt reglist_ble REGEX ^[^\#].+$)
foreach(line ${reglist_ble})
    separate_arguments(argv UNIX_COMMAND ${line})
    list(LENGTH argv len)
    if(${len})
        list(GET argv 0 xlsname_raw)
        string(REPLACE $HW_VERSION ${HW_VERSION} xlsname_relative ${xlsname_raw})
        set(xlsname ${CMAKE_CURRENT_SOURCE_DIR}/${xlsname_relative})
        list(GET argv 1 blockname)
        string(TOLOWER ${blockname} blockname_lower)
        list(GET argv 2 regaddr)
        list(GET argv 3 regaccess)
        list(GET argv 4 regprefix)
        list(GET argv 5 regformat)
        if(${len} EQUAL 6)
            set(sheetname Registers)
            set(hname reg_${blockname_lower}.h)
        elseif(${len} EQUAL 7)
            list(GET argv 6 sheetname)
            string(TOLOWER ${sheetname} sheetname_lower)
            set(hname reg_${blockname_lower}_${sheetname_lower}.h)
        endif()
        set(output_dir ${CMAKE_CURRENT_SOURCE_DIR}/src/build/ble-full/reg/fw)
        add_custom_command(
            OUTPUT ${output_dir}/${hname} ${output_dir}/_${hname}
            COMMAND python ${PROJECT_SOURCE_DIR}/tools/reg_xls2h/gen.py
            ${xlsname} ${blockname} ${regaddr} ${regaccess} ${regprefix} ${regformat} ${sheetname} ${output_dir}/${hname}    
            DEPENDS config/reglist_ble.txt
        ) 
        list(APPEND ble_regs ${output_dir}/${hname} ${output_dir}/_${hname})
    endif(${len})
endforeach(line)
add_custom_target(reg_headers DEPENDS ${ble_regs})
add_subdirectory(src)
if(CMAKE_C_COMPILER_ID STREQUAL ARMCC)
add_subdirectory(bootloader/boot_rom)
add_subdirectory(bootloader/boot_ram)
endif()

target_include_directories(usr_inc
INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

