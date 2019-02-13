set(modules_list
ke
dbg
rwip
common
nvds
rf
ecc_p256
h4tl
#app
#display
bxfs
)
foreach(module ${modules_list})
    add_subdirectory(${module})
endforeach(module)