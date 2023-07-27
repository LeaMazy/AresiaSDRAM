#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
* Program binary file path as input. Then the program will execute each command
* and write the vhd file to test the command sequence.
*/

uint16_t instrCount = 0;
uint32_t instrSize = 0;
uint8_t* instrMem = NULL;
uint8_t dataMem[4096] = {0xff};
uint32_t reg[32] = {0};
uint32_t PC = 0;
uint32_t counterReg = 0;

/* bit field are purely indicative of the length used, we use bitwise mask when we fill
* the structure */

struct command
{
    uint32_t opcode : 7;
    uint32_t rd     : 5;
    uint32_t funct3 : 3;
    uint32_t rs1    : 5;
    uint32_t rs2    : 5;
    uint32_t funct7 : 7;
    uint32_t imm5   : 5;
    uint32_t imm7   : 7;
    uint32_t imm12  : 12;
    uint32_t imm20  : 20;
    uint32_t instr  : 32;
};

/* Function declaration */
void structBuilding(uint32_t instruction, struct command* instr);
void readInstrFile(const char* filepath);

void executeInstr(struct command* instr, FILE *VHDLFile);

void executeLUI(struct command* instr, FILE *VHDLFile);
void executeAUIPC(struct command* instr, FILE *VHDLFile);
void executeJAL(struct command* instr, FILE *VHDLFile);
void executeJALR(struct command* instr, FILE *VHDLFile);
void executeBRANCH(struct command* instr, FILE *VHDLFile);
void executeLOAD(struct command* instr, FILE *VHDLFile);
void executeSTORE(struct command* instr, FILE *VHDLFile);
void executeOP_IMM(struct command* instr, FILE *VHDLFile);
void executeOP(struct command* instr, FILE *VHDLFile);
void executeMISC_MEM(struct command* instr, FILE *VHDLFile);
void executeSYSTEM(struct command* instr, FILE *VHDLFile);

/* write uint8_t, uint16_t or uint32_t data in dataMem[] with
* a little endianess */
void static inline writeDataMemory(uint32_t addr, uint32_t data, uint8_t sizeInByte)
{
    if (addr < sizeof(dataMem))
    {
        switch (sizeInByte)
        {
            case 1:
                dataMem[addr] = data & 0x000000ff;
            break;

            case 2:
                dataMem[addr]   = data & 0x000000ff;
                dataMem[addr+1] = (data >> 8) & 0x000000ff;
            break;

            case 4:
                dataMem[addr]   = data & 0x000000ff;
                dataMem[addr+1] = (data >> 8) & 0x000000ff;
                dataMem[addr+2] = (data >> 16) & 0x000000ff;
                dataMem[addr+3] = (data >> 24) & 0x000000ff;
            break;
            default: break;
        }
    }
    else
    {
        if (addr == 0x80000000) 
        {
            counterReg = data;
        }
        else
        {
            printf("***OOM***");
        }
    }
}

/* Return uint8_t, uint16_t or uint32_t data store in uint32_t with
* a little endianess */
uint32_t static inline readDataMemory(uint32_t addr, uint8_t sizeInByte)
{
    uint32_t data = 0;
    
    if (addr < sizeof(dataMem))
    {
        switch (sizeInByte)
        {
            case 1:
                data = dataMem[addr];
            break;

            case 2:
                data = dataMem[addr] | (dataMem[addr+1] << 8);
            break;

            case 4:
                data = dataMem[addr]           |
                      (dataMem[addr+1] << 8)   |
                      (dataMem[addr+2] << 16)  |
                      (dataMem[addr+3] << 24);
            break;
            default: break;
        }
    }
    else
    {
        if (addr == 0x80000000) 
        {
            data = counterReg;
        }
        else
        {
            printf("***OOM***");
        }
    }
    return data;
}

/* Return uint32_t instruction with a little endianess */
uint32_t static inline readProgramMemory(uint32_t addr)
{
    uint32_t instruction = instrMem[addr]           |
                          (instrMem[addr+1] << 8)   |
                          (instrMem[addr+2] << 16)  |
                          (instrMem[addr+3] << 24);
    return instruction;
}

