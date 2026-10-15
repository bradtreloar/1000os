#!/bin/bash
set -xue

# Need to use clang as C compiler because we depend on some builtins.
CC=clang

# -std=c11
#   Use C11 standard.
# -O2
#   Enable optimizations to generate efficient machine code.
# -g3
#   Generate the maximum amount of debug information.
# -Wall
#   Enable major warnings.
# -Wextra
#   Enable additional warnings.
# --target=riscv32-unknown-elf
#   Compile for 32-bit RISC-V.
# -ffreestanding
#   Do not use the standard library of the host environment.
# -fuse-ld=lld
#   Use LLVM linker (ld.lld).
# -fno-stack-protector
#   Disable unnecessary stack protection to avoid unexpected behavior in stack
#   manipulation.
# -nostdlib
#   Do not link the standard library.
COMPILER_FLAGS="-std=c11 -O2 -g3 -Wall -Wextra --target=riscv32-unknown-elf -fuse-ld=lld -fno-stack-protector -ffreestanding -nostdlib"

# -Wl,-Tkernel.ld
#   Linker script: kernel.ld.
# -Wl,-Map=kernel.map
#   Output a map file (linker allocation result): kernel.map
LINKER_FLAGS="-Wl,-Tkernel.ld -Wl,-Map=kernel.map"

# Build the kernel ELF binary.
$CC $COMPILER_FLAGS $LINKER_FLAGS -o kernel.elf \
    kernel.c common.c

# Start RISC-V VM in QEMU.
# Use the default BIOS: OpenSBI
# Connect VM's serial port to QEMU monitor's stdout.
# Don't try to reboot if the machine crashes.
qemu-system-riscv32 -machine virt -bios default -nographic -serial mon:stdio --no-reboot \
    -kernel kernel.elf
