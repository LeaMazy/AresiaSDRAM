#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

uint8_t* instrMem = NULL;
uint32_t instrSize = 0;
uint32_t dataMemSize = 0;
void readInstrFile(const char* filepath);

void initVHDLFileInstr8(FILE **VHDLFile, const char* filepath);
void fillInstrMemInstr8(FILE *VHDLFile);
void endVHDLFileInstr8(FILE *VHDLFile);

void initVHDLFileInstr32(FILE **VHDLFile, const char* filepath);
void fillInstrMemInstr32(FILE *VHDLFile);
void endVHDLFileInstr32(FILE *VHDLFile);

void initVHDLFileData(FILE **VHDLFile, const char* filepath);
void fillInstrMemData(FILE *VHDLFile);
void endVHDLFileData(FILE *VHDLFile);

void main(int argc, char** argv)
{
    /* Declare a pointer that will point on VHDL Test bench file*/
    FILE *VHDLFileInstr;
    FILE *VHDLFileData;
    
    /* Define Ram size used with hex value as input */
    dataMemSize = (uint32_t)strtol(argv[1], NULL, 16);
	dataMemSize = dataMemSize*4;
    
    /* Read the file passed in argument of the script execution */
    readInstrFile(argv[2]);
    
    /* Init VHDL file with instruction using 8 bits word */
    initVHDLFileInstr8(&VHDLFileInstr, argv[3]);
    fillInstrMemInstr8(VHDLFileInstr);
    endVHDLFileInstr8(VHDLFileInstr);
    
    /* Init VHDL file with instruction using 32 bits word */
    initVHDLFileInstr32(&VHDLFileInstr, argv[4]);
    fillInstrMemInstr32(VHDLFileInstr);
    endVHDLFileInstr32(VHDLFileInstr);
    
    /* Init VHDL file that will contain the whole ram in 8 bits word */
    initVHDLFileData(&VHDLFileData, argv[5]);
    fillInstrMemData(VHDLFileData);
    endVHDLFileData(VHDLFileData);
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
}

/* Function that init the VHDL instruction file with 8 bit word format */
void initVHDLFileInstr8(FILE **VHDLFile, const char* filepath)
{
    /* Open the vhd file in write mode to erase it */
    *VHDLFile = fopen(filepath, "w");

    char* text =  
        "-- Projet de fin d'études : RISC-V\n"
        "-- ECE Paris / SECAPEM\n"
        "-- Instruction Memory VHDL\n"
        "\n"
        "-- LIBRARIES\n"
        "library ieee;\n"
        "use ieee.std_logic_1164.all;\n"
        "use ieee.numeric_std.all;\n"
        "\n"
        "-- ENTITY\n"
        "entity InstructionMemory is\n"
        "\tport (\n"
        "\t\t-- INPUTS\n"
        "\t\tIMclock\t\t\t: in std_logic;\n"
        "\t\tIMreset\t\t\t: in std_logic;\n"
        "\t\tIMprogcounter\t: in std_logic_vector(31 downto 0);\n"
        "\t\t-- OUTPUTS\n"
        "\t\tIMout\t\t\t\t: out std_logic_vector(31 downto 0)\n"
        "\t);\n"
        "end entity;\n"
        "\n"
        "-- ARCHITECTURE\n"
        "architecture archi of InstructionMemory is\n"
        "\ttype mem is array(0 to "
    ;
    /* write string sequence in VHDL file*/
    fprintf(*VHDLFile, "%s%d", text, (instrSize)-1);
    
    text =
        ") of std_logic_vector(7 downto 0);\n"
        "\tsignal SigIMmemory : mem :=(\n"
        "\t\t"
    ;
    /* write string sequence in VHDL file*/
    fprintf(*VHDLFile, "%s", text);
}

