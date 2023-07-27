#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

uint8_t* instrMem = NULL;
uint32_t instrSize = 0;
uint32_t dataMemSize = 0;
void readInstrFile(const char* filepath);
void buildHexFile(char* filepath);
void buildHexFile32(char* filepath);
uint8_t cks8(uint8_t byteNb, uint32_t addr, uint8_t fieldType, uint32_t data);

void main(int argc, char** argv)
{
    /* Define Ram size used with hex value as input */
    dataMemSize = (uint32_t)strtol(argv[1], NULL, 16);
    /* Read the file passed in argument of the script execution */
    readInstrFile(argv[2]);
    /* Save the hex files with the name passed in argument 2 */
    buildHexFile(argv[3]);
    /* Save the  instr hex file with the name passed in argument 3 */
    buildHexFile32(argv[4]);
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

/* Function that init the VHDL test file */
void buildHexFile(char* filepath)
{
    uint8_t byteNb = 1;
    uint8_t cks = 0;
    uint8_t fieldType = 0;

    char filespath[4][100];
    char str[10];
    FILE *hexFile[4];

    /* Open the vhd files in write mode to erase them */
    for (uint8_t i = 0 ; i < sizeof(uint32_t) ; i ++)
    {
        /* Build string with custom name */
        memset(str, '\0', sizeof(str));
        memset(filespath[i], '\0', sizeof(filespath[i]));
        strcpy(filespath[i], filepath);
        if (i < 2)
        {
            strcat(filespath[i], "0");
        }
        itoa(i*8 , str, 10);
        strcat(str, ".hex");
        strcat(filespath[i], str);
        hexFile[i] = fopen(filespath[i], "w");
    }

    for (uint32_t addr = 0 ; addr < dataMemSize ; addr+=4)
    {
        /* Check if there is data to save */
        if (addr < instrSize)
        {
            /* Loop to do each file */
            for (uint8_t i = 0 ; i < sizeof(uint32_t) ; i++)
            {
                /* Compute cheksum with the address divided by 4 since 4 files has the same address with an offset*/
                cks = cks8(byteNb, (addr/4), fieldType, instrMem[addr+sizeof(uint32_t)-i-1]);
                /* Print string in .hex file with the correct data */
                fprintf(hexFile[i], ":%.2x%.4x%.2x%.2x%.2x\n", byteNb, (addr/4), fieldType, instrMem[addr+sizeof(uint32_t)-i-1], cks);
                // /* console print out */
				//printf("\nLittle :\n");
                //printf(":%.2x%.4x%.2x%.2x%.2x\t", byteNb, (addr/4), fieldType, instrMem[addr+i], cks);
				//printf("\nBig :\n");
				//printf(":byteNB : %.2x | addr/4 : %.4x | fieldtype : %.2x |instrMem : %.2x |cks : %.2x\t", byteNb, (addr/4), fieldType, instrMem[addr+sizeof(uint32_t)-i-1], cks);
            }
        }
        else
        {
            /* keep fiiling the ram memory with 0 even when the programm file is smaller than ram size */
            for (uint8_t i = 0 ; i < sizeof(uint32_t) ; i++)
            {
                /* compute checksum with data set to 0 */
                cks = cks8(byteNb, addr/4, fieldType, 0);
                /* Print string in current .hex file with the correct data */
                fprintf(hexFile[i], ":%.2x%.4x%.2x%.2x%.2x\n", byteNb, addr/4, fieldType, 0, cks);
                // /* console print out */
                // printf(":%.2x%.4x%.2x%.2x%.2x\t", byteNb, addr/4, fieldType, 0, cks);
            }
        }
    }

    /* close files and add EOF intel hex string */
    for (uint8_t i = 0 ; i < sizeof(uint32_t) ; i ++)
    {
        fprintf(hexFile[i], ":00000001FF");
        fclose(hexFile[i]);
    }
}

void buildHexFile32(char* filepath)
{
    uint8_t byteNb = 4;
    uint8_t cks = 0;
    uint8_t fieldType = 0;
    FILE *instrFile = fopen(filepath, "w");
    uint32_t data = 0;
    
    for (uint32_t addr = 0 ; addr < dataMemSize ; addr+=4)
    {
        data = 0;
        /* Check if there is data to save */
        if (addr < instrSize)
        {
            fprintf(instrFile, ":%.2x%.4x%.2x", byteNb, addr/4, fieldType);
            for (uint8_t i = 0 ; i < sizeof(uint32_t) ; i++)
            {
                data <<= 8;
                data |= instrMem[addr+sizeof(uint32_t)-i-1];
            }
            fprintf(instrFile, "%.8x%.2x\n", data, cks8(byteNb, addr/4, fieldType, data));
        }
        else
        {
            fprintf(instrFile, ":%.2x%.4x%.2x%.8x%.2x\n", byteNb, addr/4, fieldType, 0, cks8(byteNb, addr/4, fieldType, 0));
        }
    }
    fprintf(instrFile, ":00000001FF");
    fclose(instrFile);
}

/* Checksum : split each line into bytes add them, complement the result and add 1  */
uint8_t cks8(uint8_t byteNb, uint32_t addr, uint8_t fieldType, uint32_t data)
{
    uint8_t cks = 0;

    cks += byteNb;
    cks += (addr & 0xff);
    cks += (addr >> 8 & 0xff);
    cks += fieldType;
    for (uint8_t i = 0 ; i < byteNb ; i++)
    {
        cks += (( data >> (i*8)) & 0xff);
    }
    return ~cks + 1;
}