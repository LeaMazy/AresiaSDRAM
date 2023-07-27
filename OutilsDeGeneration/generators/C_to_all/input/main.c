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
            
//             bufferWord |= (receptUart<<(i*8));

//             while ((*UART_Status & Tx_busy) == 1);
//             *Tx = receptUart;
//             Len = Len - 1;
//         }

//         *addrSRAM=bufferWord;
//         bufferWord=*addrSRAM;
//         for(int i = 3; i>=0; i--){
//             bufferByte = (bufferWord>>(i*8)) & 0xff;
//             while ((*UART_Status & Tx_busy) == 1);
//             *Tx = bufferByte;
//         }

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

// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// // #define uint32_t unsigned int
// typedef __SIZE_TYPE__ size_t;
// // adresses in data memory
// #define AD_TOPleds 0x8000000c
// #define AD_DISPLAY1 0x80000004  // MSB=Hex3, Hex2, Hex1, LSB=Hex0 (Hex described in DE10LITE User manual)
// #define AD_DISPLAY2 0x80000008  // MSB=X, X, Hex5, LSB=Hex4
// #define MASTER_CLK_RATE 50 * 1000000
// #define PLL_DIV  50
// #define CLK_RATE    MASTER_CLK_RATE/PLL_DIV//*40
// #define MS_TICK    CLK_RATE///40000
// #define DISPLAY_NB 6

// #define addrMax 0xFFC //0x7FFFFFFF
// #define uint32_t unsigned int
// #define uint8_t unsigned char
// #define Tx_busy 1  //0001
// #define Rx_error 2 //0010
// #define Rx_full 4  //0100


// void calcDisp(uint32_t pos, uint32_t* hexVal, uint32_t len)
// {
//     uint32_t disp1 = 0;
//     uint32_t disp2 = 0;
    
//     for (uint32_t i = DISPLAY_NB ; i > 4 ; i--)
//     {
//         uint32_t shift = ((i-5)*8);
//         uint32_t index = (pos-i);
//         uint32_t condition = (index >= 0 && index < len);
//         disp2 |= (condition ? (hexVal[index] << shift) : (0xff << shift));
//     }
    
//     for (uint32_t i = 4 ; i > 0 ; i--)
//     {
//         uint32_t shift = ((i-1)*8);
//         uint32_t index = (pos-i);
//         uint32_t condition = (index >= 0 && index < len);
//         disp1 |= (condition ? (hexVal[index] << shift) : (0xff << shift));
//     }
//     volatile int *AD_DISPLAYt = (int*) AD_DISPLAY2; 
//     *AD_DISPLAYt = disp2;
//     AD_DISPLAYt = (int*) AD_DISPLAY1;
//     *AD_DISPLAYt = disp1;
// }

// // void *memcpy(void *restrict d, const void *restrict s, size_t n){
//     // const char *s8 = s;
//     // char *d8 = d;
//     // while (n--)
//         // *(d8++) = *(s8++);
//     // return d;
// // }

// int getCode7Seg(char c)
// {

//     char hexVal=0;
//     switch(c)
//     {
//         case '0' : hexVal = 0xc0; break;
//         case '1' : hexVal = 0xf9; break;
//         case '2' : hexVal = 0xa4; break;
//         case '3' : hexVal = 0xb0; break;
//         case '4' : hexVal = 0x99; break;
//         case '5' : hexVal = 0x92; break;
//         case '6' : hexVal = 0x82; break;
//         case '7' : hexVal = 0xf8; break;
//         case '8' : hexVal = 0x80; break;
//         case '9' : hexVal = 0x90; break;
//         case 'A' : hexVal = 0x88; break;
//         case 'B' : hexVal = 0x83; break;
//         case 'C' : hexVal = 0xc6; break;
//         case 'D' : hexVal = 0xa1; break;
//         case 'E' : hexVal = 0x86; break;
//         case 'F' : hexVal = 0x8e; break;
//         case 'G' : hexVal = 0xc2; break;
//         case 'H' : hexVal = 0x8b; break;
//         case 'I' : hexVal = 0xf9; break;
//         case 'J' : hexVal = 0xe1; break;
//         case 'K' : hexVal = 0x8a; break;
//         case 'L' : hexVal = 0xc7; break;
//         case 'M' : hexVal = 0xaa; break;
//         case 'N' : hexVal = 0xab; break;
//         case 'O' : hexVal = 0xc0; break;
//         case 'P' : hexVal = 0x8c; break;
//         case 'Q' : hexVal = 0x98; break;
//         case 'R' : hexVal = 0xce; break;
//         case 'S' : hexVal = 0x92; break;
//         case 'T' : hexVal = 0x87; break;
//         case 'U' : hexVal = 0xc1; break;
//         case 'V' : hexVal = 0xb5; break;
//         case 'W' : hexVal = 0x95; break;
//         case 'X' : hexVal = 0x89; break;
//         case 'Y' : hexVal = 0x91; break;
//         case 'Z' : hexVal = 0xa4; break;
//         case ' ' : hexVal = 0xff; break;
//         case '.' : hexVal = 0x7f; break;
//         case '-' : hexVal = 0xbf; break;
//         case '_' : hexVal = 0xf7; break;
//         default  : hexVal = 0x7f; break;
//     }
//     *(uint32_t*) AD_DISPLAY2 = hexVal;//(uint32_t) c;
//     return hexVal;
// }