enum
{
    OP_LUI      = 0b0110111,
    OP_AUIPC    = 0b0010111,
    OP_JAL      = 0b1101111,
    OP_JALR     = 0b1100111,
    OP_BRANCH   = 0b1100011,
    OP_LOAD     = 0b0000011,
    OP_STORE    = 0b0100011,
    OP_OP_IMM   = 0b0010011,
    OP_OP       = 0b0110011,
    OP_MISC_MEM = 0b0001111,
    OP_SYSTEM   = 0b1110011
} OPcode_enum;

enum
{
    F3_JALR    = 0b000,
    F3_BEQ     = 0b000,
    F3_BNE     = 0b001,
    F3_BLT     = 0b100,
    F3_BGE     = 0b101,
    F3_BLTU    = 0b110,
    F3_BGEU    = 0b111,
    F3_LB      = 0b000,
    F3_LH      = 0b001,
    F3_LW      = 0b010,
    F3_LBU     = 0b100,
    F3_LHU     = 0b101,
    F3_SB      = 0b000,
    F3_SH      = 0b001,
    F3_SW      = 0b010,
    F3_ADDI    = 0b000,
    F3_SLTI    = 0b010,
    F3_SLTIU   = 0b011,
    F3_XORI    = 0b100,
    F3_ORI     = 0b110,
    F3_ANDI    = 0b111,
    F3_SLLI    = 0b001,
    F3_SRLI    = 0b101,
    F3_SRAI    = 0b101,
    F3_ADD     = 0b000,
    F3_SUB     = 0b000,
    F3_SLL     = 0b001,
    F3_SLT     = 0b010,
    F3_SLTU    = 0b011,
    F3_XOR     = 0b100,
    F3_SRL     = 0b101,
    F3_SRA     = 0b101,
    F3_OR      = 0b110,
    F3_AND     = 0b111,
    F3_FENCE   = 0b000,
    F3_FENCE_I = 0b001,
    F3_ECALL   = 0b000,
    F3_EBREAK  = 0b000,
    F3_CSRRW   = 0b001,
    F3_CSRRS   = 0b010,
    F3_CSRRC   = 0b011,
    F3_CSRRWI  = 0b101,
    F3_CSRRSI  = 0b110,
    F3_CSRRCI  = 0b111
} funct3_enum;

/* Only funct7 different from 0 are useful but we choose to enumerate all informations found in data sheet*/
enum
{
    F7_SLLI    = 0b0000000,
    F7_SRLI    = 0b0000000,
    F7_SRAI    = 0b0100000,
    F7_ADD     = 0b0000000,
    F7_SUB     = 0b0100000,
    F7_SLL     = 0b0000000,
    F7_SLT     = 0b0000000,
    F7_SLTU    = 0b0000000,
    F7_XOR     = 0b0000000,
    F7_SRL     = 0b0000000,
    F7_SRA     = 0b0100000,
    F7_OR      = 0b0000000,
    F7_AND     = 0b0000000
} funct7_enum;

void main(int argc, char** argv)
{
    /* Declare a pointer that will point on VHDL Test bench file*/
    FILE *VHDLFile;
    struct command instr;
    /* Read the file passed in argument of the script execution */
    readInstrFile(argv[1]);
    /* set breakpoint limit*/
    uint16_t breakpoint = atoi(argv[2]);
    printf("Simulation: \n\n");
    /* loop as long RA did not come back to its init value or breakpoint reached */
    do
    {
		//printf("0x%.8x\n", instrMem[PC]);
		//printf("%.2x%.2x%.2x%.2x\n", instrMem[PC+3], instrMem[PC+2], instrMem[PC+1], instrMem[PC]);
        /* print in console execution infos */
        printf("%3d : 0x%.8x : 0x%.8x : ", instrCount, PC,readProgramMemory(PC));
        /* reset struct data */
        memset(&instr, 0, sizeof(struct command));
        /* Build the structure with the instruction returned at the PC addr in instrMem[] */
        structBuilding(readProgramMemory(PC), &instr);
        /* Execute the instruction */
        executeInstr(&instr, VHDLFile);
        /* reset hard-wired zero register in case an instruction edited the register */
        reg[0] = 0;
        /* increment instruction counter to help simulation reading */
        instrCount++;
        /* add one tick to counter register */
        counterReg++;
    }
    while (PC != 8 && instrCount != breakpoint);
}