/* Function that init the VHDL instruction file with 32 bit word format */
void initVHDLFileInstr32(FILE **VHDLFile, const char* filepath)
{
    /* Open the vhd file in write mode to erase it */
    *VHDLFile = fopen(filepath, "w");

    char* text =  
        "-- Projet de fin d'études : RISC-V\n"
        "-- ECE Paris / SECAPEM\n"
        "-- Instruction Memory VHDL\n"
        "\n"
        "-- LIBRARIES\n"
        "library ieee;\n"
        "use ieee.std_logic_1164.all;\n"
        "use ieee.numeric_std.all;\n"
        "\n"
        "-- ENTITY\n"
        "entity InstructionMemory is\n"
        "\tport (\n"
        "\t\t-- INPUTS\n"
        "\t\tIMclock\t\t\t: in std_logic;\n"
        "\t\tIMreset\t\t\t: in std_logic;\n"
        "\t\tIMprogcounter\t: in std_logic_vector(31 downto 0);\n"
        "\t\t-- OUTPUTS\n"
        "\t\tIMout\t\t\t\t: out std_logic_vector(31 downto 0)\n"
        "\t);\n"
        "end entity;\n"
        "\n"
        "-- ARCHITECTURE\n"
        "architecture archi of InstructionMemory is\n"
        "\ttype mem is array(0 to "
    ;
    /* write string sequence in VHDL file*/
    fprintf(*VHDLFile, "%s%d", text, (instrSize/4)-1);
    
    text =
        ") of std_logic_vector(31 downto 0);\n"
        "\tsignal SigIMmemory : mem :=("
    ;
    /* write string sequence in VHDL file*/
    fprintf(*VHDLFile, "%s", text);
}

/* Fill the memInstr array */
void fillInstrMemInstr8(FILE *VHDLFile)
{     
    for (uint16_t i = 0 ; i < instrSize ; i++)
    {
        if (i % 32 == 0 && i !=0) fprintf(VHDLFile, "\n\t\t");
        if (i % 4 == 0 && i % 32 !=0) fprintf(VHDLFile,"  ");
        fprintf(VHDLFile, "x\"%.2x\"", instrMem[i]);
        if (i != instrSize - 1) fprintf(VHDLFile, ",");
    }
}

/* Fill the memInstr array */
void fillInstrMemInstr32(FILE *VHDLFile)
{     
    for (uint16_t i = 0 ; i < instrSize ; i++)
    {
        if (i % 4 == 0) fprintf(VHDLFile, "\n\t\tx\"");       
        fprintf(VHDLFile, "%.2x", instrMem[i]);
        if (i % 4 == 3) fprintf(VHDLFile, "\"");
        if (i % 4 == 3 && i != instrSize - 1) fprintf(VHDLFile, ",");
    }
}

/* Function that close the VHDL test file*/
void endVHDLFileInstr32(FILE *VHDLFile)
{
    char* text =
        "\n\t);\n"
        "\t\n"
        "\tsignal sigad : integer;\n"
        "\tsignal sigpc : std_logic_vector(2 downto 0);\n"
        "\t\n"
        "begin\n"
        "\n"
        "\tsigpc <= IMprogcounter(4 downto 2);\n"
        "\tIMout <= SigIMmemory(sigad) when rising_edge(IMclock);\n"
    ;
    
    fprintf(VHDLFile, "%s", text);
    fprintf(VHDLFile, "\tSigad <= 0 when ((unsigned(sigpc) > %d)) else to_integer(unsigned(sigpc));\n", (instrSize/4)-1);
    text = 
        "\n"
        "end archi;\n"
        "-- END FILE"
    ;
    /* write string sequence in VHDL file*/
    fprintf(VHDLFile, "%s", text);
    /* close file */
    fclose(VHDLFile);
}

