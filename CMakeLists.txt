cmake_minimum_required(VERSION 3.20)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_C_COMPILER riscv64-unknown-elf-gcc)
set(CMAKE_CXX_COMPILER riscv64-unknown-elf-g++)
set(CMAKE_ASM_COMPILER riscv64-unknown-elf-gcc)

project(qemu_riscv_embedded LANGUAGES C CXX ASM)

set(CMAKE_EXECUTABLE_SUFFIX "")
set(CMAKE_C_STANDARD_LIBRARIES "")
set(CMAKE_CXX_STANDARD_LIBRARIES "")
set(CMAKE_EXE_LINKER_FLAGS "")

set(CMAKE_OBJDUMP riscv64-unknown-elf-objdump)
set(CMAKE_SIZE riscv64-unknown-elf-size)
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/scripts/linker.ld)

add_executable(kernel.elf
    boot/entry.S
    kernel/kernel.cpp
    drivers/uart/uart.cpp
)

set_target_properties(kernel.elf PROPERTIES
    SUFFIX ""
    LINK_FLAGS "-nostdlib -T${LINKER_SCRIPT} -Wl,-Map=${CMAKE_BINARY_DIR}/kernel.map"
)

target_include_directories(kernel.elf PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/drivers
    ${CMAKE_SOURCE_DIR}/kernel
    ${CMAKE_SOURCE_DIR}/platform
)

target_compile_options(kernel.elf PRIVATE
    -ffreestanding
    -mcmodel=medany
    $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-unwind-tables>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-asynchronous-unwind-tables>
    $<$<COMPILE_LANGUAGE:ASM>:-x$<SEMICOLON>assembler-with-cpp>
)

add_custom_command(TARGET kernel.elf POST_BUILD
    COMMAND ${CMAKE_OBJDUMP} -d $<TARGET_FILE:kernel.elf> > ${CMAKE_BINARY_DIR}/kernel.disasm
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:kernel.elf>
    COMMENT "Generating disassembly and size report"
)