/* Function that read the instruction and split it in different variable
  * to fill the instr structure that will ease instruction execution later */
void structBuilding(uint32_t instruction, struct command* instr)
{
    /* Some value are stored in two places,
     * it's a not efficient way to have the right name later */
    instr->opcode = instruction       & 0x7f;
    instr->rd     = instruction >> 7  & 0x1f;
    instr->imm5   = instruction >> 7  & 0x1f;
    instr->imm20  = instruction >> 12 & 0xfffff;
    instr->funct3 = instruction >> 12 & 0x7;
    instr->rs1    = instruction >> 15 & 0x1f;
    instr->imm12  = instruction >> 20 & 0xfff;
    instr->rs2    = instruction >> 20 & 0x1f;
    instr->funct7 = instruction >> 25 & 0x7f;
    instr->imm7   = instruction >> 25 & 0x7f;
    instr->instr  = instruction;
}

/* Function that open a bin file with instruction in it, t
 * he file can be checked in bash terminal with xxd.exe -g 4 -u main.bin */
void readInstrFile(const char* filepath)
{
    FILE *instrFile;
    /* Open the program file in binary read mode */
    instrFile = fopen(filepath, "rb");
    /* seek the end of the file to get the size */
    fseek(instrFile, 0L, SEEK_END);
    /* store file size in chunk of uint8_t in instrSize*/
    instrSize = ftell(instrFile);
    /* Allocate instr array base on the size of the file read */
    instrMem = malloc((instrSize));
    /* reset pointer address on file */
    rewind(instrFile);
    /* copy chunk of uint8_t data from file into instrMem[] */
    fread(instrMem, sizeof(uint8_t), instrSize, instrFile);
	
    /* close file */
    fclose(instrFile);
    /* copy instrMem into dataMem */
    memcpy(dataMem, instrMem, instrSize);
	
	
}


/* Function that execute the right OPcode subroutine */
void executeInstr(struct command* instr, FILE *VHDLFile)
{
    switch(instr->opcode)
    {
        case OP_LUI:
            executeLUI(instr, VHDLFile);
        break;

        case OP_AUIPC:
            executeAUIPC(instr, VHDLFile);
        break;

        case OP_JAL:
            executeJAL(instr, VHDLFile);
        break;

        case OP_JALR:
            executeJALR(instr, VHDLFile);
        break;

        case OP_BRANCH:
            executeBRANCH(instr, VHDLFile);
        break;

        case OP_LOAD:
            executeLOAD(instr, VHDLFile);
        break;

        case OP_STORE:
            executeSTORE(instr, VHDLFile);
        break;

        case OP_OP_IMM:
            executeOP_IMM(instr, VHDLFile);
        break;

        case OP_OP:
            executeOP(instr, VHDLFile);
        break;

        case OP_MISC_MEM:
            executeMISC_MEM(instr, VHDLFile);
        break;

        case OP_SYSTEM:
            executeSYSTEM(instr, VHDLFile);
        break;
    }
}

/* Load upper immediate OPcode type subroutine */
void executeLUI(struct command* instr, FILE *VHDLFile)
{
    printf("LUI\t: reg[%.2d] = 0x%x << 12\n", instr->rd, instr->imm20);
    reg[instr->rd] = instr->imm20 << 12;
    printf("%41s reg[%.2d] = 0x%x\n", "", instr->rd, reg[instr->rd]);
    PC = PC + 4;
}

/* add upper immediate to PC OPcode type subroutine */
void executeAUIPC(struct command* instr, FILE *VHDLFile)
{
    printf("AUIPC\t: reg[%.2d] = PC + 0x%x << 12\n",instr->rd, instr->imm20);
    reg[instr->rd] = PC + (instr->imm20 << 12);
    printf("%41s reg[%.2d] = 0x%x + 0x%x = 0x%x\n", "", instr->rd, PC,(instr->imm20 << 12), reg[instr->rd]);
    PC = PC + 4;
}

