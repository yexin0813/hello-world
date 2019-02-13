set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
#set(CMAKE_FIND_ROOT_PATH "C:\\Program Files (x86)\\GNU Tools ARM Embedded\\5.2 2015q4\\arm-none-eabi")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY) # For toolchains that do not support linking binaries without custom flags or scripts one may set the CMAKE_TRY_COMPILE_TARGET_TYPE variable to STATIC_LIBRARY to tell CMake not to try to link executables during its checks.