// void wait(int delay){
//     volatile uint32_t delayCPT=0;
//     for(int i=0; i<delay; i++){
//         delayCPT++;
//     }
//     delayCPT=0;
// }

// int main(int argc, char *argv[]) {
//     ///////////////////VARIABLE////////////////
//     char * str = "ARESIA RISC-5";

//     uint32_t len=0;
//     volatile uint32_t ledPos=1;
//     volatile uint32_t sens = 1; // 1 vers la droite, 0 vers la gauche

//     // uint32_t CPTtext=0;
//     uint32_t hexVal[16];


//     // uint8_t *ADDMax = (uint8_t*)addrMax;
//     // uint8_t *addrSRAM = (uint8_t*)0x00000000;
//     // volatile uint32_t *UART_Status = (uint32_t*)0xC0000000;
//     // volatile uint32_t *Rx = (uint32_t*)0xC0000004;
//     // volatile uint32_t *Tx = (uint32_t*)0xC0000004;

//     // while (ADDMax > addrSRAM){
//     //     // bufferByte = *addrSRAM;
//     //     while ((*UART_Status & Tx_busy) == 1);
//     //     *Tx = *addrSRAM;

//     //     addrSRAM = addrSRAM + 1;
//     // }
    
//     ////////////////INITIALISATION/////////////
//     volatile int *AD_DISPLAYt = (int*) AD_DISPLAY2; 
//     *AD_DISPLAYt = 0xffff;
//     AD_DISPLAYt = (int*) AD_DISPLAY1;
//     *AD_DISPLAYt = 0xFFFFFFFF;
//     AD_DISPLAYt = (int*) AD_TOPleds;
//     *AD_DISPLAYt = 0xccc;


//     // *AD_DISPLAY1 = 0xFFFFFFFF;
//     // *AD_DISPLAY2 = 0xffff;
//     // *AD_TOPleds = 0xccc;
    
//     ////////////////////CODE///////////////////

//     *(uint32_t*) AD_DISPLAY1 = (uint32_t)str[0];
//     getCode7Seg(str[0]);

//     for (uint32_t i = 0 ; str[i]!='\0'; i++)
//     {
//         hexVal[i] = getCode7Seg(str[i]);
//         len++;
//     }
//     // AD_DISPLAYt = (int*) AD_DISPLAY1;
//     // for (int i=0; i<6; i++){
//         // hexVal[i] = getCode7Seg(str[i]);
//     // }
    
    
//     // AD_DISPLAY2 = (0xc0 << 8) | 0x8a;
    
//     // AD_DISPLAY2 = (hexVal[0] << 8) | hexVal[1];
//     // AD_DISPLAY1  = (hexVal[2] << 24) | (hexVal[3] << 16) | (hexVal[4] << 8) | hexVal[5];
//     // calcDisp(0, hexVal, len);
//     for(;;)
//     {
//         for (uint32_t pos = 0 ; pos < (len + DISPLAY_NB) ; pos++)
//         {
//             for(uint32_t i=0; i<50; i++){
//                 volatile int *AD_DISPLAYt = (int*) AD_TOPleds;
//                 *AD_DISPLAYt = ledPos;
//                 // *AD_TOPleds = ledPos;
//                 if(ledPos==512) sens=1;
//                 if(ledPos==1) sens=0;
//                 if(sens==1){ ledPos = ledPos>>1;}
//                 else{       ledPos = ledPos<<1; }
//                 wait(10000);
//             }
            
