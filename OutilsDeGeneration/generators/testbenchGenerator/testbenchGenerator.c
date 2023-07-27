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
uint8_t dataMem[4096] = {0};
uint32_t reg[32] = {0};
uint32_t PC = 0;
uint32_t counterReg = 0;
uint8_t regsChecked, memsChecked, async, excel;

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
void initVHDLFile(FILE **VHDLFile, const char* filepath);
void endVHDLFile(FILE *VHDLFile);

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

void static inline assertRegs(uint16_t step, FILE *VHDLFile)
{
    fprintf(VHDLFile, "\t\t\tassert reg00 = x\"00000000\" report \"reg00 error at step %d\" severity error;\n", step);

    for (uint8_t i = 1 ; i < sizeof(reg)/sizeof(uint32_t) ; i++)
    {
        fprintf(VHDLFile, "\t\t\tassert reg%.2x = x\"%.8x\" report \"reg%.2x error at step %d\" severity error;\n", i, reg[i], i, step);
    }
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
    /* declare argument value */
    memsChecked = 0, regsChecked = 0, async = 0, excel = 0;
    for (uint8_t i = 4 ; argv[i] != NULL ; i++)
    {
        if (strcmp(argv[i], "-mem") == 0) memsChecked = 1;
        if (strcmp(argv[i], "-reg") == 0) regsChecked = 1;
        if (strcmp(argv[i], "-async") == 0) async = 1;
        if (strcmp(argv[i], "-excel") == 0) excel = 1;
    }
    /* Declare a pointer that will point on VHDL Test bench file*/
    FILE *VHDLFile;
    struct command instr;
    /* Read the file passed in argument of the script execution */
    readInstrFile(argv[1]);
    /* Init VHDL file with all the static information as reg port and signal names */
    initVHDLFile(&VHDLFile, argv[2]);
    /* set breakpoint limit*/
    uint16_t breakpoint = atoi(argv[3]);

    /* loop as long RA did not come back to its init value or breakpoint reached */
    do
    {
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

    endVHDLFile(VHDLFile);
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

    memcpy(dataMem, instrMem, instrSize);
}

/* Function that init the VHDL test file */
void initVHDLFile(FILE **VHDLFile, const char* filepath)
{
    char* text;
    /* Open the vhd file in write mode to erase it */
    *VHDLFile = fopen(filepath, "w");

    if (memsChecked == 0)
    {
        text =
            "library IEEE;\n"
            "use IEEE.STD_LOGIC_1164.ALL;\n"
            "use IEEE.NUMERIC_STD.ALL;\n"
            "use work.simulPkg.all;\n"
            "\n"
            "entity testbench is\n"
            "end testbench;\n"
            "\n"
            "architecture vhdl of testbench is\n"
            "\n"
            "    component Processor is\n"
            "        Port (\n"
            "        -- INPUTS\n"
            "        PROCclock        : in std_logic;\n"
            "        PROCreset        : in std_logic;\n"
            "        PROCinstruction  : in std_logic_vector(31 downto 0);\n"
            "        PROCoutputDM     : in std_logic_vector(31 downto 0);\n"
            "        -- OUTPUTS\n"
            "        PROCprogcounter  : out std_logic_vector(31 downto 0);\n"
            "        PROCstore        : out std_logic;\n"
            "        PROCload         : out std_logic;\n"
            "        PROCfunct3       : out std_logic_vector(2 downto 0);\n"
            "        PROCaddrDM       : out std_logic_vector(31 downto 0);\n"
            "        PROCinputDM      : out std_logic_vector(31 downto 0)\n"
            "    );\n"
            "    end component;\n"
            "\n"
            "    signal reset, ck : std_logic;\n"
            "    signal counter, progcounter, instr: std_logic_vector(31 downto 0);\n"
            "    \n"
            "    signal dataAddr: std_logic_vector(31 downto 0);\n"
            "    signal load, store : std_logic;\n"
            "    signal dataLength : std_logic_vector(2 downto 0);\n"
            "    signal inputData, outputData: std_logic_vector(31 downto 0);\n"
            "    \n"
            "    signal reg00, reg01, reg02, reg03, reg04, reg05, reg06, reg07, reg08, reg09, reg0A, reg0B, reg0C, reg0D, reg0E, reg0F, reg10, reg11, reg12, reg13, reg14, reg15, reg16, reg17, reg18, reg19, reg1A, reg1B, reg1C, reg1D, reg1E, reg1F : std_logic_vector(31 downto 0);\n"
            "    signal SigTOPdisplay1, SigTOPdisplay2 : std_logic_vector (31 downto 0);\n"
            "    \n"
            "    BEGIN\n"
            "\n"
            "    --instanciation de l'entité PROC\n"
            "    iProcessor : Processor port map (\n"
            "        PROCclock        => ck,\n"
            "        PROCreset        => reset,\n"
            "        PROCinstruction  => instr,\n"
            "        PROCoutputDM     => outputData,\n"
            "        PROCprogcounter  => progcounter,\n"
            "        PROCstore        => store,\n"
            "        PROCload         => load,\n"
            "        PROCfunct3       => dataLength,\n"
            "        PROCaddrDM       => dataAddr,\n"
            "        PROCinputDM      => inputData\n"
            "    );\n"
            "    \n"
            "    reg00       <= PKG_reg00;\n"
            "    reg01       <= PKG_reg01;\n"
            "    reg02       <= PKG_reg02;\n"
            "    reg03       <= PKG_reg03;\n"
            "    reg04       <= PKG_reg04;\n"
            "    reg05       <= PKG_reg05;\n"
            "    reg06       <= PKG_reg06;\n"
            "    reg07       <= PKG_reg07;\n"
            "    reg08       <= PKG_reg08;\n"
            "    reg09       <= PKG_reg09;\n"
            "    reg0A       <= PKG_reg0A;\n"
            "    reg0B       <= PKG_reg0B;\n"
            "    reg0C       <= PKG_reg0C;\n"
            "    reg0D       <= PKG_reg0D;\n"
            "    reg0E       <= PKG_reg0E;\n"
            "    reg0F       <= PKG_reg0F;\n"
            "    reg10       <= PKG_reg10;\n"
            "    reg11       <= PKG_reg11;\n"
            "    reg12       <= PKG_reg12;\n"
            "    reg13       <= PKG_reg13;\n"
            "    reg14       <= PKG_reg14;\n"
            "    reg15       <= PKG_reg15;\n"
            "    reg16       <= PKG_reg16;\n"
            "    reg17       <= PKG_reg17;\n"
            "    reg18       <= PKG_reg18;\n"
            "    reg19       <= PKG_reg19;\n"
            "    reg1A       <= PKG_reg1A;\n"
            "    reg1B       <= PKG_reg1B;\n"
            "    reg1C       <= PKG_reg1C;\n"
            "    reg1D       <= PKG_reg1D;\n"
            "    reg1E       <= PKG_reg1E;\n"
            "    reg1F       <= PKG_reg1F;\n"
            "    \n"
            " \tVecteurTest : process\n"
            "\t\tbegin\n"
            "\t\t-- init  simulation\n"
            "\t\t\tck <= '0';\n"
            "\t\t\treset <= '1';\n"
            "\t\t\tinstr <= x\"00000000\"; -- init\n"
            "\t\t\twait for 10 ns;\n"
            "\t\t\tck <= '1';\n"
            "\t\t\twait for 5 ns;\n"
            "\t\t\treset <= '0';\n"
            "\t\t\twait for 5 ns;\n"
            "\t\t\tassert false report \"Index;Instruction;Description;Status;Note\" severity note;\n"
            "\n"
        ;
    }
    else
    {
        text =
            "-- Projet de fin d'études : RISC-V\n"
            "-- ECE Paris / SECAPEM\n"
            "\n"
            "-- LIBRARIES\n"
            "library ieee;\n"
            "use ieee.std_logic_1164.all;\n"
            "use ieee.numeric_std.all;\n"
            "use work.simulPkg.all;\n"
            "\n"
            "-- ENTITY\n"
            "entity TestBenchTop is\n"
            "end entity;\n"
            "\n"
            "architecture VHDL of TestBenchTop is\n"
            "\tcomponent Top is\n"
            "    port (\n"
            "        -- INPUTS\n"
            "        TOPclock       : in std_logic;\t\t--must go through pll\n"
            "        TOPreset       : in std_logic; \t--SW0\n"
            "        -- DEMO OUTPUTS\n"
            "        TOPdisplay1    : out std_logic_vector(31 downto 0);\t--0x80000004\n"
            "        TOPdisplay2    : out std_logic_vector(31 downto 0);\t--0x80000008\n"
            "        TOPleds        : out std_logic_vector(31 downto 0)\t--0x8000000c\n"
            "\n"
            "\t);\n"
            "\tend component;\n"
            "\n"
            "\tsignal reset, ck : std_logic;\n"
            "\tsignal counter, progcounter, instr: std_logic_vector(31 downto 0);\n"
            "    \n"
            "\tsignal dataAddr: std_logic_vector(31 downto 0);\n"
            "\tsignal load, store : std_logic;\n"
            "\tsignal dataLength : std_logic_vector(2 downto 0);\n"
            "\tsignal inputData, outputData: std_logic_vector(31 downto 0);\n"
            "    \n"
            "\tsignal reg00, reg01, reg02, reg03, reg04, reg05, reg06, reg07, reg08, reg09, reg0A, reg0B, reg0C, reg0D, reg0E, reg0F, reg10, reg11, reg12, reg13, reg14, reg15, reg16, reg17, reg18, reg19, reg1A, reg1B, reg1C, reg1D, reg1E, reg1F : std_logic_vector(31 downto 0);\n"
            "\tsignal SigTOPdisplay1, SigTOPdisplay2 : std_logic_vector (31 downto 0);\n"
            "\t\n"
            "\tBEGIN\n"
            "\t\n"
            "\t--instanciation de l'entité PROC\n"
            "\tiTop : Top port map (\n"
            "\t\tTOPclock        => ck,\n"
            "\t\tTOPreset        => reset,\n"
            "\t\tTOPdisplay1     => SigTOPdisplay1,\n"
            "\t\tTOPdisplay2     => SigTOPdisplay2\n"
            "\t);\n"
            "    \n"
            "    counter     <= PKG_counter;\n"
            "    store       <= PKG_store;      \n"
            "    load        <= PKG_load;       \n"
            "    dataLength  <= PKG_funct3;     \n"
            "    dataAddr    <= PKG_addrDM;     \n"
            "    inputData   <= PKG_inputDM;    \n"
            "    outputData  <= PKG_outputDM;   \n"
            "    progcounter <= PKG_progcounter;\n"
            "    instr       <= PKG_instruction;\n"
            "    reg00       <= PKG_reg00;\n"
            "    reg01       <= PKG_reg01;\n"
            "    reg02       <= PKG_reg02;\n"
            "    reg03       <= PKG_reg03;\n"
            "    reg04       <= PKG_reg04;\n"
            "    reg05       <= PKG_reg05;\n"
            "    reg06       <= PKG_reg06;\n"
            "    reg07       <= PKG_reg07;\n"
            "    reg08       <= PKG_reg08;\n"
            "    reg09       <= PKG_reg09;\n"
            "    reg0A       <= PKG_reg0A;\n"
            "    reg0B       <= PKG_reg0B;\n"
            "    reg0C       <= PKG_reg0C;\n"
            "    reg0D       <= PKG_reg0D;\n"
            "    reg0E       <= PKG_reg0E;\n"
            "    reg0F       <= PKG_reg0F;\n"
            "    reg10       <= PKG_reg10;\n"
            "    reg11       <= PKG_reg11;\n"
            "    reg12       <= PKG_reg12;\n"
            "    reg13       <= PKG_reg13;\n"
            "    reg14       <= PKG_reg14;\n"
            "    reg15       <= PKG_reg15;\n"
            "    reg16       <= PKG_reg16;\n"
            "    reg17       <= PKG_reg17;\n"
            "    reg18       <= PKG_reg18;\n"
            "    reg19       <= PKG_reg19;\n"
            "    reg1A       <= PKG_reg1A;\n"
            "    reg1B       <= PKG_reg1B;\n"
            "    reg1C       <= PKG_reg1C;\n"
            "    reg1D       <= PKG_reg1D;\n"
            "    reg1E       <= PKG_reg1E;\n"
            "    reg1F       <= PKG_reg1F;\n"
            "\n"
            "\tVecteurTest : process\n"
            "\t\tbegin\n"
            "\t\t-- init  simulation\n"
            "\t\t\tck <= '1';\n"
            "\t\t\treset <= '1';\n"
            "\t\t\twait for 2ns;\n"
            "\t\t\treset <= '0';\n"
            "\t\t\twait for 8 ns;\n"
            "\t\t\tassert instr = x\"00000000\" report \"wrong instruction at init\" severity error;\n"
            "\t\t\tassert false report \"Index;Instruction;Description;Status;Note\" severity note;\n"
            "\n"
        ;
    }
    /* write string sequence in VHDL file*/
    fprintf(*VHDLFile, "%s", text);
}

/* Function that close the VHDL test file*/
void endVHDLFile(FILE *VHDLFile)
{
    char* text =
        "\t\t\twait;\n"
        "\t\tend process;\n"
        "END vhdl;";
    /* write string sequence in VHDL file*/
    fprintf(VHDLFile, "%s", text);
    /* close file */
    fclose(VHDLFile);
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
    fprintf(VHDLFile, "\t\t-- load instruction %d\n", instrCount);
    fprintf(VHDLFile, "\t\t\tck <= '0';\n");
    if (memsChecked == 0)
    {
        fprintf(VHDLFile, "\t\t\tinstr <= x\"%.8x\"; -- LUI : reg[%.2d] = 0x%x << 12\n", instr->instr, instr->rd, instr->imm20);
        fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
    }
    else
    {
        fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
        fprintf(VHDLFile, "\t\t\tassert instr = x\"%.8x\" report \"instruction error at step %d\" severity error;\n", instr->instr, instrCount*2);
    }
    if (excel) fprintf(VHDLFile, "\t\t\tassert false report \"%d;0x%.8x;LUI : reg[%.2d] = 0x%x << 12;OK; ;\" severity note;\n",instrCount, instr->instr, instr->rd, instr->imm20);
    fprintf(VHDLFile, "\t\t\tassert progcounter = x\"%.8x\" report \"progcounter error at step %d\" severity error;\n", PC, instrCount*2);


    reg[instr->rd] = instr->imm20 << 12;
    PC = PC + 4;

    fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
    fprintf(VHDLFile, "\n");
    fprintf(VHDLFile, "\t\t-- execute instruction %d\n", instrCount);
    fprintf(VHDLFile, "\t\t\tck <= '1';\n");
    fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
    if (regsChecked == 1)
    {
        assertRegs(instrCount*2+1, VHDLFile);
    }
    fprintf(VHDLFile, "\t\t\tassert progcounter = x\"%.8x\" report \"progcounter error at step %d\" severity error;\n", PC, instrCount*2+1);
    fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
    fprintf(VHDLFile, "\n");
}

/* add upper immediate to PC OPcode type subroutine */
void executeAUIPC(struct command* instr, FILE *VHDLFile)
{
    fprintf(VHDLFile, "\t\t-- load instruction %d\n", instrCount);
    fprintf(VHDLFile, "\t\t\tck <= '0';\n");
    if (memsChecked == 0)
    {
        fprintf(VHDLFile, "\t\t\tinstr <= x\"%.8x\"; -- AUIPC : reg[%.2d] = PC + 0x%x << 12\n", instr->instr, instr->rd, instr->imm20);
        fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
    }
    else
    {
        fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
        fprintf(VHDLFile, "\t\t\tassert instr = x\"%.8x\" report \"instruction error at step %d\" severity error;\n", instr->instr, instrCount*2);
    }
    if (excel) fprintf(VHDLFile, "\t\t\tassert false report \"%d;0x%.8x;AUIPC : reg[%.2d] = PC + 0x%x << 12;OK; ;\" severity note;\n",instrCount, instr->instr, instr->rd, instr->imm20);
    fprintf(VHDLFile, "\t\t\tassert progcounter = x\"%.8x\" report \"progcounter error at step %d\" severity error;\n", PC, instrCount*2);

    reg[instr->rd] = PC + (instr->imm20 << 12);
    PC = PC + 4;

    fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
    fprintf(VHDLFile, "\n");
    fprintf(VHDLFile, "\t\t-- execute instruction %d\n", instrCount);
    fprintf(VHDLFile, "\t\t\tck <= '1';\n");
    fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
    if (regsChecked == 1)
    {
        assertRegs(instrCount*2+1, VHDLFile);
    }
    fprintf(VHDLFile, "\t\t\tassert progcounter = x\"%.8x\" report \"progcounter error at step %d\" severity error;\n", PC, instrCount*2+1);
    fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
    fprintf(VHDLFile, "\n");
}

/* Jump and link OPcode type subroutine */
void executeJAL(struct command* instr, FILE *VHDLFile)
{
    uint32_t offset = (( instr->imm20 >> 9  & 0x3ff) << 1  ) | // 10:0 0 forced to 0
                      (( instr->imm20 >> 8  & 0x01 ) << 11 ) | // 11
                      (( instr->imm20 >> 0  & 0xff ) << 12 ) | // 19:12
                      (( instr->imm20 >> 19 & 0x01 ) << 20 );  // 20 sign

    offset = (offset & 0x100000) ? offset| ~0x000fffff : offset & 0x000fffff;

    fprintf(VHDLFile, "\t\t-- load instruction %d\n", instrCount);
    fprintf(VHDLFile, "\t\t\tck <= '0';\n");
    if (memsChecked == 0)
    {
        fprintf(VHDLFile, "\t\t\tinstr <= x\"%.8x\"; -- JAL : reg[%.2d] = PC+4 and PC = 0x%x + %d\n", instr->instr, instr->rd, PC, offset);
        fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
    }
    else
    {
        fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
        fprintf(VHDLFile, "\t\t\tassert instr = x\"%.8x\" report \"instruction error at step %d\" severity error;\n", instr->instr, instrCount*2);
    }
    if (excel) fprintf(VHDLFile, "\t\t\tassert false report \"%d;0x%.8x;JAL : reg[%.2d] = PC+4 and PC = 0x%x + %d;OK; ;\" severity note;\n",instrCount, instr->instr, instr->rd, PC, offset);
    fprintf(VHDLFile, "\t\t\tassert progcounter = x\"%.8x\" report \"progcounter error at step %d\" severity error;\n", PC, instrCount*2);

    reg[instr->rd] = PC + 4;
    PC = PC + offset;

    fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
    fprintf(VHDLFile, "\n");
    fprintf(VHDLFile, "\t\t-- execute instruction %d\n", instrCount);
    fprintf(VHDLFile, "\t\t\tck <= '1';\n");
    fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
    if (regsChecked == 1)
    {
        assertRegs(instrCount*2+1, VHDLFile);
    }
    fprintf(VHDLFile, "\t\t\tassert progcounter = x\"%.8x\" report \"progcounter error at step %d\" severity error;\n", PC, instrCount*2+1);
    fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
    fprintf(VHDLFile, "\n");
}

/* Indirect Jump and link OPcode type subroutine */
void executeJALR(struct command* instr, FILE *VHDLFile)
{
    int32_t offset = (instr->imm12 & 0x800) ? instr->imm12 | ~0x000007ff : instr->imm12 & 0x000007ff;

    fprintf(VHDLFile, "\t\t-- load instruction %d\n", instrCount);
    fprintf(VHDLFile, "\t\t\tck <= '0';\n");
    if (memsChecked == 0)
    {
        fprintf(VHDLFile, "\t\t\tinstr <= x\"%.8x\"; -- JALR : reg[%.2d] = PC+4 and PC = (reg[%.2d] + %d) & ~1 \n", instr->instr, instr->rd, instr->rs1, offset);
        fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
    }
    else
    {
        fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
        fprintf(VHDLFile, "\t\t\tassert instr = x\"%.8x\" report \"instruction error at step %d\" severity error;\n", instr->instr, instrCount*2);
    }
    if (excel) fprintf(VHDLFile, "\t\t\tassert false report \"%d;0x%.8x;JALR : reg[%.2d] = PC+4 and PC = (reg[%.2d] + %d) & ~1;OK; ;\" severity note;\n",instrCount, instr->instr, instr->rd, instr->rs1, offset);
    fprintf(VHDLFile, "\t\t\tassert progcounter = x\"%.8x\" report \"progcounter error at step %d\" severity error;\n", PC, instrCount*2);

    reg[instr->rd] = PC + 4;
    PC = reg[instr->rs1] + (offset & ~1);

    fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
    fprintf(VHDLFile, "\n");
    fprintf(VHDLFile, "\t\t-- execute instruction %d\n", instrCount);
    fprintf(VHDLFile, "\t\t\tck <= '1';\n");
    fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
    if (regsChecked == 1)
    {
        assertRegs(instrCount*2+1, VHDLFile);
    }
    fprintf(VHDLFile, "\t\t\tassert progcounter = x\"%.8x\" report \"progcounter error at step %d\" severity error;\n", PC, instrCount*2+1);
    fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
    fprintf(VHDLFile, "\n");
}

/* Branch OPcode type subroutine */
void executeBRANCH(struct command* instr, FILE *VHDLFile)
{
    int32_t offset =  (  instr->imm5 & 0x1e        )         | // 4:0 with 0 forced 0
                      (( instr->imm7 & 0x3f        ) << 5  ) | // 10:5
                      (( instr->imm5 & 0x01        ) << 11 ) | // 11
                      (((instr->imm7 & 0x40) >> 6  ) << 12 );  // 12 sign

    offset = (offset & 0x1000) ? offset | ~0x00000fff : offset & 0x00000fff;

    fprintf(VHDLFile, "\t\t-- load instruction %d\n", instrCount);
    fprintf(VHDLFile, "\t\t\tck <= '0';\n");

    /* All branches work the same, if the condition is true jump to
     * PC + Immediate within 4Kib range */
    switch(instr->funct3)
    {
        case F3_BEQ: // Branch if equal
            if (memsChecked == 0)
            {
                fprintf(VHDLFile, "\t\t\tinstr <= x\"%.8x\"; -- BEQ : if ( reg[%.2d] == reg[%.2d] ) PC = PC + %d\n", instr->instr, instr->rs1, instr->rs2, offset);
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
            }
            else
            {
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
                fprintf(VHDLFile, "\t\t\tassert instr = x\"%.8x\" report \"instruction error at step %d\" severity error;\n", instr->instr, instrCount*2);
            }
            if (excel) fprintf(VHDLFile, "\t\t\tassert false report \"%d;0x%.8x;BEQ : if ( reg[%.2d] == reg[%.2d] ) PC = PC + %d;OK; ;\" severity note;\n",instrCount, instr->instr, instr->rs1, instr->rs2, offset);
            fprintf(VHDLFile, "\t\t\tassert progcounter = x\"%.8x\" report \"progcounter error at step %d\" severity error;\n", PC, instrCount*2);
            if ( reg[instr->rs1] == reg[instr->rs2] ) PC = PC + offset;
            else PC = PC + 4;

        break;

        case F3_BNE: // branch if not equal
            if (memsChecked == 0)
            {
                fprintf(VHDLFile, "\t\t\tinstr <= x\"%.8x\"; -- BNE : if ( reg[%.2d] != reg[%.2d] ) PC = PC + %d\n", instr->instr, instr->rs1, instr->rs2, offset);
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
            }
            else
            {
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
                fprintf(VHDLFile, "\t\t\tassert instr = x\"%.8x\" report \"instruction error at step %d\" severity error;\n", instr->instr, instrCount*2);
            }

            if (excel) fprintf(VHDLFile, "\t\t\tassert false report \"%d;0x%.8x;BNE : if ( reg[%.2d] != reg[%.2d] ) PC = PC + %d;OK; ;\" severity note;\n",instrCount, instr->instr, instr->rs1, instr->rs2, offset);
            fprintf(VHDLFile, "\t\t\tassert progcounter = x\"%.8x\" report \"progcounter error at step %d\" severity error;\n", PC, instrCount*2);
            if ( reg[instr->rs1] != reg[instr->rs2] ) PC = PC + offset;
            else PC = PC + 4;

        break;

        case F3_BLT: // branch if lesser than with sign consideration
            if (memsChecked == 0)
            {
                fprintf(VHDLFile, "\t\t\tinstr <= x\"%.8x\"; -- BLT : if ( reg[%.2d] < reg[%.2d] ) PC = PC + %d\n", instr->instr, instr->rs1, instr->rs2, offset);
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
            }
            else
            {
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
                fprintf(VHDLFile, "\t\t\tassert instr = x\"%.8x\" report \"instruction error at step %d\" severity error;\n", instr->instr, instrCount*2);
            }

            if (excel) fprintf(VHDLFile, "\t\t\tassert false report \"%d;0x%.8x;BLT : if ( reg[%.2d] < reg[%.2d] ) PC = PC + %d;OK; ;\" severity note;\n",instrCount, instr->instr, instr->rs1, instr->rs2, offset);
            fprintf(VHDLFile, "\t\t\tassert progcounter = x\"%.8x\" report \"progcounter error at step %d\" severity error;\n", PC, instrCount*2);
            if ( (int32_t) reg[instr->rs1] < (int32_t) reg[instr->rs2] ) PC = PC + offset;
            else PC = PC + 4;

        break;

        case F3_BGE: // branch if greater than with sign consideration
            if (memsChecked == 0)
            {
                fprintf(VHDLFile, "\t\t\tinstr <= x\"%.8x\"; -- BGE : if ( reg[%.2d] >= reg[%.2d] ) PC = PC + %d\n", instr->instr, instr->rs1, instr->rs2, offset);
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
            }
            else
            {
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
                fprintf(VHDLFile, "\t\t\tassert instr = x\"%.8x\" report \"instruction error at step %d\" severity error;\n", instr->instr, instrCount*2);
            }

            if (excel) fprintf(VHDLFile, "\t\t\tassert false report \"%d;0x%.8x;BGE : if ( reg[%.2d] >= reg[%.2d] ) PC = PC + %d;OK; ;\" severity note;\n",instrCount, instr->instr, instr->rs1, instr->rs2, offset);
            fprintf(VHDLFile, "\t\t\tassert progcounter = x\"%.8x\" report \"progcounter error at step %d\" severity error;\n", PC, instrCount*2);
            if ( (int32_t) reg[instr->rs1] >= (int32_t) reg[instr->rs2] ) PC = PC + offset;
            else PC = PC + 4;

        break;

        case F3_BLTU: // branch if lesser than without sign consideration
            if (memsChecked == 0)
            {
                fprintf(VHDLFile, "\t\t\tinstr <= x\"%.8x\"; -- BLTU: if ( reg[%.2d] < reg[%.2d] ) PC = PC + %d\n", instr->instr, instr->rs1, instr->rs2, offset);
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
            }
            else
            {
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
                fprintf(VHDLFile, "\t\t\tassert instr = x\"%.8x\" report \"instruction error at step %d\" severity error;\n", instr->instr, instrCount*2);
            }

            if (excel) fprintf(VHDLFile, "\t\t\tassert false report \"%d;0x%.8x;BLTU: if ( reg[%.2d] < reg[%.2d] ) PC = PC + %d;OK; ;\" severity note;\n",instrCount, instr->instr, instr->rs1, instr->rs2, offset);
            fprintf(VHDLFile, "\t\t\tassert progcounter = x\"%.8x\" report \"progcounter error at step %d\" severity error;\n", PC, instrCount*2);
            if ( reg[instr->rs1] < reg[instr->rs2] ) PC = PC + offset;
            else PC = PC + 4;

        break;

        case F3_BGEU: // branch if greater than without sign consideration
            if (memsChecked == 0)
            {
                fprintf(VHDLFile, "\t\t\tinstr <= x\"%.8x\"; -- BGEU : if ( reg[%.2d] >= reg[%.2d] ) PC = PC + %d\n", instr->instr, instr->rs1, instr->rs2, offset);
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
            }
            else
            {
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
                fprintf(VHDLFile, "\t\t\tassert instr = x\"%.8x\" report \"instruction error at step %d\" severity error;\n", instr->instr, instrCount*2);
            }

            if (excel) fprintf(VHDLFile, "\t\t\tassert false report \"%d;0x%.8x;BGEU : if ( reg[%.2d] >= reg[%.2d] ) PC = PC + %d;OK; ;\" severity note;\n",instrCount, instr->instr, instr->rs1, instr->rs2, offset);
            fprintf(VHDLFile, "\t\t\tassert progcounter = x\"%.8x\" report \"progcounter error at step %d\" severity error;\n", PC, instrCount*2);
            if ( reg[instr->rs1] >= reg[instr->rs2] ) PC = PC + offset;
            else PC = PC + 4;

        break;
    }
    fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
    fprintf(VHDLFile, "\n");
    fprintf(VHDLFile, "\t\t-- execute instruction %d\n", instrCount);
    fprintf(VHDLFile, "\t\t\tck <= '1';\n");
    fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
    if (regsChecked == 1)
    {
        assertRegs(instrCount*2+1, VHDLFile);
    }
    fprintf(VHDLFile, "\t\t\tassert progcounter = x\"%.8x\" report \"progcounter error at step %d\" severity error;\n", PC, instrCount*2+1);
    fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
    fprintf(VHDLFile, "\n");
}

/* LOAD OPcode type subroutine */
void executeLOAD(struct command* instr, FILE *VHDLFile)
{
    uint32_t data;
    uint32_t offset = (instr->imm12 & 0x800) ? instr->imm12 | ~0x000007ff : instr->imm12 & 0x000007ff;
    uint32_t addr = reg[instr->rs1] + offset;

    fprintf(VHDLFile, "\t\t-- load instruction %d\n", instrCount);
    fprintf(VHDLFile, "\t\t\tck <= '0';\n");

    switch(instr->funct3)
    {
        case F3_LB: // Load int8_t
            data = readDataMemory(addr, sizeof(uint8_t));
            reg[instr->rd] = ((data & 0xff) >> 7) ? data | ~0x000000ff : data & 0x000000ff;

            if (memsChecked == 0)
            {
                fprintf(VHDLFile, "\t\t\tinstr <= x\"%.8x\"; -- LDB : reg[%.2d] = dataMem[reg[%.2d] + %d]\n", instr->instr, instr->rd, instr->rs1, offset);
                fprintf(VHDLFile, "\t\t\toutputData <= x\"%.8x\";\n",reg[instr->rd]);
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
            }
            else
            {
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
                fprintf(VHDLFile, "\t\t\tassert instr = x\"%.8x\" report \"instruction error at step %d\" severity error;\n", instr->instr, instrCount*2);
            }
            if (excel) fprintf(VHDLFile, "\t\t\tassert false report \"%d;0x%.8x;LDB : reg[%.2d] = dataMem[reg[%.2d] + %d];OK; ;\" severity note;\n",instrCount, instr->instr, instr->rd, instr->rs1, offset);
            fprintf(VHDLFile, "\t\t\tassert progcounter = x\"%.8x\" report \"progcounter error at step %d\" severity error;\n", PC, instrCount*2);
            fprintf(VHDLFile, "\t\t\tassert dataAddr = x\"%.8x\"    report \"address error at step %d\"     severity error;\n", addr, instrCount*2);
            fprintf(VHDLFile, "\t\t\tassert dataLength = \"000\"        report \"length error at step %d\"      severity error;\n", instrCount*2);
            fprintf(VHDLFile, "\t\t\tassert load = '1'                report \"load error at step %d\"        severity error;\n", instrCount*2);
        break;

        case F3_LH: // Load int16_t
            data = readDataMemory(addr, sizeof(uint16_t));
            reg[instr->rd] = ((data & 0xffff) >> 15) ? data | ~0x0000ffff : data & 0x0000ffff;
            if (memsChecked == 0)
            {
                fprintf(VHDLFile, "\t\t\tinstr <= x\"%.8x\"; -- LDH : reg[%.2d] = dataMem[reg[%.2d] + %d]\n", instr->instr, instr->rd, instr->rs1, offset);
                fprintf(VHDLFile, "\t\t\toutputData <= x\"%.8x\";\n",reg[instr->rd]);
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
            }
            else
            {
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
                fprintf(VHDLFile, "\t\t\tassert instr = x\"%.8x\" report \"instruction error at step %d\" severity error;\n", instr->instr, instrCount*2);
            }
            if (excel) fprintf(VHDLFile, "\t\t\tassert false report \"%d;0x%.8x;LDH : reg[%.2d] = dataMem[reg[%.2d] + %d];OK; ;\" severity note;\n",instrCount, instr->instr, instr->rd, instr->rs1, offset);
            fprintf(VHDLFile, "\t\t\tassert progcounter = x\"%.8x\" report \"progcounter error at step %d\" severity error;\n", PC, instrCount*2);
            fprintf(VHDLFile, "\t\t\tassert dataAddr = x\"%.8x\"    report \"address error at step %d\"     severity error;\n", addr, instrCount*2);
            fprintf(VHDLFile, "\t\t\tassert dataLength = \"001\"        report \"length error at step %d\"      severity error;\n", instrCount*2);
            fprintf(VHDLFile, "\t\t\tassert load = '1'                report \"load error at step %d\"        severity error;\n", instrCount*2);
        break;

        case F3_LW: // Load int32_t / uint32_t
            reg[instr->rd] = readDataMemory(addr, sizeof(uint32_t));
            if (memsChecked == 0)
            {
                fprintf(VHDLFile, "\t\t\tinstr <= x\"%.8x\"; -- LDW : reg[%.2d] = dataMem[reg[%.2d] + %d]\n", instr->instr, instr->rd, instr->rs1, offset);
                fprintf(VHDLFile, "\t\t\toutputData <= x\"%.8x\";\n",reg[instr->rd]);
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
            }
            else
            {
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
                fprintf(VHDLFile, "\t\t\tassert instr = x\"%.8x\" report \"instruction error at step %d\" severity error;\n", instr->instr, instrCount*2);
            }
            if (excel) fprintf(VHDLFile, "\t\t\tassert false report \"%d;0x%.8x;LDW : reg[%.2d] = dataMem[reg[%.2d] + %d];OK; ;\" severity note;\n",instrCount, instr->instr, instr->rd, instr->rs1, offset);
            fprintf(VHDLFile, "\t\t\tassert progcounter = x\"%.8x\" report \"progcounter error at step %d\" severity error;\n", PC, instrCount*2);
            fprintf(VHDLFile, "\t\t\tassert dataAddr = x\"%.8x\"    report \"address error at step %d\"     severity error;\n", addr, instrCount*2);
            fprintf(VHDLFile, "\t\t\tassert dataLength = \"010\"        report \"length error at step %d\"      severity error;\n", instrCount*2);
            fprintf(VHDLFile, "\t\t\tassert load = '1'                report \"load error at step %d\"        severity error;\n", instrCount*2);
        break;

        case F3_LBU: // Load uint8_t
            reg[instr->rd] = readDataMemory(addr, sizeof(uint8_t));
            if (memsChecked == 0)
            {
                fprintf(VHDLFile, "\t\t\tinstr <= x\"%.8x\"; -- LBU : reg[%.2d] = dataMem[reg[%.2d] + %d]\n", instr->instr, instr->rd, instr->rs1, offset);
                fprintf(VHDLFile, "\t\t\toutputData <= x\"%.8x\";\n",reg[instr->rd]);
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
            }
            else
            {
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
                fprintf(VHDLFile, "\t\t\tassert instr = x\"%.8x\" report \"instruction error at step %d\" severity error;\n", instr->instr, instrCount*2);
            }
            if (excel) fprintf(VHDLFile, "\t\t\tassert false report \"%d;0x%.8x;LBU : reg[%.2d] = dataMem[reg[%.2d] + %d];OK; ;\" severity note;\n",instrCount, instr->instr, instr->rd, instr->rs1, offset);
            fprintf(VHDLFile, "\t\t\tassert progcounter = x\"%.8x\" report \"progcounter error at step %d\" severity error;\n", PC, instrCount*2);
            fprintf(VHDLFile, "\t\t\tassert dataAddr = x\"%.8x\"    report \"address error at step %d\"     severity error;\n", addr, instrCount*2);
            fprintf(VHDLFile, "\t\t\tassert dataLength = \"100\"        report \"length error at step %d\"      severity error;\n", instrCount*2);
            fprintf(VHDLFile, "\t\t\tassert load = '1'                report \"load error at step %d\"        severity error;\n", instrCount*2);
        break;

        case F3_LHU: // Load uint16_t
            reg[instr->rd] = readDataMemory(addr, sizeof(uint16_t));
            if (memsChecked == 0)
            {
                fprintf(VHDLFile, "\t\t\tinstr <= x\"%.8x\"; -- LHU : reg[%.2d] = dataMem[reg[%.2d] + %d]\n", instr->instr, instr->rd, instr->rs1, offset);
                fprintf(VHDLFile, "\t\t\toutputData <= x\"%.8x\";\n",reg[instr->rd]);
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
            }
            else
            {
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
                fprintf(VHDLFile, "\t\t\tassert instr = x\"%.8x\" report \"instruction error at step %d\" severity error;\n", instr->instr, instrCount*2+1);
            }
            if (excel) fprintf(VHDLFile, "\t\t\tassert false report \"%d;0x%.8x;LHU : reg[%.2d] = dataMem[reg[%.2d] + %d];OK; ;\" severity note;\n",instrCount, instr->instr, instr->rd, instr->rs1, offset);
            fprintf(VHDLFile, "\t\t\tassert progcounter = x\"%.8x\" report \"progcounter error at step %d\" severity error;\n", PC, instrCount*2);
            fprintf(VHDLFile, "\t\t\tassert dataAddr = x\"%.8x\"    report \"address error at step %d\"     severity error;\n", addr, instrCount*2);
            fprintf(VHDLFile, "\t\t\tassert dataLength = \"101\"        report \"length error at step %d\"      severity error;\n", instrCount*2);
            fprintf(VHDLFile, "\t\t\tassert load = '1'                report \"load error at step %d\"        severity error;\n", instrCount*2);
        break;
    }
    PC = PC + 4;

    fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
    fprintf(VHDLFile, "\n");
    fprintf(VHDLFile, "\t\t-- execute instruction %d\n", instrCount);
    fprintf(VHDLFile, "\t\t\tck <= '1';\n");
    fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
    if (regsChecked == 1)
    {
        assertRegs(instrCount*2+1, VHDLFile);
    }
    if (memsChecked != 0)
    {
        // BUG with async memory
        // fprintf(VHDLFile, "\t\t\tassert outputData = x\"%.8x\" report \"data error at step %d\" severity error;\n", reg[instr->rd], instrCount*2+1);
    }
    fprintf(VHDLFile, "\t\t\tassert progcounter = x\"%.8x\" report \"progcounter error at step %d\" severity error;\n", PC, instrCount*2+1);
    fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
    fprintf(VHDLFile, "\n");
}

/* STORE OPcode type subroutine */
void executeSTORE(struct command* instr, FILE *VHDLFile)
{
    uint32_t offset = (instr->imm7 << 5) | (instr->imm5);
    offset = (offset & 0x800) ? offset | ~0x000007ff : offset & 0x000007ff;
    uint32_t addr = reg[instr->rs1] + offset;

    fprintf(VHDLFile, "\t\t-- load instruction %d\n", instrCount);
    fprintf(VHDLFile, "\t\t\tck <= '0';\n");

    switch(instr->funct3)
    {
        case F3_SB: // Store a uint8_t
            if (memsChecked == 0)
            {
                fprintf(VHDLFile, "\t\t\tinstr <= x\"%.8x\"; -- STRB : dataMem[reg[%.2d] + %d] = reg[%.2d]\n", instr->instr, instr->rs1, offset, instr->rs2);
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
            }
            else
            {
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
                fprintf(VHDLFile, "\t\t\tassert instr = x\"%.8x\" report \"instruction error at step %d\" severity error;\n", instr->instr, instrCount*2);
            }

            if (excel) fprintf(VHDLFile, "\t\t\tassert false report \"%d;0x%.8x;STRB : dataMem[reg[%.2d] + %d] = reg[%.2d];OK; ;\" severity note;\n",instrCount, instr->instr,  instr->rs1, offset, instr->rs2);
            fprintf(VHDLFile, "\t\t\tassert progcounter = x\"%.8x\" report \"progcounter error at step %d\" severity error;\n", PC, instrCount*2);
            fprintf(VHDLFile, "\t\t\tassert dataAddr = x\"%.8x\"    report \"address error at step %d\"     severity error;\n", addr, instrCount*2);
            fprintf(VHDLFile, "\t\t\tassert inputData = x\"%.8x\"   report \"data error at step  %d\"       severity error;\n",reg[instr->rs2], instrCount*2);
            fprintf(VHDLFile, "\t\t\tassert dataLength = \"000\"        report \"length error at step %d\"      severity error;\n", instrCount*2);
            fprintf(VHDLFile, "\t\t\tassert store = '1'               report \"store error at step %d\"       severity error;\n", instrCount*2);

            writeDataMemory(addr, reg[instr->rs2], sizeof(uint8_t));

        break;

        case F3_SH: // Store a uint16_t
            if (memsChecked == 0)
            {
                fprintf(VHDLFile, "\t\t\tinstr <= x\"%.8x\"; -- STRH : dataMem[reg[%.2d] + %d] = reg[%.2d]\n", instr->instr, instr->rs1, offset, instr->rs2);
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
            }
            else
            {
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
                fprintf(VHDLFile, "\t\t\tassert instr = x\"%.8x\" report \"instruction error at step %d\" severity error;\n", instr->instr, instrCount*2);
            }

            if (excel) fprintf(VHDLFile, "\t\t\tassert false report \"%d;0x%.8x;STRH : dataMem[reg[%.2d] + %d] = reg[%.2d];OK; ;\" severity note;\n",instrCount, instr->instr,  instr->rs1, offset, instr->rs2);
            fprintf(VHDLFile, "\t\t\tassert progcounter = x\"%.8x\" report \"progcounter error at step %d\" severity error;\n", PC, instrCount*2);
            fprintf(VHDLFile, "\t\t\tassert dataAddr = x\"%.8x\"    report \"address error at step %d\"     severity error;\n", addr, instrCount*2);
            fprintf(VHDLFile, "\t\t\tassert inputData = x\"%.8x\"   report \"data error at step  %d\"       severity error;\n",reg[instr->rs2], instrCount*2);
            fprintf(VHDLFile, "\t\t\tassert dataLength = \"001\"        report \"length error at step %d\"      severity error;\n", instrCount*2);
            fprintf(VHDLFile, "\t\t\tassert store = '1'               report \"store error at step %d\"       severity error;\n", instrCount*2);

            writeDataMemory(addr, reg[instr->rs2], sizeof(uint16_t));

        break;

        case F3_SW: // Store a uint32_t
            if (memsChecked == 0)
            {
                fprintf(VHDLFile, "\t\t\tinstr <= x\"%.8x\"; -- STRW : dataMem[reg[%.2d] + %d] = reg[%.2d]\n", instr->instr, instr->rs1, offset, instr->rs2);
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
            }
            else
            {
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
                fprintf(VHDLFile, "\t\t\tassert instr = x\"%.8x\" report \"instruction error at step %d\" severity error;\n", instr->instr, instrCount*2);
            }

            if (excel) fprintf(VHDLFile, "\t\t\tassert false report \"%d;0x%.8x;STRW : dataMem[reg[%.2d] + %d] = reg[%.2d];OK; ;\" severity note;\n",instrCount, instr->instr,  instr->rs1, offset, instr->rs2);
            fprintf(VHDLFile, "\t\t\tassert progcounter = x\"%.8x\" report \"progcounter error at step %d\" severity error;\n", PC, instrCount*2);
            fprintf(VHDLFile, "\t\t\tassert dataAddr = x\"%.8x\"    report \"address error at step %d\"     severity error;\n", addr, instrCount*2);
            fprintf(VHDLFile, "\t\t\tassert inputData = x\"%.8x\"   report \"data error at step  %d\"       severity error;\n",reg[instr->rs2], instrCount*2);
            fprintf(VHDLFile, "\t\t\tassert dataLength = \"010\"        report \"length error at step %d\"      severity error;\n", instrCount*2);
            fprintf(VHDLFile, "\t\t\tassert store = '1'               report \"store error at step %d\"       severity error;\n", instrCount*2);

            writeDataMemory(addr, reg[instr->rs2], sizeof(uint32_t));

        break;
    }
    PC = PC + 4;

    fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
    fprintf(VHDLFile, "\n");
    fprintf(VHDLFile, "\t\t-- execute instruction %d\n", instrCount);
    fprintf(VHDLFile, "\t\t\tck <= '1';\n");
    fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
    if (regsChecked == 1)
    {
        assertRegs(instrCount*2+1, VHDLFile);
    }
    fprintf(VHDLFile, "\t\t\tassert progcounter = x\"%.8x\" report \"progcounter error at step %d\" severity error;\n", PC, instrCount*2+1);
    fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
    fprintf(VHDLFile, "\n");
}

/* IMM OPcode type subroutine */
void executeOP_IMM(struct command* instr, FILE *VHDLFile)
{
    uint32_t immExt = (instr->imm12 & 0x800) ? instr->imm12 | ~0x000007ff : instr->imm12 & 0x000007ff;
    uint32_t shift = instr->imm12 & 0x1f;

    fprintf(VHDLFile, "\t\t-- load instruction %d\n", instrCount);
    fprintf(VHDLFile, "\t\t\tck <= '0';\n");
    switch(instr->funct3)
    {
        case F3_ADDI: // ADD with Immediate
            if (memsChecked == 0)
            {
                fprintf(VHDLFile, "\t\t\tinstr <= x\"%.8x\"; -- ADDI : reg[%.2d] = reg[%.2d] + %d\n", instr->instr, instr->rd, instr->rs1, immExt);
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
            }
            else
            {
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
                fprintf(VHDLFile, "\t\t\tassert instr = x\"%.8x\" report \"instruction error at step %d\" severity error;\n", instr->instr, instrCount*2);
            }

            if (excel) fprintf(VHDLFile, "\t\t\tassert false report \"%d;0x%.8x;ADDI : reg[%.2d] = reg[%.2d] + %d;OK; ;\" severity note;\n",instrCount, instr->instr, instr->rd, instr->rs1, immExt);
            fprintf(VHDLFile, "\t\t\tassert progcounter = x\"%.8x\" report \"progcounter error at step %d\" severity error;\n", PC, instrCount*2);

            reg[instr->rd] = reg[instr->rs1] + immExt;
        break;

        case F3_SLTI: // Logical compare with Immediate and sign consideration
            if (memsChecked == 0)
            {
                fprintf(VHDLFile, "\t\t\tinstr <= x\"%.8x\"; -- SLTI : reg[%.2d] = ( reg[%.2d] < %d ) ? 1 : 0\n", instr->instr, instr->rd, instr->rs1, immExt);
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
            }
            else
            {
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
                fprintf(VHDLFile, "\t\t\tassert instr = x\"%.8x\" report \"instruction error at step %d\" severity error;\n", instr->instr, instrCount*2);
            }
            if (excel) fprintf(VHDLFile, "\t\t\tassert false report \"%d;0x%.8x;SLTI : reg[%.2d] = ( reg[%.2d] < %d ) ? 1 : 0;OK; ;\" severity note;\n",instrCount, instr->instr, instr->rd, instr->rs1, immExt);
            fprintf(VHDLFile, "\t\t\tassert progcounter = x\"%.8x\" report \"progcounter error at step %d\" severity error;\n", PC, instrCount*2);

            reg[instr->rd] = ((int32_t) reg[instr->rs1] < (int32_t) immExt ) ? 1 : 0;
        break;

        case F3_SLTIU: // Logical compare with Immediate and without sign consideration
            if (memsChecked == 0)
            {
                fprintf(VHDLFile, "\t\t\tinstr <= x\"%.8x\"; -- SLTIU : reg[%.2d] = ( reg[%.2d] < %d ) ? 1 : 0\n", instr->instr, instr->rd, instr->rs1, immExt);
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
            }
            else
            {
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
                fprintf(VHDLFile, "\t\t\tassert instr = x\"%.8x\" report \"instruction error at step %d\" severity error;\n", instr->instr, instrCount*2);
            }

            if (excel) fprintf(VHDLFile, "\t\t\tassert false report \"%d;0x%.8x;SLTIU : reg[%.2d] = ( reg[%.2d] < %d ) ? 1 : 0;OK; ;\" severity note;\n",instrCount, instr->instr, instr->rd, instr->rs1, immExt);
            fprintf(VHDLFile, "\t\t\tassert progcounter = x\"%.8x\" report \"progcounter error at step %d\" severity error;\n", PC, instrCount*2);

            reg[instr->rd] = ( reg[instr->rs1] < immExt ) ? 1 : 0;
        break;

        case F3_XORI: // XOR with Immediate
            if (memsChecked == 0)
            {
                fprintf(VHDLFile, "\t\t\tinstr <= x\"%.8x\"; -- XORI: reg[%.2d] = reg[%.2d] ^ %d\n", instr->instr, instr->rd, instr->rs1, immExt);
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
            }
            else
            {
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
                fprintf(VHDLFile, "\t\t\tassert instr = x\"%.8x\" report \"instruction error at step %d\" severity error;\n", instr->instr, instrCount*2);
            }

            if (excel) fprintf(VHDLFile, "\t\t\tassert false report \"%d;0x%.8x;XORI: reg[%.2d] = reg[%.2d] ^ %d;OK; ;\" severity note;\n",instrCount, instr->instr, instr->rd, instr->rs1, immExt);
            fprintf(VHDLFile, "\t\t\tassert progcounter = x\"%.8x\" report \"progcounter error at step %d\" severity error;\n", PC, instrCount*2);

            reg[instr->rd] = reg[instr->rs1] ^ immExt;
        break;

        case F3_ORI: // OR with Immediate
            if (memsChecked == 0)
            {
                fprintf(VHDLFile, "\t\t\tinstr <= x\"%.8x\"; -- ORI: reg[%.2d] = reg[%.2d] | %d\n", instr->instr, instr->rd, instr->rs1, immExt);
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
            }
            else
            {
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
                fprintf(VHDLFile, "\t\t\tassert instr = x\"%.8x\" report \"instruction error at step %d\" severity error;\n", instr->instr, instrCount*2);
            }

            if (excel) fprintf(VHDLFile, "\t\t\tassert false report \"%d;0x%.8x;ORI: reg[%.2d] = reg[%.2d] | %d;OK; ;\" severity note;\n",instrCount, instr->instr, instr->rd, instr->rs1, immExt);
            fprintf(VHDLFile, "\t\t\tassert progcounter = x\"%.8x\" report \"progcounter error at step %d\" severity error;\n", PC, instrCount*2);

            reg[instr->rd] = reg[instr->rs1] | immExt;
        break;

        case F3_ANDI: // AND with Immediate
            if (memsChecked == 0)
            {
                fprintf(VHDLFile, "\t\t\tinstr <= x\"%.8x\"; -- ANDI: reg[%.2d] = reg[%.2d] & %d\n", instr->instr, instr->rd, instr->rs1, immExt);
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
            }
            else
            {
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
                fprintf(VHDLFile, "\t\t\tassert instr = x\"%.8x\" report \"instruction error at step %d\" severity error;\n", instr->instr, instrCount*2);
            }

            if (excel) fprintf(VHDLFile, "\t\t\tassert false report \"%d;0x%.8x;ANDI: reg[%.2d] = reg[%.2d] & %d;OK; ;\" severity note;\n",instrCount, instr->instr, instr->rd, instr->rs1, immExt);
            fprintf(VHDLFile, "\t\t\tassert progcounter = x\"%.8x\" report \"progcounter error at step %d\" severity error;\n", PC, instrCount*2);

            reg[instr->rd] = reg[instr->rs1] & immExt;
        break;

        case F3_SLLI: // Immediate Left Logical Shift
            if (memsChecked == 0)
            {
                fprintf(VHDLFile, "\t\t\tinstr <= x\"%.8x\"; -- SLLI : reg[%.2d] = reg[%.2d] << %d & 0x1f\n", instr->instr, instr->rd, instr->rs1, shift);
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
            }
            else
            {
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
                fprintf(VHDLFile, "\t\t\tassert instr = x\"%.8x\" report \"instruction error at step %d\" severity error;\n", instr->instr, instrCount*2);
            }

            if (excel) fprintf(VHDLFile, "\t\t\tassert false report \"%d;0x%.8x;SLLI : reg[%.2d] = reg[%.2d] << %d & 0x1f;OK; ;\" severity note;\n",instrCount, instr->instr, instr->rd, instr->rs1, shift);
            fprintf(VHDLFile, "\t\t\tassert progcounter = x\"%.8x\" report \"progcounter error at step %d\" severity error;\n", PC, instrCount*2);

            reg[instr->rd] = reg[instr->rs1] << shift;
        break;

        case F3_SRLI:
        //case F3_SRAI: duplicate value
            switch(instr->funct7)
            {
                case F7_SRLI: // Immediate Right Logical Shift
                    if (memsChecked == 0)
                    {
                        fprintf(VHDLFile, "\t\t\tinstr <= x\"%.8x\"; -- SRLI : reg[%.2d] = reg[%.2d] >> %d & 0x1f\n", instr->instr, instr->rd, instr->rs1, shift);
                        fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
                    }
                    else
                    {
                        fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
                        fprintf(VHDLFile, "\t\t\tassert instr = x\"%.8x\" report \"instruction error at step %d\" severity error;\n", instr->instr, instrCount*2);
                    }

                    if (excel) fprintf(VHDLFile, "\t\t\tassert false report \"%d;0x%.8x;SRLI : reg[%.2d] = reg[%.2d] >> %d & 0x1f;OK; ;\" severity note;\n",instrCount, instr->instr, instr->rd, instr->rs1, shift);
                    fprintf(VHDLFile, "\t\t\tassert progcounter = x\"%.8x\" report \"progcounter error at step %d\" severity error;\n", PC, instrCount*2);

                    reg[instr->rd] = reg[instr->rs1] >> shift;
                break;
                case F7_SRAI: // Aritmethic Right Logical Shift
                    if (memsChecked == 0)
                    {
                        fprintf(VHDLFile, "\t\t\tinstr <= x\"%.8x\"; -- SRAI : reg[%.2d] = (int) reg[%.2d] >> (int) %d & 0x1f\n", instr->instr, instr->rd, instr->rs1, shift);
                        fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
                    }
                    else
                    {
                        fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
                        fprintf(VHDLFile, "\t\t\tassert instr = x\"%.8x\" report \"instruction error at step %d\" severity error;\n", instr->instr, instrCount*2);
                    }

                    if (excel) fprintf(VHDLFile, "\t\t\tassert false report \"%d;0x%.8x;SRAI : reg[%.2d] = (int) reg[%.2d] >> (int) %d & 0x1f;OK; ;\" severity note;\n",instrCount, instr->instr, instr->rd, instr->rs1, shift);
                    fprintf(VHDLFile, "\t\t\tassert progcounter = x\"%.8x\" report \"progcounter error at step %d\" severity error;\n", PC, instrCount*2);

                    reg[instr->rd] = (int32_t) reg[instr->rs1] >> (int32_t) shift;
                break;
            }
        break;
    }
    PC = PC + 4;

    fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
    fprintf(VHDLFile, "\n");
    fprintf(VHDLFile, "\t\t-- execute instruction %d\n", instrCount);
    fprintf(VHDLFile, "\t\t\tck <= '1';\n");
    fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
    if (regsChecked == 1)
    {
        assertRegs(instrCount*2+1, VHDLFile);
    }
    fprintf(VHDLFile, "\t\t\tassert progcounter = x\"%.8x\" report \"progcounter error at step %d\" severity error;\n", PC, instrCount*2+1);
    fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
    fprintf(VHDLFile, "\n");
}

/* OP OPcode type subroutine */
void executeOP(struct command* instr, FILE *VHDLFile)
{
    fprintf(VHDLFile, "\t\t-- load instruction %d\n", instrCount);
    fprintf(VHDLFile, "\t\t\tck <= '0';\n");

    uint32_t shift = reg[instr->rs2] & 0x1f;

    switch(instr->funct3)
    {
        case F3_ADD:
        // case F3_SUB: duplicate value
            switch(instr->funct7)
            {
                case F7_ADD: // ADD
                    if (memsChecked == 0)
                    {
                        fprintf(VHDLFile, "\t\t\tinstr <= x\"%.8x\"; -- ADD: reg[%.2d] = reg[%.2d] + reg[%.2d]\n", instr->instr, instr->rd, instr->rs1, instr->rs2);
                        fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
                    }
                    else
                    {
                        fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
                        fprintf(VHDLFile, "\t\t\tassert instr = x\"%.8x\" report \"instruction error at step %d\" severity error;\n", instr->instr, instrCount*2);
                    }

                    if (excel) fprintf(VHDLFile, "\t\t\tassert false report \"%d;0x%.8x;ADD: reg[%.2d] = reg[%.2d] + reg[%.2d];OK; ;\" severity note;\n",instrCount, instr->instr, instr->rd, instr->rs1, instr->rs2);
                    fprintf(VHDLFile, "\t\t\tassert progcounter = x\"%.8x\" report \"progcounter error at step %d\" severity error;\n", PC, instrCount*2);

                    reg[instr->rd] = reg[instr->rs1] + reg[instr->rs2];
                break;

                case F7_SUB: // SUB
                    if (memsChecked == 0)
                    {
                        fprintf(VHDLFile, "\t\t\tinstr <= x\"%.8x\"; -- SUB: reg[%.2d] = reg[%.2d] - reg[%.2d]\n", instr->instr, instr->rd, instr->rs1, instr->rs2);
                        fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
                    }
                    else
                    {
                        fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
                        fprintf(VHDLFile, "\t\t\tassert instr = x\"%.8x\" report \"instruction error at step %d\" severity error;\n", instr->instr, instrCount*2);
                    }
                    if (excel) fprintf(VHDLFile, "\t\t\tassert false report \"%d;0x%.8x;SUB: reg[%.2d] = reg[%.2d] - reg[%.2d];OK; ;\" severity note;\n",instrCount, instr->instr, instr->rd, instr->rs1, instr->rs2);
                    fprintf(VHDLFile, "\t\t\tassert progcounter = x\"%.8x\" report \"progcounter error at step %d\" severity error;\n", PC, instrCount*2);

                    reg[instr->rd] = reg[instr->rs1] - reg[instr->rs2];
                break;
            }
        break;

        case F3_SLL: // Left Logical Shift
            if (memsChecked == 0)
            {
                fprintf(VHDLFile, "\t\t\tinstr <= x\"%.8x\"; -- SLL : reg[%.2d] = reg[%.2d] << reg[%.2d] & 0x1f\n", instr->instr, instr->rd, instr->rs1, instr->rs2);
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
            }
            else
            {
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
                fprintf(VHDLFile, "\t\t\tassert instr = x\"%.8x\" report \"instruction error at step %d\" severity error;\n", instr->instr, instrCount*2);
            }
            if (excel) fprintf(VHDLFile, "\t\t\tassert false report \"%d;0x%.8x;SLL : reg[%.2d] = reg[%.2d] << reg[%.2d] & 0x1f;OK; ;\" severity note;\n",instrCount, instr->instr, instr->rd, instr->rs1, instr->rs2);
            fprintf(VHDLFile, "\t\t\tassert progcounter = x\"%.8x\" report \"progcounter error at step %d\" severity error;\n", PC, instrCount*2);

            reg[instr->rd] = reg[instr->rs1] << shift;
        break;

        case F3_SLT: // Logical compare with Immediate and sign consideration
            if (memsChecked == 0)
            {
                fprintf(VHDLFile, "\t\t\tinstr <= x\"%.8x\"; -- SLT : reg[%.2d] = ( reg[%.2d] < reg[%.2d] ) ? 1 : 0\n", instr->instr, instr->rd, instr->rs1, instr->rs2);
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
            }
            else
            {
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
                fprintf(VHDLFile, "\t\t\tassert instr = x\"%.8x\" report \"instruction error at step %d\" severity error;\n", instr->instr, instrCount*2);
            }
            if (excel) fprintf(VHDLFile, "\t\t\tassert false report \"%d;0x%.8x;SLT : reg[%.2d] = ( reg[%.2d] < reg[%.2d] ) ? 1 : 0;OK; ;\" severity note;\n",instrCount, instr->instr, instr->rd, instr->rs1, instr->rs2);
            fprintf(VHDLFile, "\t\t\tassert progcounter = x\"%.8x\" report \"progcounter error at step %d\" severity error;\n", PC, instrCount*2);

            reg[instr->rd] = ((int32_t) reg[instr->rs1] < (int32_t) reg[instr->rs2] ) ? 1 : 0;
        break;

        case F3_SLTU: // Logical compare with Immediate and withouth sign consideration
            if (memsChecked == 0)
            {
                fprintf(VHDLFile, "\t\t\tinstr <= x\"%.8x\"; -- SLTU: reg[%.2d] = ( reg[%.2d] < reg[%.2d] ) ? 1 : 0\n", instr->instr, instr->rd, instr->rs1, instr->rs2);
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
            }
            else
            {
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
                fprintf(VHDLFile, "\t\t\tassert instr = x\"%.8x\" report \"instruction error at step %d\" severity error;\n", instr->instr, instrCount*2);
            }
            if (excel) fprintf(VHDLFile, "\t\t\tassert false report \"%d;0x%.8x;SLTU: reg[%.2d] = ( reg[%.2d] < reg[%.2d] ) ? 1 : 0;OK; ;\" severity note;\n",instrCount, instr->instr, instr->rd, instr->rs1, instr->rs2);
            fprintf(VHDLFile, "\t\t\tassert progcounter = x\"%.8x\" report \"progcounter error at step %d\" severity error;\n", PC, instrCount*2);

            reg[instr->rd] = (reg[instr->rs1] < reg[instr->rs2] ) ? 1 : 0;
        break;

        case F3_XOR: // XOR
            if (memsChecked == 0)
            {
                fprintf(VHDLFile, "\t\t\tinstr <= x\"%.8x\"; -- XOR: reg[%.2d] = reg[%.2d] ^ reg[%.2d]\n", instr->instr, instr->rd, instr->rs1, instr->rs2);
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
            }
            else
            {
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
                fprintf(VHDLFile, "\t\t\tassert instr = x\"%.8x\" report \"instruction error at step %d\" severity error;\n", instr->instr, instrCount*2);
            }
            if (excel) fprintf(VHDLFile, "\t\t\tassert false report \"%d;0x%.8x;XOR: reg[%.2d] = reg[%.2d] ^ reg[%.2d];OK; ;\" severity note;\n",instrCount, instr->instr, instr->rd, instr->rs1, instr->rs2);
            fprintf(VHDLFile, "\t\t\tassert progcounter = x\"%.8x\" report \"progcounter error at step %d\" severity error;\n", PC, instrCount*2);

            reg[instr->rd] = reg[instr->rs1] ^ reg[instr->rs2];
        break;

        case F3_SRL:
        // case F3_SRA: duplicate value
            switch(instr->funct7)
            {
                case F7_SRL: //Right Logical Shift
                    if (memsChecked == 0)
                    {
                        fprintf(VHDLFile, "\t\t\tinstr <= x\"%.8x\"; -- SRL : reg[%.2d] = reg[%.2d] >> reg[%.2d] & 0x1f\n", instr->instr, instr->rd, instr->rs1, instr->rs2);
                        fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
                    }
                    else
                    {
                        fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
                        fprintf(VHDLFile, "\t\t\tassert instr = x\"%.8x\" report \"instruction error at step %d\" severity error;\n", instr->instr, instrCount*2);
                    }
                    if (excel) fprintf(VHDLFile, "\t\t\tassert false report \"%d;0x%.8x;SRL : reg[%.2d] = reg[%.2d] >> reg[%.2d] & 0x1f;OK; ;\" severity note;\n",instrCount, instr->instr, instr->rd, instr->rs1, instr->rs2);
                    fprintf(VHDLFile, "\t\t\tassert progcounter = x\"%.8x\" report \"progcounter error at step %d\" severity error;\n", PC, instrCount*2);

                    reg[instr->rd] = reg[instr->rs1] >> shift;
                break;

                case F7_SRA: // Right arithmetic Shift
                    if (memsChecked == 0)
                    {
                        fprintf(VHDLFile, "\t\t\tinstr <= x\"%.8x\"; -- SRA: reg[%.2d] = (int) reg[%.2d] >> (int) reg[%.2d] & 0x1f\n", instr->instr, instr->rd, instr->rs1, instr->rs2);
                        fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
                    }
                    else
                    {
                        fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
                        fprintf(VHDLFile, "\t\t\tassert instr = x\"%.8x\" report \"instruction error at step %d\" severity error;\n", instr->instr, instrCount*2);
                    }
                    if (excel) fprintf(VHDLFile, "\t\t\tassert false report \"%d;0x%.8x;SRA: reg[%.2d] = (int) reg[%.2d] >> (int) reg[%.2d] & 0x1f;OK; ;\" severity note;\n",instrCount, instr->instr, instr->rd, instr->rs1, instr->rs2);
                    fprintf(VHDLFile, "\t\t\tassert progcounter = x\"%.8x\" report \"progcounter error at step %d\" severity error;\n", PC, instrCount*2);

                    reg[instr->rd] = (int32_t) reg[instr->rs1] >> shift;
                break;
            }
        break;

        case F3_OR: // OR
            if (memsChecked == 0)
            {
                fprintf(VHDLFile, "\t\t\tinstr <= x\"%.8x\"; -- OR: reg[%.2d] = reg[%.2d] | reg[%.2d]\n", instr->instr, instr->rd, instr->rs1, instr->rs2);
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
            }
            else
            {
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
                fprintf(VHDLFile, "\t\t\tassert instr = x\"%.8x\" report \"instruction error at step %d\" severity error;\n", instr->instr, instrCount*2);
            }
            if (excel) fprintf(VHDLFile, "\t\t\tassert false report \"%d;0x%.8x;OR: reg[%.2d] = reg[%.2d] | reg[%.2d];OK; ;\" severity note;\n",instrCount, instr->instr, instr->rd, instr->rs1, instr->rs2);
            fprintf(VHDLFile, "\t\t\tassert progcounter = x\"%.8x\" report \"progcounter error at step %d\" severity error;\n", PC, instrCount*2);

            reg[instr->rd] = reg[instr->rs1] | reg[instr->rs2];
        break;

        case F3_AND:// AND
            if (memsChecked == 0)
            {
                fprintf(VHDLFile, "\t\t\tinstr <= x\"%.8x\"; -- AND: reg[%.2d] = reg[%.2d] & reg[%.2d]\n", instr->instr, instr->rd, instr->rs1, instr->rs2);
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
            }
            else
            {
                fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
                fprintf(VHDLFile, "\t\t\tassert instr = x\"%.8x\" report \"instruction error at step %d\" severity error;\n", instr->instr, instrCount*2);
            }
            if (excel) fprintf(VHDLFile, "\t\t\tassert false report \"%d;0x%.8x;AND: reg[%.2d] = reg[%.2d] & reg[%.2d];OK; ;\" severity note;\n",instrCount, instr->instr, instr->rd, instr->rs1, instr->rs2);
            fprintf(VHDLFile, "\t\t\tassert progcounter = x\"%.8x\" report \"progcounter error at step %d\" severity error;\n", PC, instrCount*2);

            reg[instr->rd] = reg[instr->rs1] & reg[instr->rs2];

        break;
    }
    PC = PC + 4;

    fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
    fprintf(VHDLFile, "\n");
    fprintf(VHDLFile, "\t\t-- execute instruction %d\n", instrCount);
    fprintf(VHDLFile, "\t\t\tck <= '1';\n");
    fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
    if (regsChecked == 1)
    {
        assertRegs(instrCount*2+1, VHDLFile);
    }
    fprintf(VHDLFile, "\t\t\tassert progcounter = x\"%.8x\" report \"progcounter error at step %d\" severity error;\n", PC, instrCount*2+1);
    fprintf(VHDLFile, "\t\t\twait for 5 ns;\n");
    fprintf(VHDLFile, "\n");
}

void executeMISC_MEM(struct command* instr, FILE *VHDLFile){}
void executeSYSTEM(struct command* instr, FILE *VHDLFile){}