/* Jump and link OPcode type subroutine */
void executeJAL(struct command* instr, FILE *VHDLFile)
{
    uint32_t offset = (( instr->imm20 >> 9  & 0x3ff) << 1  ) | // 10:0 0 forced to 0
                      (( instr->imm20 >> 8  & 0x01 ) << 11 ) | // 11
                      (( instr->imm20 >> 0  & 0xff ) << 12 ) | // 19:12
                      (( instr->imm20 >> 19 & 0x01 ) << 20 );  // 20 sign

    offset = (offset & 0x100000) ? offset| ~0x000fffff : offset & 0x000fffff;

    printf("JAL\t: reg[%.2d] = PC+4 and PC = 0x%x + %d\n", instr->rd, PC, offset);
    reg[instr->rd] = PC + 4;
    printf("%41s reg[%.2d] = 0x%x and PC = ", "", instr->rd, reg[instr->rd]);
    PC = PC + offset;
    printf("0x%x\n", PC);
}

/* Indirect Jump and link OPcode type subroutine */
void executeJALR(struct command* instr, FILE *VHDLFile)
{
    int32_t offset = (instr->imm12 & 0x800) ? instr->imm12 | ~0x000007ff : instr->imm12 & 0x000007ff;

    printf("JALR\t: reg[%.2d] = PC+4 and PC = (reg[%.2d] + %d) & ~1 \n",instr->rd, instr->rs1, offset);
    reg[instr->rd] = PC + 4;
    printf("%41s reg[%.2d] = 0x%x and PC = ", "", instr->rd, reg[instr->rd]);
    PC = reg[instr->rs1] + (offset & ~1);
    printf("0x%x\n", PC);
}

/* Branch OPcode type subroutine */
void executeBRANCH(struct command* instr, FILE *VHDLFile)
{
    int32_t offset =  (  instr->imm5 & 0x1e        )         | // 4:0 with 0 forced 0
                      (( instr->imm7 & 0x3f        ) << 5  ) | // 10:5
                      (( instr->imm5 & 0x01        ) << 11 ) | // 11
                      (((instr->imm7 & 0x40) >> 6  ) << 12 );  // 12 sign

    offset = (offset & 0x1000) ? offset | ~0x00000fff : offset & 0x00000fff;

    /* All branches work the same, if the condition is true jump to
     * PC + Immediate within 4Kib range */
    switch(instr->funct3)
    {
        case F3_BEQ: // Branch if equal
            printf("BEQ\t: if ( reg[%.2d] == reg[%.2d] ) PC = PC + %d \n", instr->rs1, instr->rs2, offset);
            printf("%41s if ( 0x%x == 0x%x ) PC = 0x%x + %d => ", "", reg[instr->rs1], reg[instr->rs2], PC, offset);
            if ( reg[instr->rs1] == reg[instr->rs2] ) PC = PC + offset;
            else PC = PC + 4;
            printf("PC = 0x%x\n", PC);
        break;

        case F3_BNE: // branch if not equal
            printf("BNE\t: if ( reg[%.2d] != reg[%.2d] ) PC = PC + %d \n", instr->rs1, instr->rs2, offset);
            printf("%41s if ( 0x%x != 0x%x ) PC = 0x%x + %d => ", "", reg[instr->rs1], reg[instr->rs2], PC, offset);
            if ( reg[instr->rs1] != reg[instr->rs2] ) PC = PC + offset;
            else PC = PC + 4;
            printf("PC = 0x%x\n", PC);
        break;

        case F3_BLT: // branch if lesser than with sign consideration
            printf("BLT\t: if ( reg[%.2d] < reg[%.2d] ) PC = PC + %d \n", instr->rs1, instr->rs2, offset);
            printf("%41s if ( %d < %d ) PC = 0x%x + %d => ", "", reg[instr->rs1], reg[instr->rs2], PC, offset);
            if ( (int32_t) reg[instr->rs1] < (int32_t) reg[instr->rs2] ) PC = PC + offset;
            else PC = PC + 4;
            printf("PC = 0x%x\n", PC);
        break;

        case F3_BGE: // branch if greater than with sign consideration
            printf("BGE\t: if ( reg[%.2d] >= reg[%.2d] ) PC = PC + %d \n", instr->rs1, instr->rs2, offset);
            printf("%41s if ( %d > %d ) PC = 0x%x + %d => ", "", reg[instr->rs1], reg[instr->rs2], PC, offset);
            if ( (int32_t) reg[instr->rs1] >= (int32_t) reg[instr->rs2] ) PC = PC + offset;
            else PC = PC + 4;
            printf("PC = 0x%x\n", PC);
        break;

        case F3_BLTU: // branch if lesser than without sign consideration
            printf("BLTU\t: if ( reg[%.2d] < reg[%.2d] ) PC = PC + %d \n", instr->rs1, instr->rs2, offset);
            printf("%41s if ( %d < %d ) PC = 0x%x + %d => ", "", reg[instr->rs1], reg[instr->rs2], PC, offset);
            if ( reg[instr->rs1] < reg[instr->rs2] ) PC = PC + offset;
            else PC = PC + 4;
            printf("PC = 0x%x\n", PC);
        break;

        case F3_BGEU: // branch if greater than without sign consideration
            printf("BGEU\t: if ( reg[%.2d] >= reg[%.2d] ) PC = PC + %d \n", instr->rs1, instr->rs2, offset);
            printf("%41s if ( %d > %d ) PC = 0x%x + %d => ", "", reg[instr->rs1], reg[instr->rs2], PC, offset);
            if ( reg[instr->rs1] >= reg[instr->rs2] ) PC = PC + offset;
            else PC = PC + 4;
            printf("PC = 0x%x\n", PC);
        break;
    }
}