//             // calcDisp(pos, hexVal, len);
//             //wait(1000);
//         }
//     }
//     while(1);
// }





































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










































//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// #define uint8_t unsigned char
// #define uint32_t unsigned int
// typedef __SIZE_TYPE__ size_t;
// //adresses in data memory
// #define AD_TOPleds 0x8000000c
// #define AD_DISPLAY1 0x80000004  // MSB=Hex3, Hex2, Hex1, LSB=Hex0 (Hex described in DE10LITE User manual)
// #define AD_DISPLAY2 0x80000008  // MSB=X, X, Hex5, LSB=Hex4
// #define MASTER_CLK_RATE 50 * 1000000
// #define PLL_DIV  50
// #define CLK_RATE    MASTER_CLK_RATE/PLL_DIV*40
// #define MS_TICK    CLK_RATE/40000
// #define DISPLAY_NB 6

// #define addrMax 0xFFC //0x7FFFFFFF
// #define uint32_t unsigned int
// #define uint8_t unsigned char
// #define Tx_busy 1  //0001
// #define Rx_error 2 //0010
// #define Rx_full 4  //0100

// volatile uint32_t *UART_Status = (uint32_t*)0xC0000000;
// volatile uint32_t *Rx = (uint32_t*)0xC0000004;
// volatile uint32_t *Tx = (uint32_t*)0xC0000004;
// volatile char * str = "ARESIA RISC-5";
// volatile uint32_t hexVal[16];


// void calcDisp(uint32_t pos, uint32_t* hexVal, uint32_t len)
// {
//     uint32_t disp1 = 0;
//     uint32_t disp2 = 0;
    
//     for (uint32_t i = DISPLAY_NB ; i > 4 ; i--)
//     {
//         uint32_t shift = ((i-5)*8);
//         uint32_t index = (pos-i);
//         uint32_t condition = (index >= 0 && index < len);
//         disp2 |= (condition ? (hexVal[index] << shift) : (0xff << shift));
//     }
    
//     for (uint32_t i = 4 ; i > 0 ; i--)
//     {
//         uint32_t shift = ((i-1)*8);
//         uint32_t index = (pos-i);
//         uint32_t condition = (index >= 0 && index < len);
//         disp1 |= (condition ? (hexVal[index] << shift) : (0xff << shift));
//     }
//     volatile int *AD_DISPLAYt = (int*) AD_DISPLAY2; 
//     *AD_DISPLAYt = disp2;
//     AD_DISPLAYt = (int*) AD_DISPLAY1;
//     *AD_DISPLAYt = disp1;
// }

// // void *memcpy(void *restrict d, const void *restrict s, size_t n){
//     // const char *s8 = s;
//     // char *d8 = d;
//     // while (n--)
//         // *(d8++) = *(s8++);
//     // return d;
// // }

// int getCode7Seg(char c)
// {

