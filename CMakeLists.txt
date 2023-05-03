cmake_minimum_required(VERSION 2.8.9)
project(aft-hal LANGUAGES C CXX)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_C_COMPILER riscv64-unknown-elf-gcc)
set(CMAKE_CXX_COMPILER riscv64-unknown-elf-g++)

add_compile_options(-g -march=rv32imc -mabi=ilp32 -mcmodel=medany -static -O1 -ffreestanding -nostartfiles -ffunction-sections -fdata-sections -fno-threadsafe-statics)
add_link_options("SHELL:-T ${CMAKE_CURRENT_SOURCE_DIR}/src/link.ld" -march=rv32imc -mabi=ilp32 -mcmodel=medany -static -nostdlib -ffreestanding -nostartfiles -ffunction-sections -fdata-sections)
#add_link_options("LINKER:-T ${CMAKE_CURRENT_SOURCE_DIR}/src/link.ld")
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

add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/CMakeFiles
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/CMakeTmp
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/cmake_install.cmake
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/CMakeCache.txt
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/Makefile
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/app
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/*.bin
    COMMENT "Cleaning up CMake-generated files"
)