/* LOAD OPcode type subroutine */
void executeLOAD(struct command* instr, FILE *VHDLFile)
{
    uint32_t data;
    uint32_t offset = (instr->imm12 & 0x800) ? instr->imm12 | ~0x000007ff : instr->imm12 & 0x000007ff;
    uint32_t addr = reg[instr->rs1] + offset;

    switch(instr->funct3)
    {
        case F3_LB: // Load int8_t
            printf("LDB\t: reg[%.2d] = dataMem[reg[%.2d] + %d]\n", instr->rd, instr->rs1, offset);
            data = readDataMemory(addr, sizeof(uint8_t));
            reg[instr->rd] = ((data & 0xff) >> 7) ? data | ~0x000000ff : data & 0x000000ff;
            printf("%41s reg[%.2d] = dataMem[0x%x] = 0x%x\n", "", instr->rd, addr, reg[instr->rd]);
        break;

        case F3_LH: // Load int16_t
            printf("LDH\t: reg[%.2d] = dataMem[reg[%.2d] + %d]\n", instr->rd, instr->rs1, offset);
            data = readDataMemory(addr, sizeof(uint16_t));
            reg[instr->rd] = ((data & 0xffff) >> 15) ? data | ~0x0000ffff : data & 0x0000ffff;
            printf("%41s reg[%.2d] = dataMem[0x%x] = 0x%x\n", "", instr->rd, addr, reg[instr->rd]);
        break;

        case F3_LW: // Load int32_t / uint32_t
            printf("LDW\t: reg[%.2d] = dataMem[reg[%.2d] + %d]\n", instr->rd, instr->rs1, offset);
            reg[instr->rd] = readDataMemory(addr, sizeof(uint32_t));
            printf("%41s reg[%.2d] = dataMem[0x%x] = 0x%x\n", "", instr->rd, addr, reg[instr->rd]);
        break;

        case F3_LBU: // Load uint8_t
            printf("LBU\t: reg[%.2d] = dataMem[reg[%.2d] + %d]\n", instr->rd, instr->rs1, offset);
            reg[instr->rd] = readDataMemory(addr, sizeof(uint8_t));
            printf("%41s reg[%.2d] = dataMem[0x%x] = 0x%x\n", "", instr->rd, addr, reg[instr->rd]);
        break;

        case F3_LHU: // Load uint16_t
            printf("LDHU\t: reg[%.2d] = dataMem[reg[%.2d] + %d]\n", instr->rd, instr->rs1, offset);
            reg[instr->rd] = readDataMemory(addr, sizeof(uint16_t));
            printf("%41s reg[%.2d] = dataMem[0x%x] = 0x%x\n", "", instr->rd, addr, reg[instr->rd]);
        break;
    }
    PC = PC + 4;
}