//     char hexVal=0;
//     switch(c)
//     {
//         case '0' : hexVal = 0xc0; break;
//         case '1' : hexVal = 0xf9; break;
//         case '2' : hexVal = 0xa4; break;
//         case '3' : hexVal = 0xb0; break;
//         case '4' : hexVal = 0x99; break;
//         case '5' : hexVal = 0x92; break;
//         case '6' : hexVal = 0x82; break;
//         case '7' : hexVal = 0xf8; break;
//         case '8' : hexVal = 0x80; break;
//         case '9' : hexVal = 0x90; break;
//         case 'A' : hexVal = 0x88; break;
//         case 'B' : hexVal = 0x83; break;
//         case 'C' : hexVal = 0xc6; break;
//         case 'D' : hexVal = 0xa1; break;
//         case 'E' : hexVal = 0x86; break;
//         case 'F' : hexVal = 0x8e; break;
//         case 'G' : hexVal = 0xc2; break;
//         case 'H' : hexVal = 0x8b; break;
//         case 'I' : hexVal = 0xf9; break;
//         case 'J' : hexVal = 0xe1; break;
//         case 'K' : hexVal = 0x8a; break;
//         case 'L' : hexVal = 0xc7; break;
//         case 'M' : hexVal = 0xaa; break;
//         case 'N' : hexVal = 0xab; break;
//         case 'O' : hexVal = 0xc0; break;
//         case 'P' : hexVal = 0x8c; break;
//         case 'Q' : hexVal = 0x98; break;
//         case 'R' : hexVal = 0xce; break;
//         case 'S' : hexVal = 0x92; break;
//         case 'T' : hexVal = 0x87; break;
//         case 'U' : hexVal = 0xc1; break;
//         case 'V' : hexVal = 0xb5; break;
//         case 'W' : hexVal = 0x95; break;
//         case 'X' : hexVal = 0x89; break;
//         case 'Y' : hexVal = 0x91; break;
//         case 'Z' : hexVal = 0xa4; break;
//         case ' ' : hexVal = 0xff; break;
//         case '.' : hexVal = 0x7f; break;
//         case '-' : hexVal = 0xbf; break;
//         case '_' : hexVal = 0xf7; break;
//         default  : hexVal = 0x7f; break;
//     }
//     return hexVal;
// }

// void wait(int delay){
//     volatile uint32_t delayCPT=0;
//     for(int i=0; i<delay; i++){
//         delayCPT++;
//     }
//     delayCPT=0;
// }

// int main(int argc, char *argv[]) {
//     ///////////////////VARIABLE////////////////
//     // char * str = "ARESIA RISC-5";

//     uint32_t len=0;
//     volatile uint32_t ledPos=1;
//     volatile uint32_t sens = 1; // 1 vers la droite, 0 vers la gauche
//     volatile uint32_t tmp;
//     // uint32_t CPTtext=0;
//     // uint32_t hexVal[16];


//     uint8_t *ADDMax = (uint8_t*)addrMax;
//     uint8_t *addrSRAM = (uint8_t*)0x00000000;
//     volatile uint32_t *UART_Status = (uint32_t*)0xC0000000;

//     while (ADDMax > addrSRAM){
//         // bufferByte = *addrSRAM;
//         while ((*UART_Status & Tx_busy) == 1);
//         *Tx = *addrSRAM;

//         addrSRAM = addrSRAM + 1;
//     }
    
//     //////////////INITIALISATION/////////////
//     volatile int *AD_DISPLAYt = (int*) AD_DISPLAY2; 
//     *AD_DISPLAYt = 0xffff;
//     AD_DISPLAYt = (int*) AD_DISPLAY1;
//     *AD_DISPLAYt = 0xFFFFFFFF;
//     AD_DISPLAYt = (int*) AD_TOPleds;
//     *AD_DISPLAYt = 0xccc;


//     // *AD_DISPLAY1 = 0xFFFFFFFF;
//     // *AD_DISPLAY2 = 0xffff;
//     // *AD_TOPleds = 0xccc;
    
//     ////////////////////CODE///////////////////


//     for (uint32_t i = 0 ; str[i]!='\0'; i++)
//     {
//         hexVal[i] = getCode7Seg(str[i]);
//         len++;
//     }
    
//     int j = 0;
//     for(;;)
//     {
//         for (uint32_t pos = 0 ; pos < (len + DISPLAY_NB) ; pos++)
//         {
//             for(uint32_t i=0; i<50; i++){
//                 volatile int *AD_DISPLAYt = (int*) AD_TOPleds;
//                 *AD_DISPLAYt = ledPos;
//                 // *AD_TOPleds = ledPos;
//                 if(ledPos==512) sens=1;
//                 if(ledPos==1) sens=0;
//                 if(sens==1){ ledPos = ledPos>>1;}
//                 else{       ledPos = ledPos<<1; }
//                 wait(10000);
//             }
            
//             calcDisp(pos, hexVal, len);
//         }
//         j++;
//     }
//     while(1);
// }


// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// // #define uint32_t unsigned int
// // #define uint16_t unsigned short
// // #define uint8_t unsigned char


// // void main(void){
// //     volatile int *AD_DISPLAY1 = (int *)0x80000004;
// //     volatile int *AD_DISPLAY2 = (int *)0x80000008;
// //     *AD_DISPLAY2=0xffff;     //BO
// //     *AD_DISPLAY1=0x83c0c087; //OT

