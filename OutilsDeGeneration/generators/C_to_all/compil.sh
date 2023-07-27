#!/bin/bash
# Export gcc path
export  PATH="$PATH:../../gcc_toolchains/riscv32-unknown-elf/bin/"

# Variable initialization
c_file_name="main"
bin_asm_path="output/bin_asm"
bin_file=$bin_asm_path/$c_file_name".bin"
asm_file=$bin_asm_path/$c_file_name".asm"

vhd_path="output/vhd"
instr8_file=$vhd_path/"InstructionMemory8.vhd"
instr32_file=$vhd_path/"InstructionMemory32.vhd"
dataMem_file=$vhd_path/"DataMemory.vhd"
testbench_file=$vhd_path/"TestBench.vhd"

VHDL_path="../../VHDL"
#async_proj_path=$VHDL_path/"PROC-ECE_ASYNC"
async_proj_path=$VHDL_path/"PROC-ECE_SYNC SDRAM"
core_only_proj_path=$VHDL_path/"PROC-ECE_CORE_ONLY"
sync_proj_path=$VHDL_path/"PROC-ECE_SYNC SDRAM"

hex_path="output/hex"
hex_ram_file=$hex_path/"RAMhex" # No extension here, the DataMemGenerator.exe will use this name to generate 4 different files with this base name
hex_instr_file=$hex_path/"IMhex.hex"

# Check if the different directories exist
if [[ ! -d "output" ]]
then
    echo "$vhd_path created."
    mkdir output
    mkdir $vhd_path
    mkdir $hex_path
    mkdir $bin_asm_path
fi

# Generate linker file based on vectors.s settings
 riscv32-unknown-elf-as -march=rv32i -mabi=ilp32 res/vectors.s -o vectors.o
# Command to generate assembly test sequence
#riscv32-unknown-elf-as -march=rv32i -mabi=ilp32 res/unitTest.s -o vectors.o
#riscv32-unknown-elf-as -march=rv32i -mabi=ilp32 res/storeLoadTest.s -o vectors.o

# Compile C code in input using the GCC toolchains
# riscv32-unknown-elf-gcc -march=rv32i -mabi=ilp32 -nostdlib -ffreestanding  -Wall -g -c -Os input/$c_file_name.c -o $c_file_name.o
riscv32-unknown-elf-gcc -march=rv32i -mabi=ilp32 -nostdlib -ffreestanding  -Wall -g -c -O1 input/$c_file_name.c -o $c_file_name.o


# riscv32-unknown-elf-gcc -march=rv32i -mabi=ilp32 -nostdlib -ffreestanding -O3 -T res/memmap res/vectors.s input/$c_file_name.c -o $c_file_name.elf
# Use the linker file and C output object to generate elf file

riscv32-unknown-elf-ld vectors.o -nostdlib $c_file_name.o -T res/memmap -o $c_file_name.elf

#riscv32-unknown-elf-ld vectors.o  $c_file_name.o -T res/memmap -o $c_file_name.elf
# Dump assembler file
riscv32-unknown-elf-objdump -S -D $c_file_name.elf > $asm_file
echo ".asm file generated"

# Output the binary file of the .elf
riscv32-unknown-elf-objcopy $c_file_name.elf -O binary $bin_file
echo ".bin file generated"

# Remove files from the process
rm $c_file_name.elf $c_file_name.o vectors.o

# RAM's size in hexadecimal
hex_ram_size="1000"

# Execute all generators of memory files
./res/intelHexGenerator.exe $hex_ram_size $bin_file $hex_ram_file $hex_instr_file
echo ".hex memory files generated"
./res/DataMemGenerator.exe $hex_ram_size $bin_file $instr8_file $instr32_file $dataMem_file
echo ".vhd memory files generated"

# Execute the test bench generator possible arguments :
# -excel : Activate excel report generation
# -reg : Add register checking at each rising clock
# -mem : Generate assert on read and write in memories, to use when you simulate the whole system with external mem
# -async : Specify two clocks are required to r/w in memories

# Fix a limit of iteration in simulation and test bench generation to avoid never ending program
breakpoint="200"

./res/testbenchGenerator.exe $bin_file $testbench_file $breakpoint -excel -mem -reg #TestBench ?
echo ".vhd testbench file generated"

echo "**** SIMULATION ****"
# Launch the simulation
./res/simulator $bin_file $breakpoint

# Ask user if he want to copy files directly in the async project
while : ; do #
    read -n1 -p "Copy files in ASYNC PROJECT? [Y/n] " response
    echo
    case "$response" in
        y|Y|"")
            #cp -v $testbench_file $async_proj_path/vhdl_files
            cp -v $hex_instr_file $async_proj_path
            cp -v $hex_instr_file $async_proj_path/RAMhex32.hex
            break ;;
        n|N) echo -e "canceled"; break ;;
        *) echo "Invalid option given." ;;
    esac
done