/* STORE OPcode type subroutine */
void executeSTORE(struct command* instr, FILE *VHDLFile)
{
    uint32_t offset = (instr->imm7 << 5) | (instr->imm5);
    offset = (offset & 0x800) ? offset | ~0x000007ff : offset & 0x000007ff;
    uint32_t addr = reg[instr->rs1] + offset;

    switch(instr->funct3)
    {
        case F3_SB: // Store a uint8_t
            printf("STRB\t: dataMem[reg[%.2d] + %d] = reg[%.2d]\n", instr->rs1, offset, instr->rs2);
            writeDataMemory(addr, reg[instr->rs2], sizeof(uint8_t));
            printf("%41s dataMem[0x%x] = 0x%x\n", "", addr, reg[instr->rs2] & 0xff);
        break;

        case F3_SH: // Store a uint16_t
            printf("STRH\t: dataMem[reg[%.2d] + %d] = reg[%.2d]\n", instr->rs1, offset, instr->rs2);
            writeDataMemory(addr, reg[instr->rs2], sizeof(uint16_t));
            printf("%41s dataMem[0x%x] = 0x%x\n", "", addr, reg[instr->rs2] & 0xffff);
        break;

        case F3_SW: // Store a uint32_t
            printf("STRW\t: dataMem[reg[%.2d] + %d] = reg[%.2d]\n", instr->rs1, offset, instr->rs2);
            writeDataMemory(addr, reg[instr->rs2], sizeof(uint32_t));
            printf("%41s dataMem[0x%x] = 0x%x\n", "", addr, reg[instr->rs2]);
        break;
    }
    PC = PC + 4;
}

