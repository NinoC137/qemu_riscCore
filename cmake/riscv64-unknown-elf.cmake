set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR riscv64)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(TOOLCHAIN_BIN_DIR "/opt/homebrew/opt/riscv-gnu-toolchain/bin" CACHE PATH "Directory containing the RISC-V ELF toolchain binaries")

set(CMAKE_C_COMPILER   "${TOOLCHAIN_BIN_DIR}/riscv64-unknown-elf-gcc")
set(CMAKE_CXX_COMPILER "${TOOLCHAIN_BIN_DIR}/riscv64-unknown-elf-g++")
set(CMAKE_ASM_COMPILER "${TOOLCHAIN_BIN_DIR}/riscv64-unknown-elf-gcc")

set(CMAKE_OBJDUMP "${TOOLCHAIN_BIN_DIR}/riscv64-unknown-elf-objdump" CACHE FILEPATH "objdump executable")
set(CMAKE_SIZE    "${TOOLCHAIN_BIN_DIR}/riscv64-unknown-elf-size" CACHE FILEPATH "size executable")

set(CMAKE_C_COMPILER_WORKS TRUE CACHE INTERNAL "")
set(CMAKE_CXX_COMPILER_WORKS TRUE CACHE INTERNAL "")
set(CMAKE_ASM_COMPILER_WORKS TRUE CACHE INTERNAL "")