/* Function that close the VHDL test file*/
void endVHDLFileInstr8(FILE *VHDLFile)
{
    char* text =
        "\n\t);\n"
        "\t\n"
        "\tsignal SigIMprogcounter00 : integer;\n"
        "\tsignal SigIMprogcounter08 : integer;\n"
        "\tsignal SigIMprogcounter16 : integer;\n"
        "\tsignal SigIMprogcounter24 : integer;\n"
        "\t\n"
        "begin\n"
        "\n"
    ;
    
    fprintf(VHDLFile, "%s", text);
    fprintf(VHDLFile, "\tSigIMprogcounter00 <= 0 when ((unsigned(IMprogcounter) > %d) OR IMreset = '1') else to_integer(unsigned(IMprogcounter));\n", (instrSize)-1);
    fprintf(VHDLFile, "\tSigIMprogcounter08 <= 0 when ((unsigned(IMprogcounter) > %d) OR IMreset = '1') else to_integer(unsigned(IMprogcounter) + 1);\n", (instrSize)-1);
    fprintf(VHDLFile, "\tSigIMprogcounter16 <= 0 when ((unsigned(IMprogcounter) > %d) OR IMreset = '1') else to_integer(unsigned(IMprogcounter) + 2);\n", (instrSize)-1);
    fprintf(VHDLFile, "\tSigIMprogcounter24 <= 0 when ((unsigned(IMprogcounter) > %d) OR IMreset = '1') else to_integer(unsigned(IMprogcounter) + 3);\n", (instrSize)-1);
   
    text = 
        "\n"
        "\tIMout(7 downto 0)   <= SigIMmemory(SigIMprogcounter00) when (SigIMprogcounter00 >= 0) else x\"00\";\n"
        "\tIMout(15 downto 8)  <= SigIMmemory(SigIMprogcounter08) when (SigIMprogcounter08 >= 0) else x\"00\";\n"
        "\tIMout(23 downto 16) <= SigIMmemory(SigIMprogcounter16) when (SigIMprogcounter16 >= 0) else x\"00\";\n"
        "\tIMout(31 downto 24) <= SigIMmemory(SigIMprogcounter24) when (SigIMprogcounter24 >= 0) else x\"00\";\t\n"
        "\n"
        "end archi;\n"
        "-- END FILE"
    ;
    /* write string sequence in VHDL file*/
    fprintf(VHDLFile, "%s", text);
    /* close file */
    fclose(VHDLFile);
}

void initVHDLFileData(FILE **VHDLFile, const char* filepath)
{
    /* Open the vhd file in write mode to erase it */
    *VHDLFile = fopen(filepath, "w");

    char* text =      
        "-- Projet de fin d'études : RISC-V\n"
        "-- ECE Paris / SECAPEM\n"
        "-- Data Memory VHDL\n"
        "\n"
        "-- LIBRARIES\n"
        "library ieee;\n"
        "use ieee.std_logic_1164.all;\n"
        "use ieee.numeric_std.all;\n"
        "\n"
        "-- ENTITY\n"
        "entity DataMemory is\n"
        "\tport (\n"
        "\t\t-- INPUTS\n"
        "\t\tDMclock\t\t: in std_logic;\n"
        "\t\tDMreset\t\t: in std_logic;\n"
        "\t\tDMstore\t\t: in std_logic;\n"
        "\t\tDMload\t\t: in std_logic;\n"
        "\t\tDMaddr\t\t: in std_logic_vector(31 downto 0);\n"
        "\t\tDMin\t\t\t: in std_logic_vector(31 downto 0);\n"
        "\t\tDMfunct3\t\t: in std_logic_vector(2 downto 0);\n"
        "\t\t-- OUTPUTS\n"
        "\t\tDMout\t\t\t: out std_logic_vector(31 downto 0)\n"
        "\t);\n"
        "end entity;\n"
        "\n"
        "-- ARCHITECTURE\n"
        "architecture archi of DataMemory is\n"
        "\ttype mem is array(0 to "
    ;
    /* write string sequence in VHDL file*/
    fprintf(*VHDLFile, "%s%d", text, (dataMemSize/16)-1);
    
    text =
        ") of std_logic_vector(31 downto 0);\n"
        "\tsignal SigDMmemory : mem :=(\n"
        "\t\t"
    ;
    /* write string sequence in VHDL file*/
    fprintf(*VHDLFile, "%s", text);       
}

