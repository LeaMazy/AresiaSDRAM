// #define addrMax 0xFFC
// #define uint32_t unsigned int
// #define uint8_t unsigned char
// #define Tx_busy 1  //0001
// #define Rx_error 2 //0010
// #define Rx_full 4  //0100

// void main(void)
// {
//     uint32_t *addrSRAM = (uint32_t*)0x00000000;
//     uint32_t *ADDMax = (uint32_t*)addrMax;
//     uint8_t *addrSRAMB = (uint8_t*)0x00000000;
//     uint8_t *ADDMaxB = (uint8_t*)addrMax;
//     uint32_t Lentmp;
//     uint32_t Len = 0x00000000;
//     uint32_t receptUart = 0;
//     uint32_t bufferWord = 0;
//     uint32_t bufferByte = 0; 
//     volatile uint32_t *UART_Status = (uint32_t*)0xC0000000;
//     volatile uint32_t *Rx = (uint32_t*)0xC0000004;
//     volatile uint32_t *Tx = (uint32_t*)0xC0000004;

//     volatile int *AD_DISPLAY1 = (int *)0x80000004;
//     volatile int *AD_DISPLAY2 = (int *)0x80000008;
//     *AD_DISPLAY2=0xffff;     //BO
//     *AD_DISPLAY1=0x83c0c087; //OT

//     for(int i = 3; i >= 0; i--){
//         while ((*UART_Status & Rx_full) == 0);
//         Lentmp = *Rx;
//         while ((*UART_Status & Tx_busy) == 1);
//         *Tx=Lentmp;
//         Len |= Lentmp<<i*8;

//     }
//     while (Len > 0 && ADDMax > addrSRAM){
//         bufferWord = 0;
//         for(int i = 3; i >= 0; i--){
//             while ((*UART_Status & Rx_full) == 0);
//             receptUart=*Rx;
            
//             // bufferWord |= (receptUart<<(i*8));

//             while ((*UART_Status & Tx_busy) == 1);
//             *Tx = receptUart;
//             Len = Len - 1;
//         }

//         // *addrSRAM=bufferWord;
//         // bufferWord=*addrSRAM;
//         // for(int i = 3; i>=0; i--){
//         //     bufferByte = (bufferWord>>(i*8)) & 0xff;
//         //     while ((*UART_Status & Tx_busy) == 1);
//         //     *Tx = bufferByte;
//         // }

//         addrSRAM = addrSRAM + 1;

//     }
//     addrSRAMB = 0;
//     while (ADDMaxB > addrSRAMB){
//         // bufferByte = *addrSRAM;
//         while ((*UART_Status & Tx_busy) == 1);
//         *Tx = *addrSRAMB;
//         addrSRAMB = addrSRAMB + 1;
//     }
//     // Tx = (uint32_t *)1;
//     // while(Tx==(uint32_t *)1){
//     //     Tx = Tx + (*UART_Status);
//     // }
//     while(1);
// }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// #define uint32_t unsigned int
typedef __SIZE_TYPE__ size_t;
// adresses in data memory
#define AD_TOPleds 0x8000000c
#define AD_DISPLAY1 0x80000004  // MSB=Hex3, Hex2, Hex1, LSB=Hex0 (Hex described in DE10LITE User manual)
#define AD_DISPLAY2 0x80000008  // MSB=X, X, Hex5, LSB=Hex4
#define MASTER_CLK_RATE 50 * 1000000
#define PLL_DIV  50
#define CLK_RATE    MASTER_CLK_RATE/PLL_DIV//*40
#define MS_TICK    CLK_RATE///40000
#define DISPLAY_NB 6

#define addrMax 0xFFC //0x7FFFFFFF
#define uint32_t unsigned int
#define uint8_t unsigned char
#define Tx_busy 1  //0001
#define Rx_error 2 //0010
#define Rx_full 4  //0100


