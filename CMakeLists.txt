cmake_minimum_required(VERSION 3.20)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_C_COMPILER riscv64-unknown-elf-gcc)
set(CMAKE_CXX_COMPILER riscv64-unknown-elf-g++)
set(CMAKE_ASM_COMPILER riscv64-unknown-elf-gcc)

project(qemu_riscv_embedded LANGUAGES C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXECUTABLE_SUFFIX "")
set(CMAKE_C_STANDARD_LIBRARIES "")
set(CMAKE_CXX_STANDARD_LIBRARIES "")
set(CMAKE_EXE_LINKER_FLAGS "")

set(CMAKE_OBJDUMP riscv64-unknown-elf-objdump)
set(CMAKE_SIZE riscv64-unknown-elf-size)
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/scripts/linker.ld)

set(KERNEL_SOURCE_DIRS
    ${CMAKE_SOURCE_DIR}/boot
    ${CMAKE_SOURCE_DIR}/drivers
    ${CMAKE_SOURCE_DIR}/kernel
    ${CMAKE_SOURCE_DIR}/platform
    ${CMAKE_SOURCE_DIR}/secure
    ${CMAKE_SOURCE_DIR}/user
)

set(KERNEL_SOURCES)
set(KERNEL_INCLUDE_DIRS ${CMAKE_SOURCE_DIR})

foreach(dir IN LISTS KERNEL_SOURCE_DIRS)
    file(GLOB_RECURSE DIR_SOURCES CONFIGURE_DEPENDS
        ${dir}/*.c
        ${dir}/*.cc
        ${dir}/*.cpp
        ${dir}/*.S
        ${dir}/*.s
    )
    list(APPEND KERNEL_SOURCES ${DIR_SOURCES})

    file(GLOB_RECURSE DIR_PATHS CONFIGURE_DEPENDS LIST_DIRECTORIES true ${dir}/*)
    foreach(path IN LISTS DIR_PATHS)
        if(IS_DIRECTORY "${path}")
            list(APPEND KERNEL_INCLUDE_DIRS ${path})
        endif()
    endforeach()
endforeach()

list(FILTER KERNEL_SOURCES EXCLUDE REGEX ".*/boot/openSBI/.*")
list(FILTER KERNEL_SOURCES EXCLUDE REGEX ".*/build/.*")
list(FILTER KERNEL_SOURCES EXCLUDE REGEX ".*/tests/.*")
list(FILTER KERNEL_SOURCES EXCLUDE REGEX ".*/tools/.*")
list(SORT KERNEL_SOURCES)

list(FILTER KERNEL_INCLUDE_DIRS EXCLUDE REGEX ".*/boot/openSBI(/.*)?$")
list(FILTER KERNEL_INCLUDE_DIRS EXCLUDE REGEX ".*/build(/.*)?$")
list(FILTER KERNEL_INCLUDE_DIRS EXCLUDE REGEX ".*/tests(/.*)?$")
list(FILTER KERNEL_INCLUDE_DIRS EXCLUDE REGEX ".*/tools(/.*)?$")
list(REMOVE_DUPLICATES KERNEL_INCLUDE_DIRS)
list(SORT KERNEL_INCLUDE_DIRS)

add_executable(kernel.elf ${KERNEL_SOURCES})

set_target_properties(kernel.elf PROPERTIES
    SUFFIX ""
    LINK_FLAGS "-nostdlib -T${LINKER_SCRIPT} -Wl,-Map=${CMAKE_BINARY_DIR}/kernel.map"
)

target_include_directories(kernel.elf PRIVATE
    ${KERNEL_INCLUDE_DIRS}
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

option(BUILD_HOST_TESTS "Build host-side unit tests" OFF)
if(BUILD_HOST_TESTS)
    add_subdirectory(tests)
endif()
