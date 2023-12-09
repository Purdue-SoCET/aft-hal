

set(CMAKE_C_COMPILER riscv64-unknown-elf-gcc)
set(CMAKE_CXX_COMPILER riscv64-unknown-elf-g++)

cmake_minimum_required(VERSION 3.13)
project(aftx07-tests)

enable_language(ASM)
include(CMakePrintHelpers)

set(COMMON_COMPILE_OPTIONS
    -march=rv32imc_zicsr_zifencei
    -mabi=ilp32
    -mcmodel=medany
    -static
    -nostdlib
    -nostartfiles
    -ffreestanding
    -ffunction-sections
    -fdata-sections
    -Wall
    -Wextra
    -pedantic
)

# usage: -DDEBUG=true|false
if (DEBUG)
    set(COMMON_COMPILE_OPTIONS
        ${COMMON_COMPILE_OPTIONS}
        -D DEBUG
    )
endif()
# usage: -DSYNTHESIS=true/false
if (SYNTHESIS)
    set(COMMON_COMPILE_OPTIONS
        ${COMMON_COMPILE_OPTIONS}
        -D SYNTHESIS
    )
endif()

set(LINK_OPTIONS
    "-T${CMAKE_CURRENT_SOURCE_DIR}/src/link.ld"
    -Wl,--gc-sections
    -Wl,--warn-constructors
)

add_compile_options(${COMMON_COMPILE_OPTIONS})
add_link_options(${COMMON_COMPILE_OPTIONS} ${LINK_OPTIONS})

file(GLOB LIB_SOURCE src/*.c src/*.S src/*.cpp) #support/hal/pwm.cpp)# support/hal/*.cpp)
#gpio, pwm, I/O mux works
file(GLOB LIB_HEADER src/include/pal.h include/*.h)
#add_library(FemtoKernel STATIC ${LIB_SOURCE} ${LIB_HEADER})

cmake_print_variables(LIB_SOURCE)


file(GLOB TEST pwm_test.cpp) //change the name of the test file if you want
foreach( filename ${TESTS} )
    get_filename_component(targetname ${filename} NAME_WE)
    add_executable(${targetname} ${filename} ${LIB_SOURCE} ${LIB_HEADER})
    target_include_directories(${targetname} PUBLIC support support/hal) 
    set_target_properties(
        ${targetname}
        PROPERTIES
            SUFFIX ".elf"
    )

    add_custom_command(
        TARGET ${targetname}
        POST_BUILD
        COMMAND riscv64-unknown-elf-objcopy -O binary ${targetname}.elf meminit.bin
        COMMAND riscv64-unknown-elf-objdump -d ${targetname}.elf > ${targetname}.dump
        COMMENT "Generating binary file and objdump"
        # Removing existing binary in ~/AFTx07
        COMMAND ${CMAKE_COMMAND} -E remove ~/AFTx07/meminit.bin
        
        # Copying new binary to ~/AFTx07
        COMMAND ${CMAKE_COMMAND} -E copy
                ${PROJECT_BINARY_DIR}/meminit.bin
                ~/AFTx07/meminit.bin
        
        # Executing command in the destination directory and simulat on Verilator with trace enable command
        COMMAND ${CMAKE_COMMAND} -E chdir ~/AFTx07
                ./aft_out/sim-verilator/Vaftx07 --trace-enabled
    )
    #gtkwave waveform.fst // command to open the GTK waveform

endforeach()
add_custom_target(CopyFileTarget ALL
                  DEPENDS ~/AFTx07)
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