void calcDisp(uint32_t pos, uint32_t* hexVal, uint32_t len)
{
    uint32_t disp1 = 0;
    uint32_t disp2 = 0;
    
    for (uint32_t i = DISPLAY_NB ; i > 4 ; i--)
    {
        uint32_t shift = ((i-5)*8);
        uint32_t index = (pos-i);
        uint32_t condition = (index >= 0 && index < len);
        disp2 |= (condition ? (hexVal[index] << shift) : (0xff << shift));
    }
    
    for (uint32_t i = 4 ; i > 0 ; i--)
    {
        uint32_t shift = ((i-1)*8);
        uint32_t index = (pos-i);
        uint32_t condition = (index >= 0 && index < len);
        disp1 |= (condition ? (hexVal[index] << shift) : (0xff << shift));
    }
    volatile int *AD_DISPLAYt = (int*) AD_DISPLAY2; 
    *AD_DISPLAYt = disp2;
    AD_DISPLAYt = (int*) AD_DISPLAY1;
    *AD_DISPLAYt = disp1;
}

// void *memcpy(void *restrict d, const void *restrict s, size_t n){
    // const char *s8 = s;
    // char *d8 = d;
    // while (n--)
        // *(d8++) = *(s8++);
    // return d;
// }

int getCode7Seg(char c)
{

    char hexVal=0;
    switch(c)
    {
        case '0' : hexVal = 0xc0; break;
        case '1' : hexVal = 0xf9; break;
        case '2' : hexVal = 0xa4; break;
        case '3' : hexVal = 0xb0; break;
        case '4' : hexVal = 0x99; break;
        case '5' : hexVal = 0x92; break;
        case '6' : hexVal = 0x82; break;
        case '7' : hexVal = 0xf8; break;
        case '8' : hexVal = 0x80; break;
        case '9' : hexVal = 0x90; break;
        case 'A' : hexVal = 0x88; break;
        case 'B' : hexVal = 0x83; break;
        case 'C' : hexVal = 0xc6; break;
        case 'D' : hexVal = 0xa1; break;
        case 'E' : hexVal = 0x86; break;
        case 'F' : hexVal = 0x8e; break;
        case 'G' : hexVal = 0xc2; break;
        case 'H' : hexVal = 0x8b; break;
        case 'I' : hexVal = 0xf9; break;
        case 'J' : hexVal = 0xe1; break;
        case 'K' : hexVal = 0x8a; break;
        case 'L' : hexVal = 0xc7; break;
        case 'M' : hexVal = 0xaa; break;
        case 'N' : hexVal = 0xab; break;
        case 'O' : hexVal = 0xc0; break;
        case 'P' : hexVal = 0x8c; break;
        case 'Q' : hexVal = 0x98; break;
        case 'R' : hexVal = 0xce; break;
        case 'S' : hexVal = 0x92; break;
        case 'T' : hexVal = 0x87; break;
        case 'U' : hexVal = 0xc1; break;
        case 'V' : hexVal = 0xb5; break;
        case 'W' : hexVal = 0x95; break;
        case 'X' : hexVal = 0x89; break;
        case 'Y' : hexVal = 0x91; break;
        case 'Z' : hexVal = 0xa4; break;
        case ' ' : hexVal = 0xff; break;
        case '.' : hexVal = 0x7f; break;
        case '-' : hexVal = 0xbf; break;
        case '_' : hexVal = 0xf7; break;
        default  : hexVal = 0x7f; break;
    }
    *(uint32_t*) AD_DISPLAY2 = hexVal;//(uint32_t) c;
    return hexVal;
}

void wait(int delay){
    volatile uint32_t delayCPT=0;
    for(int i=0; i<delay; i++){
        delayCPT++;
    }
    delayCPT=0;
}

