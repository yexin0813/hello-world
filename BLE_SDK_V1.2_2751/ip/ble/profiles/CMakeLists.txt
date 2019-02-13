set(PROFILES_LIST
anp
bas
blp
cpp
cscp
dis
find
glp
hogp
hrp
htp
lan
pasp
prox
rscp
scpp
tip
bxota
anc
)
macro(profile_add prf)
    set(${prf}_src_dir ${CMAKE_CURRENT_SOURCE_DIR}/${${prf}_dir}/src)
    set(${prf}_src 
    ${${prf}_src_dir}/${prf}.c 
    ${${prf}_src_dir}/${prf}_task.c
    )    
    add_sources(USR_SRCS ${${prf}_src})
    target_include_directories(usr_inc
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/${${prf}_dir}/api)
endmacro(profile_add)

foreach(PROFILE ${PROFILES_LIST})
    add_subdirectory(${PROFILE})
    target_include_directories(usr_inc
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/${PROFILE})
endforeach(PROFILE)