// //     uint

// //     while(1);
// // }














//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// #define uint32_t unsigned int
// #define uint16_t unsigned short
// #define uint8_t unsigned char
// #define GPIO 0x80000010
// #define debug 0x0000200
// #define hold 0x00000002

// void wait(int delay){
//     volatile uint32_t delayCPT=0;
//     for(int i=0; i<delay; i++){
//         delayCPT++;
//     }
//     delayCPT=0;
// }

// int getCode7Seg(char c){
//     char hexVal=0;
//     switch(c)
//     {
//         case '0' : hexVal = 0xc0; break;
//         case '1' : hexVal = 0xf9; break;
//         case '2' : hexVal = 0xa4; break;
//         case '3' : hexVal = 0xb0; break;
//         case '4' : hexVal = 0x99; break;
//         case '5' : hexVal = 0x92; break;
//         case '6' : hexVal = 0x82; break;
//         case '7' : hexVal = 0xf8; break;
//         case '8' : hexVal = 0x80; break;
//         case '9' : hexVal = 0x90; break;
//         case 'A' : hexVal = 0x88; break;
//         case 'B' : hexVal = 0x83; break;
//         case 'C' : hexVal = 0xc6; break;
//         case 'D' : hexVal = 0xa1; break;
//         case 'E' : hexVal = 0x86; break;
//         case 'F' : hexVal = 0x8e; break;
//         case 'G' : hexVal = 0xc2; break;
//         case 'H' : hexVal = 0x8b; break;
//         case 'I' : hexVal = 0xf9; break;
//         case 'J' : hexVal = 0xe1; break;
//         case 'K' : hexVal = 0x8a; break;
//         case 'L' : hexVal = 0xc7; break;
//         case 'M' : hexVal = 0xaa; break;
//         case 'N' : hexVal = 0xab; break;
//         case 'O' : hexVal = 0xc0; break;
//         case 'P' : hexVal = 0x8c; break;
//         case 'Q' : hexVal = 0x98; break;
//         case 'R' : hexVal = 0xce; break;
//         case 'S' : hexVal = 0x92; break;
//         case 'T' : hexVal = 0x87; break;
//         case 'U' : hexVal = 0xc1; break;
//         case 'V' : hexVal = 0xb5; break;
//         case 'W' : hexVal = 0x95; break;
//         case 'X' : hexVal = 0x89; break;
//         case 'Y' : hexVal = 0x91; break;
//         case 'Z' : hexVal = 0xa4; break;
//         case ' ' : hexVal = 0xff; break;
//         case '.' : hexVal = 0x7f; break;
//         case '-' : hexVal = 0xbf; break;
//         case '_' : hexVal = 0xf7; break;
//         default  : hexVal = 0x7f; break;
//     }
//     return hexVal;
// }

// void main(void){
//     volatile int * modeRun = ((int*) GPIO);
//     while((*modeRun & debug) == 0);
//     volatile int *AD_DISPLAY1 = (int *)0x80000004;
//     volatile int *AD_DISPLAY2 = (int *)0x80000008;
//     *AD_DISPLAY2=0xffff;     //BO
//     *AD_DISPLAY1=0x83c0c087; //OT
//     // wait(1000000);
    
//     int i = 0;
//     //volatile char * str = "ARESIA RISC-5";
//     volatile uint8_t * testmot="ABCDE";
//     while(testmot[i] != 'E'){
//         testmot[i]=testmot[i+1];
//         *AD_DISPLAY1=getCode7Seg(testmot[i]);
//         // wait(1000000);
//         i++;
//     }

//     //volatile uint8_t tabChar[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L'};
//     // int i = 0;
//     // while(tabChar[i] != 'L'){
//     //     tabChar[i]=tabChar[i+1];
//     //     *AD_DISPLAY1=getCode7Seg(tabChar[i]);
//     //     // wait(1000000);
//     //     i++;
//     // }

//     while(1);
// }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
















// // BOOTLOADER pour charger SDRAM

