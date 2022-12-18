cmake_minimum_required(VERSION 2.8.9)
project(aft-hal LANGUAGES C CXX)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_C_COMPILER riscv64-unknown-elf-gcc)
set(CMAKE_CXX_COMPILER riscv64-unknown-elf-g++)

add_compile_options(-g -march=rv32imc -mabi=ilp32 -mcmodel=medany -static -O1 -ffreestanding -nostartfiles -ffunction-sections -fdata-sections -fno-threadsafe-statics)
#add_link_options("LINKER:--gc-sections" "SHELL:-T src/link.ld" -march=rv32imc -mabi=ilp32 -mcmodel=medany -static -nostdlib -ffreestanding -nostartfiles -ffunction-sections -fdata-sections)
add_link_options("SHELL:-T src/link.ld" -march=rv32imc -mabi=ilp32 -mcmodel=medany -static -nostdlib -ffreestanding -nostartfiles -ffunction-sections -fdata-sections)
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)

include_directories(INCLUDES "src/include/")

add_executable(app
    ${INCLUDES}
    src/alloc.cpp
    src/crt0.c
    src/gpio.cpp
    src/hal.cpp
    src/int.c
    src/intmgr.cpp
    src/stdlib.cpp
    test/main.cpp
)

add_custom_command(
    TARGET app
    POST_BUILD
    COMMAND riscv64-unknown-elf-objcopy
    ARGS -O binary $<TARGET_FILE:app> meminit.bin
)
