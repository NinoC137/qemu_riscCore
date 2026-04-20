cmake_minimum_required(VERSION 3.20)
project(qemu_riscv_embedded_tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(trap_unit_tests
    trap/trap_unit_tests.cpp
    host_stubs/uart_stub.cpp
    ../kernel/arch/riscv/syscall/syscall.cpp
    ../kernel/arch/riscv/syscall/sys_write.cpp
    ../kernel/arch/riscv/syscall/sys_exit.cpp
    ../kernel/arch/riscv/syscall/sys_yield.cpp
)

target_include_directories(trap_unit_tests PRIVATE
    ${CMAKE_SOURCE_DIR}
)

add_executable(list_unit_tests
    ds/list_unit_tests.cpp
    ../kernel/lib/ds/list.cpp
)

target_include_directories(list_unit_tests PRIVATE
    ${CMAKE_SOURCE_DIR}
)

add_executable(queue_unit_tests
    ds/queue_unit_tests.cpp
    ../kernel/lib/ds/list.cpp
    ../kernel/lib/ds/queue.cpp
)

target_include_directories(queue_unit_tests PRIVATE
    ${CMAKE_SOURCE_DIR}
)

enable_testing()
add_test(NAME trap_unit_tests COMMAND trap_unit_tests)
add_test(NAME list_unit_tests COMMAND list_unit_tests)
add_test(NAME queue_unit_tests COMMAND queue_unit_tests)