int main(int argc, char *argv[]) {
    ///////////////////VARIABLE////////////////
    char * str = "ARESIA RISC-5";

    uint32_t len=0;
    volatile uint32_t ledPos=1;
    volatile uint32_t sens = 1; // 1 vers la droite, 0 vers la gauche

    // uint32_t CPTtext=0;
    uint32_t hexVal[16];


    // uint8_t *ADDMax = (uint8_t*)addrMax;
    // uint8_t *addrSRAM = (uint8_t*)0x00000000;
    // volatile uint32_t *UART_Status = (uint32_t*)0xC0000000;
    // volatile uint32_t *Rx = (uint32_t*)0xC0000004;
    // volatile uint32_t *Tx = (uint32_t*)0xC0000004;

    // while (ADDMax > addrSRAM){
    //     // bufferByte = *addrSRAM;
    //     while ((*UART_Status & Tx_busy) == 1);
    //     *Tx = *addrSRAM;

    //     addrSRAM = addrSRAM + 1;
    // }
    
    ////////////////INITIALISATION/////////////
    volatile int *AD_DISPLAYt = (int*) AD_DISPLAY2; 
    *AD_DISPLAYt = 0xffff;
    AD_DISPLAYt = (int*) AD_DISPLAY1;
    *AD_DISPLAYt = 0xFFFFFFFF;
    AD_DISPLAYt = (int*) AD_TOPleds;
    *AD_DISPLAYt = 0xccc;


    // *AD_DISPLAY1 = 0xFFFFFFFF;
    // *AD_DISPLAY2 = 0xffff;
    // *AD_TOPleds = 0xccc;
    
    ////////////////////CODE///////////////////

    *(uint32_t*) AD_DISPLAY1 = (uint32_t)str[0];
    getCode7Seg(str[0]);

    for (uint32_t i = 0 ; str[i]!='\0'; i++)
    {
        hexVal[i] = getCode7Seg(str[i]);
        len++;
    }
    // AD_DISPLAYt = (int*) AD_DISPLAY1;
    // for (int i=0; i<6; i++){
        // hexVal[i] = getCode7Seg(str[i]);
    // }
    
    
    // AD_DISPLAY2 = (0xc0 << 8) | 0x8a;
    
    // AD_DISPLAY2 = (hexVal[0] << 8) | hexVal[1];
    // AD_DISPLAY1  = (hexVal[2] << 24) | (hexVal[3] << 16) | (hexVal[4] << 8) | hexVal[5];
    // calcDisp(0, hexVal, len);
    for(;;)
    {
        for (uint32_t pos = 0 ; pos < (len + DISPLAY_NB) ; pos++)
        {
            for(uint32_t i=0; i<50; i++){
                volatile int *AD_DISPLAYt = (int*) AD_TOPleds;
                *AD_DISPLAYt = ledPos;
                // *AD_TOPleds = ledPos;
                if(ledPos==512) sens=1;
                if(ledPos==1) sens=0;
                if(sens==1){ ledPos = ledPos>>1;}
                else{       ledPos = ledPos<<1; }
                wait(10000);
            }
            
            // calcDisp(pos, hexVal, len);
            //wait(1000);
        }
    }
    while(1);
}





































//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// #define addrMax 0x7FFFFFFF
// #define uint32_t unsigned int
// #define uint8_t unsigned char
// #define Tx_busy 1  //0001
// // #define Rx_error 2 //0010
// // #define Rx_full 4  //0100
// // uint8_t getCode7Seg(uint8_t c)
// // {
// //     volatile uint8_t hexVal = 0;
// //     switch(c)
// //     {
// //         case '0' : hexVal = 0xc0; break;
// //         //case '0' : hexVal = 192; break;
// //         case '1' : hexVal = 0xf9; break;
// //         case '2' : hexVal = 0xa4; break;
// //         case '3' : hexVal = 0xb0; break;
// //         case '4' : hexVal = 0x99; break;
// //         case '5' : hexVal = 0x92; break;
// //         case '6' : hexVal = 0x82; break;
// //         case '7' : hexVal = 0xf8; break;
// //         case '8' : hexVal = 0x80; break;
// //         case '9' : hexVal = 0x90; break;
// //         case 'A' : hexVal = 0x88; break;
// //         case 'B' : hexVal = 0x83; break;
// //         case 'C' : hexVal = 0xc6; break;
// //         case 'D' : hexVal = 0xa1; break;
// //         case 'E' : hexVal = 0x86; break;
// //         case 'F' : hexVal = 0x8e; break;
// //         case 'G' : hexVal = 0xc2; break;
// //         case 'H' : hexVal = 0x8b; break;
// //         case 'I' : hexVal = 0xfb; break;
// //         case 'J' : hexVal = 0xe1; break;
// //         case 'K' : hexVal = 0x8a; break;
// //         case 'L' : hexVal = 0xc7; break;
// //         case 'M' : hexVal = 0xaa; break;
// //         case 'N' : hexVal = 0xab; break;
// //         case 'O' : hexVal = 0xc0; break;
// //         //case 'O' : hexVal = 192; break;
// //         case 'P' : hexVal = 0x8c; break;
// //         case 'Q' : hexVal = 0x98; break;
// //         case 'R' : hexVal = 0xce; break;
// //         case 'S' : hexVal = 0x92; break;
// //         case 'T' : hexVal = 0x87; break;
// //         case 'U' : hexVal = 0xc3; break;
// //         case 'V' : hexVal = 0xb5; break;
// //         case 'W' : hexVal = 0x95; break;
// //         case 'X' : hexVal = 0x89; break;
// //         case 'Y' : hexVal = 0x91; break;
// //         case 'Z' : hexVal = 0xa4; break;
// //         case ' ' : hexVal = 0xff; break;
// //         case '.' : hexVal = 0x7f; break;
// //         case '-' : hexVal = 0xbf; break;
// //         case '_' : hexVal = 0xf7; break;
// //         default  : hexVal = 0x7f; break;
// //     }
// //     return hexVal;
// // }