/* IMM OPcode type subroutine */
void executeOP_IMM(struct command* instr, FILE *VHDLFile)
{
    uint32_t immExt = (instr->imm12 & 0x800) ? instr->imm12 | ~0x000007ff : instr->imm12 & 0x000007ff;
    uint32_t shift = instr->imm12 & 0x1f;

    switch(instr->funct3)
    {
        case F3_ADDI: // ADD with Immediate
            printf("ADDI\t: reg[%.2d] = reg[%.2d] + %d\n", instr->rd, instr->rs1, immExt);
            printf("%41s reg[%.2d] = 0x%x + %d = ", "", instr->rd, reg[instr->rs1], immExt);
            reg[instr->rd] = reg[instr->rs1] + immExt;
            printf("0x%x\n", reg[instr->rd]);

        break;

        case F3_SLTI: // Logical compare with Immediate and sign consideration
            printf("SLTI\t: reg[%.2d] = ( reg[%.2d] < %d ) ? 1 : 0\n", instr->rd, instr->rs1, immExt);
            printf("%41s reg[%.2d] = ( %d < %d ) ? = ", "", instr->rd, reg[instr->rs1], immExt);
            reg[instr->rd] = ((int32_t) reg[instr->rs1] < (int32_t) immExt ) ? 1 : 0;
            printf("0x%x\n", reg[instr->rd]);
        break;

        case F3_SLTIU: // Logical compare with Immediate and without sign consideration
            printf("SLTIU\t: reg[%.2d] = ( reg[%.2d] < %d ) ? 1 : 0\n", instr->rd, instr->rs1, immExt);
            printf("%41s reg[%.2d] = ( 0x%x < 0x%x ) ? = ", "", instr->rd, reg[instr->rs1], immExt);
            reg[instr->rd] = ( reg[instr->rs1] < immExt ) ? 1 : 0;
            printf("%d\n", reg[instr->rd]);
        break;

        case F3_XORI: // XOR with Immediate
            printf("XORI\t: reg[%.2d] = reg[%.2d] ^ %d\n", instr->rd, instr->rs1, immExt);
            printf("%41s reg[%.2d] = 0x%x ^ 0x%x = ", "", instr->rd, reg[instr->rs1], immExt);
            reg[instr->rd] = reg[instr->rs1] ^ immExt;
            printf("0x%x\n", reg[instr->rd]);
        break;

        case F3_ORI: // OR with Immediate
            printf("ORI\t: reg[%.2d] = reg[%.2d] | %d\n", instr->rd, instr->rs1, immExt);
            printf("%41s reg[%.2d] = 0x%x | 0x%x = ", "", instr->rd, reg[instr->rs1], immExt);
            reg[instr->rd] = reg[instr->rs1] | immExt;
            printf("0x%x\n", reg[instr->rd]);
        break;

        case F3_ANDI: // AND with Immediate
            printf("ANDI\t: reg[%.2d] = reg[%.2d] & %d\n", instr->rd, instr->rs1, immExt);
            printf("%41s reg[%.2d] = 0x%x & 0x%x = ", "", instr->rd, reg[instr->rs1], immExt);
            reg[instr->rd] = reg[instr->rs1] & immExt;
            printf("0x%x\n", reg[instr->rd]);
        break;

        case F3_SLLI: // Immediate Left Logical Shift
            printf("SLLI\t: reg[%.2d] = reg[%.2d] << %d\n", instr->rd, instr->rs1, shift);
            printf("%41s reg[%.2d] = 0x%x << %d = ", "", instr->rd, reg[instr->rs1], shift);
            reg[instr->rd] = reg[instr->rs1] << shift;
            printf("0x%x\n", reg[instr->rd]);
        break;

        case F3_SRLI:
        //case F3_SRAI: duplicate value
            switch(instr->funct7)
            {
                case F7_SRLI: // Immediate Right Logical Shift
                    printf("SRLI\t: reg[%.2d] = reg[%.2d] >> %d\n", instr->rd, instr->rs1, shift);
                    printf("%41s reg[%.2d] = 0x%x >> %d = ", "", instr->rd, reg[instr->rs1], shift);
                    reg[instr->rd] = reg[instr->rs1] >> shift;
                    printf("0x%x\n", reg[instr->rd]);
                break;
                case F7_SRAI: // Aritmethic Right Logical Shift
                    printf("SRLAI\t: reg[%.2d] = (int) reg[%.2d] >> (int) %d & 0x1f\n", instr->rd, instr->rs1, shift);
                    printf("%41s reg[%.2d] = 0x%x >> (int) %d = ", "", instr->rd, reg[instr->rs1], shift);
                    reg[instr->rd] = (int32_t) reg[instr->rs1] >> (int32_t) shift;
                    printf("0x%x\n", reg[instr->rd]);
                break;
            }
        break;
    }
    PC = PC + 4;
}