void fillInstrMemData(FILE *VHDLFile)
{
	fprintf(VHDLFile, "x\"");
    for (uint16_t i = 0 ; i < dataMemSize/4 ; i=i+4)
    {
        if (i % 32 == 0 && i !=0) fprintf(VHDLFile, "\",\n\t\tx\"");
        if (i % 4 == 0 && i % 32 !=0) fprintf(VHDLFile,"\" , x\"");
		//if (i % 4 == 0 && i % 32 !=0) fprintf(VHDLFile," , ");
        if (i < instrSize) 
        { 	
			//if (i % 4 == 0 && i % 32 !=0 || i % 32 == 0 && i !=0){
				// fprintf(VHDLFile,"\" , x\"");
			fprintf(VHDLFile, "\%.2x", instrMem[i+3]);
			fprintf(VHDLFile, "\%.2x", instrMem[i+2]);
			fprintf(VHDLFile, "\%.2x", instrMem[i+1]);
			fprintf(VHDLFile, "\%.2x", instrMem[i]);
			//}

			//fprintf(VHDLFile, "x\%.2x\"", instrMem[i]);
        }
        else
        {
            fprintf(VHDLFile, "00000000");
			//fprintf(VHDLFile, "x\"00\"");
        }
        if (i != dataMemSize - 1) fprintf(VHDLFile, "");
    }   
	fprintf(VHDLFile, "\"");
}