// void main(void)
// {
//     uint8_t *addrSRAM = (uint8_t*)0x00000000;
//     uint8_t *ADDMax = (uint8_t*)addrMax;
//     uint32_t Lentmp;
//     uint32_t Len = 0x00000000;
//     uint32_t receptUart = 0;
//     volatile uint32_t *UART_Status = (uint32_t*)0xC0000000;
//     volatile uint32_t *Rx = (uint32_t*)0xC0000004;
//     volatile uint32_t *Tx = (uint32_t*)0xC0000004;
//     volatile uint32_t Max = 5;
//     volatile uint32_t count = 0;
//     volatile uint32_t cond = 1;
//     // for(int i = 0; i < 4; i++){
//     //     while ((*UART_Status & Rx_full) != 0);
//     //     while (*Rx==0);
//     //     Lentmp = *Rx;
//     //     while ((*UART_Status & Tx_busy) != 0);
//     //     *Tx = 0x30;
//     //     // *Tx=Lentmp;
//     //     Len |= Lentmp<<i*8;
//     // }
//     // while(Len > 0 && ADDMax > addrSRAM){
//     // while(ADDMax > addrSRAM){
//     //     while ((*UART_Status & Rx_full) != 0);
//     //     while(*Rx == 0);
//     //     receptUart=*Rx;
//     //     while ((*UART_Status & Tx_busy) != 0);
//     //     *Tx = 0x30;
//     //     // *Tx=receptUart;
//     //     *addrSRAM=receptUart;
//     //     //Len--;
//     //     addrSRAM++;
//     // }

//     // *AD_DISPLAY2=0x83c0;     //BO
//     // *AD_DISPLAY1=0xc087ffff; //OT
//     // *AD_DISPLAY2=0x92ce;        //SR
//     // *AD_DISPLAY1=0x88aaffff;    //AM

//     // volatile int *AD_DISPLAY1 = (int *)0x80000004;
//     // volatile int *AD_DISPLAY2 = (int *)0x80000008;
//     // char const * str = "SRAM  ";
//     // volatile uint8_t hexVal[6];

//     // for (uint8_t i = 0 ; i < 6 ; i++) {
//     //     hexVal[i] = getCode7Seg(str[i]);
//     // }

//     // *AD_DISPLAY2 =0xffffffff;
//     // *AD_DISPLAY1 =0xffffffff;
//     // *AD_DISPLAY2 =hexVal[0] << 8;
//     // *AD_DISPLAY2|=hexVal[1];
    
//     // *AD_DISPLAY1 =hexVal[2] << 24;
//     // *AD_DISPLAY1|=hexVal[3] << 16;
//     // *AD_DISPLAY1|=hexVal[4] << 8;
//     // *AD_DISPLAY1|=hexVal[5];
//     while(1)
//     {
//         if (count < Max)
//         {
//             Max=4;
//         }
//         //while (count < 1)
//         while (cond)
//         {
//             Max--;
//             if(Max == count)
//             {
//                 count++;
//             }
//         }
//         Max=3;
//         //count--;
        
//         //count--;
//         // while ((*UART_Status & Tx_busy) == 1);
//         // *Tx = 0x30;
//         // while ((*UART_Status & Tx_busy) == 1);
//         // *Tx = 0x40;
//     }
// }



