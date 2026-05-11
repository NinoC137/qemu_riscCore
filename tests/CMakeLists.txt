cmake_minimum_required(VERSION 3.20)
project(qemu_riscv_embedded_tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(trap_unit_tests
    trap/trap_unit_tests.cpp
    host_stubs/uart_stub.cpp
    host_stubs/scheduler_stub.cpp
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

add_executable(bitmap_unit_tests
    ds/bitmap_unit_tests.cpp
    ../kernel/lib/ds/bitmap.cpp
)

target_include_directories(bitmap_unit_tests PRIVATE
    ${CMAKE_SOURCE_DIR}
)

add_executable(page_unit_tests
    mm/page_unit_tests.cpp
)

target_include_directories(page_unit_tests PRIVATE
    ${CMAKE_SOURCE_DIR}
)

add_executable(pmm_unit_tests
    mm/pmm_unit_tests.cpp
    ../kernel/mm/pmm.cpp
)

target_include_directories(pmm_unit_tests PRIVATE
    ${CMAKE_SOURCE_DIR}
)

add_executable(sv39_unit_tests
    mm/sv39_unit_tests.cpp
)

target_include_directories(sv39_unit_tests PRIVATE
    ${CMAKE_SOURCE_DIR}
)

add_executable(page_table_unit_tests
    mm/page_table_unit_tests.cpp
    ../kernel/arch/riscv/mm/page_table.cpp
    host_stubs/pmm_stub.cpp
)

target_include_directories(page_table_unit_tests PRIVATE
    ${CMAKE_SOURCE_DIR}
)

enable_testing()
add_test(NAME trap_unit_tests COMMAND trap_unit_tests)
add_test(NAME list_unit_tests COMMAND list_unit_tests)
add_test(NAME queue_unit_tests COMMAND queue_unit_tests)
add_test(NAME bitmap_unit_tests COMMAND bitmap_unit_tests)
add_test(NAME page_unit_tests COMMAND page_unit_tests)
add_test(NAME pmm_unit_tests COMMAND pmm_unit_tests)
add_test(NAME sv39_unit_tests COMMAND sv39_unit_tests)
add_test(NAME page_table_unit_tests COMMAND page_table_unit_tests)