#define addrMax 0x2000
#define uint32_t unsigned int
#define uint8_t unsigned char
#define Tx_busy 1  //0001
#define Rx_error 2 //0010
#define Rx_full 4  //0100
#define GPIO 0x80000010
#define debug 0x0000200
#define hold 0x00000002

// void wait(int delay){
//     volatile uint32_t delayCPT=0;
//     for(int i=0; i<delay; i++){
//         delayCPT++;
//     }
//     delayCPT=0;
// }

void main(void)
{
    // volatile int * modeRun = ((int*) GPIO);
    // while((*modeRun & debug) == 0);
    uint32_t *addrSRAM = (uint32_t*)0x00000000;
    uint32_t *ADDMax = (uint32_t*)addrMax;
    uint8_t *addrSRAMB = (uint8_t*)0x00000000;
    uint8_t *ADDMaxB = (uint8_t*)addrMax;
    uint32_t Lentmp;
    uint32_t Len = 0x00000000;
    volatile uint32_t receptUart = 0;
    volatile uint32_t bufferWord = 0;
    volatile uint32_t bufferByte = 0; 
    volatile uint32_t *UART_Status = (uint32_t*)0xC0000000;
    volatile uint32_t *Rx = (uint32_t*)0xC0000004;
    volatile uint32_t *Tx = (uint32_t*)0xC0000004;

    volatile int *AD_DISPLAY1 = (int *)0x80000004;
    volatile int *AD_DISPLAY2 = (int *)0x80000008;
    int tmp;
    *AD_DISPLAY2=0xff83;     //BO
    *AD_DISPLAY1=0xc0c087ff; //OT


    // while(1){
    //     while ((*UART_Status & Rx_full) == 0);
    //     Lentmp = *Rx;
    //     while ((*UART_Status & Tx_busy) == 1);
    //     *Tx=Lentmp;
    //     // Len |= Lentmp<<i*8;
    // }
    for(int i = 3; i >= 0; i--){
        while ((*UART_Status & Rx_full) == 0);
        Lentmp = *Rx;
        while ((*UART_Status & Tx_busy) == 1);
        *Tx=Lentmp;
        Len |= Lentmp<<i*8;

    }

    // Len = 20;
    while (Len > 0 && ADDMax >= addrSRAM){
        bufferWord = 0;
        for(int i = 3; i >= 0; i--){
            while (((*UART_Status & Rx_full) == 0) && ((*UART_Status & Rx_error) == 0));
            while(1) {
                tmp=*UART_Status;
                if((tmp & Rx_full) != 0) break;
                // if((tmp & Rx_error)!=0) {
                //     *AD_DISPLAY1=0x86;
                //     while(1);
                // }
            }    
            receptUart=*Rx;
            *AD_DISPLAY1=(receptUart<<(16));
            bufferWord |= (receptUart<<(i*8));


            // while ((*UART_Status & Tx_busy) != 0);
            // while ((*UART_Status & Tx_busy) == 1);

            // *Tx = receptUart;
            // *Tx = 0x46;
            Len = Len - 1;
            // *AD_DISPLAY1 |= receptUart;
            // *AD_DISPLAY1 |= 0x46;
        }

        *addrSRAM=bufferWord;
        bufferWord=0;
        bufferWord=*addrSRAM;
        for(int i = 3; i>=0; i--){
            bufferByte = (bufferWord>>(i*8)) & 0xff;
            while ((*UART_Status & Tx_busy) == 1);
            *Tx = bufferByte;
            *AD_DISPLAY1 |= bufferByte;
        }

        addrSRAM = addrSRAM + 1;

    }
    // addrSRAMB = 0;
    // while (ADDMaxB > addrSRAMB){
    //     // bufferByte = *addrSRAM;
    //     while ((*UART_Status & Tx_busy) == 1);
    //     *Tx = *addrSRAMB;
    //     addrSRAMB = addrSRAMB + 1;
    // }
    addrSRAM = 0;
    while (ADDMax > addrSRAM){
        bufferWord=0;
        bufferWord=*addrSRAM;
        for(int i = 3; i>=0; i--){
            bufferByte = (bufferWord>>(i*8)) & 0xff;
            while ((*UART_Status & Tx_busy) == 1);
            *Tx = bufferByte;
            *AD_DISPLAY1 |= bufferByte;
        }

        addrSRAM = addrSRAM + 1;
    }
    // Tx = (uint32_t *)1;
    // while(Tx==(uint32_t *)1){
    //     Tx = Tx + (*UART_Status);
    // }
    while(1);
}

