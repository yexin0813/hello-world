set(MAIN_SRC 
${CMAKE_CURRENT_SOURCE_DIR}/main.c
)

set_property(GLOBAL PROPERTY RTOS_APP_SRCS)
add_library(rtos_app_inc INTERFACE)
macro(rtos_app_target_build tgt)
get_property(RTOS_APP_SRCS_LIST GLOBAL PROPERTY RTOS_APP_SRCS)
app_target_build(${tgt} ${ARGN} ${RTOS_APP_SRCS_LIST} ${MAIN_SRC})
target_link_libraries(${tgt} rtos_app_inc)
endmacro(rtos_app_target_build)

target_include_directories(rtos_app_inc
INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(src)
add_subdirectory(utils)
add_subdirectory(ble)
add_subdirectory(mesh)


if(CMAKE_C_COMPILER_ID STREQUAL ARMCC)
include_directories( ./mesh/examples/simple_generic_onoff_server)
include_directories( ./utils)
include_directories( ./src/include)
include_directories( ./)
include_directories( ./src/portable/ARMCC/ARM_CM0)
aux_source_directory(./mesh/core/ LIBSRC)
aux_source_directory(./mesh/provision/ LIBSRC)


add_library(mesh_static STATIC ${LIBSRC})

set_target_properties(mesh_static PROPERTIES
ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output/libmesh/
)
target_compile_definitions(mesh_static PUBLIC B_APP_MESH_SIMPLE_GENERIC_ONOFF_SERVER)
target_link_libraries(mesh_static 
PUBLIC  fw_inc usr_inc inc_00 mesh_inc)

add_custom_command(TARGET mesh_static POST_BUILD 
COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/output/libmesh/libmesh_static.a ${PROJECT_SOURCE_DIR}/app/freertos/mesh/lib/libmesh_static.lib)

endif()
