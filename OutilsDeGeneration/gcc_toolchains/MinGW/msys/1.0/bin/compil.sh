#!/bin/bash

export  PATH="$PATH:../tools/riscv32-unknown-elf/bin/"

# riscv32-unknown-elf-gcc.exe -print-multi-lib
riscv32-unknown-elf-gcc.exe -march=rv32i -mabi=ilp32 main.c -ffreestanding -nostdlib -nostartfiles -fpic -Ttext 0 -mcmodel=medany -o main.elf
riscv32-unknown-elf-objdump.exe -d --all  main.elf > dump_ASM.txt
# riscv32-unknown-elf-objdump.exe -d --no-addresses --no-show-raw-insn main.elf > dump_ASM_Only.txt
riscv32-unknown-elf-objcopy.exe main.elf -O binary main.bin
rm main.elf