void endVHDLFileData(FILE *VHDLFile)
{
    char* text =  
        "\n\t);\n"
        "\t\n"
        "\tsignal SigDMaddr00 : integer;\n"
        "\tsignal SigDMaddr08 : integer;\n"
        "\tsignal SigDMaddr16 : integer;\n"
        "\tsignal SigDMaddr24 : integer;\n"
        "\tsignal intAddr : std_logic_vector(13 downto 0);\n"
        "\tsignal cs : std_logic;\n"
        "begin\n"
        "\t-- BEGIN\n"
        "\t\n"
        "\tintAddr <= DMaddr(13 downto 0);\n"
        "\tcs <= not DMaddr(31);\n"
        "\t\n"
        "\t-- useful adresses used to store or load\n"
        "\tSigDMaddr00 <= to_integer(unsigned(intAddr)) when (cs='1' and (DMload = '1' OR DMstore = '1')) else\n"
        "\t\t\t\t\t\t0;\n"
        "\tSigDMaddr08 <= to_integer(unsigned(intAddr)) + 1 when (cs='1' and (DMload = '1' OR DMstore = '1')) else\n"
        "\t\t\t\t\t\t0;\n"
        "\tSigDMaddr16 <= to_integer(unsigned(intAddr)) + 2 when (cs='1' and (DMload = '1' OR DMstore = '1')) else\n"
        "\t\t\t\t\t\t0;\n"
        "\tSigDMaddr24 <= to_integer(unsigned(intAddr)) + 3 when (cs='1' and (DMload = '1' OR DMstore = '1')) else\n"
        "\t\t\t\t\t\t0;\n"
        "\t\n"
        "\t-- store for synchronous data memory\n"
        "\tp2 : process(DMclock, DMreset)\n"
        "\tbegin\n"
        "\t\tif(rising_edge(DMclock)) then\n"
        "\t\t\tif(DMstore = '1' and cs='1') then\n"
        "\t\t\t\tif(DMfunct3 = \"000\") then\n"
        "\t\t\t\t\tSigDMmemory(SigDMaddr00) <= DMin(7 downto 0);\n"
        "\t\t\t\telsif(DMfunct3 = \"001\") then\n"
        "\t\t\t\t\tSigDMmemory(SigDMaddr00) <= DMin(7 downto 0);\n"
        "\t\t\t\t\tSigDMmemory(SigDMaddr08) <= DMin(15 downto 8);\n"
        "\t\t\t\t\t\n"
        "\t\t\t\telsif(DMfunct3 = \"010\") then\n"
        "\t\t\t\t\tSigDMmemory(SigDMaddr00) <= DMin(7 downto 0);\n"
        "\t\t\t\t\tSigDMmemory(SigDMaddr08) <= DMin(15 downto 8);\n"
        "\t\t\t\t\tSigDMmemory(SigDMaddr16) <= DMin(23 downto 16);\n"
        "\t\t\t\t\tSigDMmemory(SigDMaddr24) <= DMin(31 downto 24);\n"
        "\t\t\t\tend if;\n"
        "\t\t\tend if;\n"
        "\t\tend if;\n"
        "\tend process;\n"
        "\t\n"
        "\t\n"
        "\t-- load for asynchronous data memory\n"
        "\tDMout(7 downto 0) <= SigDMmemory(SigDMaddr00) when (DMload = '1' and SigDMaddr00<"
    ;
    fprintf(VHDLFile, "%s%d", text, dataMemSize-1 );
    text =
        ") else\n"
        "\t\t\t\t\t\t\t\t(others => '0');\n"
        "\t\t\t\t\t\t\t\t\n"
        "\tDMout(15 downto 8)<= SigDMmemory(SigDMaddr08) when (DMload = '1' AND (DMfunct3 = \"001\" OR DMfunct3 = \"010\" OR DMfunct3 = \"101\") and SigDMaddr00<"
    ;
    fprintf(VHDLFile, "%s%d", text, dataMemSize-1 );
    text =
        ") else\n"
        "\t\t\t\t\t\t\t\t(others => '1') when ( SigDMaddr00<"
    ;
    fprintf(VHDLFile, "%s%d", text, dataMemSize-1 );
    text =
        " and DMload = '1' AND (DMfunct3 = \"000\" AND ((SigDMmemory(SigDMaddr00) AND \"10000000\") = \"10000000\"))) else\n"
        "\t\t\t\t\t\t\t\t(others => '0');\n"
        "\t\n"
        "\tDMout(23 downto 16)<=SigDMmemory(SigDMaddr16) when (SigDMaddr00<"
    ;
    fprintf(VHDLFile, "%s%d", text, dataMemSize-1 );
    text =           
        " and DMload = '1' AND (DMfunct3 = \"010\")) else\n"
        "\t\t\t\t\t\t\t\t(others => '1') when (SigDMaddr00<"
    ;
    fprintf(VHDLFile, "%s%d", text, dataMemSize-1 );
    text =        
        " and DMload = '1' AND ((DMfunct3 = \"000\" AND ((SigDMmemory(SigDMaddr00) AND \"10000000\") = \"10000000\")) OR (DMfunct3 = \"001\" AND ((SigDMmemory(SigDMaddr08) AND \"10000000\") = \"10000000\")))) else\n"
        "\t\t\t\t\t\t\t\t(others => '0');\n"
        "\t\n"
        "\tDMout(31 downto 24)<=SigDMmemory(SigDMaddr24) when (SigDMaddr00<"
    ;
    fprintf(VHDLFile, "%s%d", text, dataMemSize-1 );
    text =    
        " and DMload = '1' AND (DMfunct3 = \"010\")) else\n"
        "\t\t\t\t\t\t\t\t(others => '1') when (SigDMaddr00<"
    ;
    fprintf(VHDLFile, "%s%d", text, dataMemSize-1 );
    text =       
        " and DMload = '1' AND ((DMfunct3 = \"000\" AND ((SigDMmemory(SigDMaddr00) AND \"10000000\") = \"10000000\")) OR (DMfunct3 = \"001\" AND ((SigDMmemory(SigDMaddr08) AND \"10000000\") = \"10000000\")))) else\n"
        "\t\t\t\t\t\t\t\t(others => '0');\n"
        "\t\n"
        "\t-- END\n"
        "end archi;\n"
        "-- END FILE"
    ;
    /* write string sequence in VHDL file*/
    fprintf(VHDLFile, "%s", text);
    /* close file */
    fclose(VHDLFile);
}