/* OP OPcode type subroutine */
void executeOP(struct command* instr, FILE *VHDLFile)
{
    uint32_t shift = reg[instr->rs2] & 0x1f;

    switch(instr->funct3)
    {
        case F3_ADD:
        // case F3_SUB: duplicate value
            switch(instr->funct7)
            {
                case F7_ADD: // ADD
                    printf("ADD\t: reg[%.2d] = reg[%.2d] + reg[%.2d] \n", instr->rd, instr->rs1, instr->rs2);
                    printf("%41s reg[%.2d] = 0x%x + 0x%x = ", "", instr->rd, reg[instr->rs1], reg[instr->rs2]);
                    reg[instr->rd] = reg[instr->rs1] + reg[instr->rs2];
                    printf("0x%x\n", reg[instr->rd]);
                break;

                case F7_SUB: // SUB
                    printf("SUB\t: reg[%.2d] = reg[%.2d] - reg[%.2d] \n", instr->rd, instr->rs1, instr->rs2);
                    printf("%41s reg[%.2d] = 0x%x - 0x%x = ", "", instr->rd, reg[instr->rs1], reg[instr->rs2]);
                    reg[instr->rd] = reg[instr->rs1] - reg[instr->rs2];
                    printf("0x%x\n", reg[instr->rd]);
                break;
            }
        break;

        case F3_SLL: // Left Logical Shift
            printf("SLL\t: reg[%.2d] = reg[%.2d] << reg[%.2d] & 0x1f\n", instr->rd, instr->rs1, instr->rs2);
            printf("%41s reg[%.2d] = 0x%x << 0x%x = ", "", instr->rd, reg[instr->rs1], shift);
            reg[instr->rd] = reg[instr->rs1] << shift;
            printf("0x%x\n", reg[instr->rd]);
        break;

        case F3_SLT: // Logical compare with Immediate and sign consideration
            printf("SLT\t: reg[%.2d] = ( reg[%.2d] < reg[%.2d] ) ? 1 : 0\n", instr->rd, instr->rs1, instr->rs2);
            printf("%41s reg[%.2d] = ( %d < %d ) ? = ", "", instr->rd, reg[instr->rs1], reg[instr->rs2]);
            reg[instr->rd] = ((int32_t) reg[instr->rs1] < (int32_t) reg[instr->rs2] ) ? 1 : 0;
            printf("%d\n", reg[instr->rd]);
        break;

        case F3_SLTU: // Logical compare with Immediate and withouth sign consideration
            printf("SLTU\t: reg[%.2d] = ( reg[%.2d] < reg[%.2d] ) ? 1 : 0\n", instr->rd, instr->rs1, instr->rs2);
            printf("%41s reg[%.2d] = ( 0x%x < 0x%x ) ? = ", "", instr->rd, reg[instr->rs1], reg[instr->rs2]);
            reg[instr->rd] = (reg[instr->rs1] < reg[instr->rs2] ) ? 1 : 0;
            printf("%d\n", reg[instr->rd]);
        break;

        case F3_XOR: // XOR
            printf("XOR\t: reg[%.2d] = reg[%.2d] ^ reg[%.2d]\n", instr->rd, instr->rs1, instr->rs2);
            printf("%41s reg[%.2d] = 0x%x ^ 0x%x = ", "", instr->rd, reg[instr->rs1], reg[instr->rs2]);
            reg[instr->rd] = reg[instr->rs1] ^ reg[instr->rs2];
            printf("0x%x\n", reg[instr->rd]);
        break;

        case F3_SRL:
        // case F3_SRA: duplicate value
            switch(instr->funct7)
            {
                case F7_SRL: //Right Logical Shift
                    printf("SRL\t: reg[%.2d] = reg[%.2d] >> reg[%.2d] & 0x1f\n", instr->rd, instr->rs1, instr->rs2);
                    printf("%41s reg[%.2d] = 0x%x >> 0x%x = ", "", instr->rd, reg[instr->rs1], shift);
                    reg[instr->rd] = reg[instr->rs1] >> shift;
                    printf("0x%x\n", reg[instr->rd]);
                break;

                case F7_SRA: // Right arithmetic Shift
                    printf("SRA\t: reg[%.2d] = (int) reg[%.2d] >> (int) reg[%.2d] & 0x1f\n", instr->rd, instr->rs1, instr->rs2);
                    printf("%41s reg[%.2d] = 0x%x >> 0x%x = ", "", instr->rd, reg[instr->rs1], shift);
                    reg[instr->rd] = (int32_t) reg[instr->rs1] >> shift;
                    printf("0x%x\n", reg[instr->rd]);
                break;
            }
        break;

        case F3_OR: // OR
            printf("OR\t: reg[%.2d] = reg[%.2d] | reg[%.2d]\n", instr->rd, instr->rs1, instr->rs2);
            printf("%41s reg[%.2d] = 0x%x | 0x%x = ", "", instr->rd, reg[instr->rs1], reg[instr->rs2]);
            reg[instr->rd] = reg[instr->rs1] | reg[instr->rs2];
            printf("0x%x\n", reg[instr->rd]);
        break;

        case F3_AND:// AND
            printf("AND\t: reg[%.2d] = reg[%.2d] & reg[%.2d]\n", instr->rd, instr->rs1, instr->rs2);
            printf("%41s reg[%.2d] = 0x%x & 0x%x = ", "", instr->rd, reg[instr->rs1], reg[instr->rs2]);
            reg[instr->rd] = reg[instr->rs1] & reg[instr->rs2];
            printf("0x%x\n", reg[instr->rd]);
        break;
    }
    PC = PC + 4;
}

void executeMISC_MEM(struct command* instr, FILE *VHDLFile){}
void executeSYSTEM(struct command* instr, FILE *VHDLFile){}