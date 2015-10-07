project(vg_examples C)

set(CMAKE_C_FLAGS "-std=c99 -ggdb3")

add_executable(uninit_read uninit_read.c)
add_executable(oob_access oob_access.c)
add_executable(use_after_free use_after_free.c)
add_executable(leak leak.c)
add_executable(static static.c)