// // //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



















// #define Tx_busy 1  //0001
// #define Rx_error 2 //0010
// #define Rx_full 4  //0100

// #define GPIO 0x80000010
// #define debug 0x0000200
// #define hold 0x00000002
// #define addrMax 0xFFC
// #define uint32_t unsigned int
// #define uint16_t unsigned short
// #define uint8_t unsigned char



// volatile uint16_t bufferByte = 0;
// volatile uint32_t bufferWord = 0;
// volatile uint32_t space32a;
// volatile uint32_t space32b;
// volatile uint32_t tmp;

// void wait(int delay){
//     volatile uint32_t delayCPT=0;
//     for(int i=0; i<delay; i++){
//         delayCPT++;
//     }
//     delayCPT=0;
// }

// void main(void)
// {
//     volatile uint16_t *addrSRAM = (uint16_t*)&space32b;
//     volatile uint32_t *ADDMax = (uint32_t*)&space32b;
//     // volatile uint32_t bufferByte = 0; 
//     volatile uint32_t *UART_Status = (uint32_t*)0xC0000000;
//     volatile uint32_t *Rx = (uint32_t*)0xC0000004;
//     volatile uint32_t *Tx = (uint32_t*)0xC0000004;
//     volatile int *AD_DISPLAY1 = (int *)0x80000004;
//     volatile int *AD_DISPLAY2 = (int *)0x80000008;

//     *AD_DISPLAY2=0xff83;     //B
//     *AD_DISPLAY1=0xc0c087ff; //OOT
    
//     bufferWord=0x12345678;

//     bufferByte=0x9A;
//     *addrSRAM=bufferByte;
//     *ADDMax=bufferWord;
//     int i=0;

//     while(1)
//     {
//         if(i>=2)
//         {
//             i=0;
//         }
//         bufferWord=*ADDMax;
//         bufferByte=*(addrSRAM+i);
//         i++;
//         *AD_DISPLAY1=bufferWord;
//         *AD_DISPLAY2=bufferByte;
//         while ((*UART_Status & Tx_busy) == 1);
//         *Tx = bufferByte;
//         while ((*UART_Status & Rx_full) == 0); 
//         tmp=*Rx;
//     }


//     // bufferWord=0;
//     // volatile int * modeRun = ((int*) GPIO);
//     // while((*modeRun & debug) == 0);
//    // *AD_DISPLAY1=bufferWord;
//     // for(int i = 3; i>=0; i--){
//         // bufferByte = (bufferWord>>(i*8)) & 0xff;
    
//     // }
    
//     while(1);
// }


// // void main(void)
// // {
// //     volatile int *AD_DISPLAY1 = (int *)0x80000004;
// //     volatile int *AD_DISPLAY2 = (int *)0x80000008;
// //     *AD_DISPLAY2=0x83c0;     //BO
// //     *AD_DISPLAY1=0xc087ffff; //OT
// //     *AD_DISPLAY2=0x92ce;        //SR
// //     *AD_DISPLAY1=0x88aaffff;    //AM

// //     while(1);
// // }




































































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
            
//             bufferWord |= (receptUart<<(i*8));

//             while ((*UART_Status & Tx_busy) == 1);
//             *Tx = receptUart;
//             Len = Len - 1;
//         }

//         *addrSRAM=bufferWord;
//         bufferWord=*addrSRAM;
//         for(int i = 3; i>=0; i--){
//             bufferByte = (bufferWord>>(i*8)) & 0xff;
//             while ((*UART_Status & Tx_busy) == 1);
//             *Tx = bufferByte;
//         }

//         addrSRAM = addrSRAM + 1;

//     }
//     addrSRAMB = 0;
//     while (ADDMaxB > addrSRAMB){
//         bufferByte = *addrSRAM;
//         while ((*UART_Status & Tx_busy) == 1);
//         *Tx = *addrSRAMB;
//         addrSRAMB = addrSRAMB + 1;
//     }
//     Tx = (uint32_t *)1;
//     while(Tx==(uint32_t *)1){
//         Tx = Tx + (*UART_Status);
//     }
//     while(1);
// }