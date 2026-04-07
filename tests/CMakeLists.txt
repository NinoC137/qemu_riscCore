cmake_minimum_required(VERSION 3.20)
project(qemu_riscv_embedded_tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(trap_unit_tests
    trap/trap_unit_tests.cpp
)

target_include_directories(trap_unit_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/..
)

enable_testing()
add_test(NAME trap_unit_tests COMMAND trap_unit_tests)
