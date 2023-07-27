// #include <stdint.h>

// uint8_t getCode7Seg(uint8_t c)
// {
//     volatile uint32_t hexVal = 0;
//     switch(c)
//     {
//         case '0' : hexVal = 0xc0; break;
//         //case '0' : hexVal = 192; break;
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
//         case 'I' : hexVal = 0xfb; break;
//         case 'J' : hexVal = 0xe1; break;
//         case 'K' : hexVal = 0x8a; break;
//         case 'L' : hexVal = 0xc7; break;
//         case 'M' : hexVal = 0xaa; break;
//         case 'N' : hexVal = 0xab; break;
//         case 'O' : hexVal = 0xc0; break;
//         //case 'O' : hexVal = 192; break;
//         case 'P' : hexVal = 0x8c; break;
//         case 'Q' : hexVal = 0x98; break;
//         case 'R' : hexVal = 0xce; break;
//         case 'S' : hexVal = 0x92; break;
//         case 'T' : hexVal = 0x87; break;
//         case 'U' : hexVal = 0xc3; break;
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

// void main(void)
// {
//     char const * str = "ARESIA";
//     volatile int *AD_DISPLAY1 = (int *)0x80000004;
//     volatile int *AD_DISPLAY2 = (int *)0x80000008;
    
//     volatile uint32_t len = 3;
//     volatile uint8_t hexVal[5];
    

//     for (uint32_t i = 0 ; i < len ; i++)
//     {
//         hexVal[i] = getCode7Seg(str[i]);
//         // hexVal[i] = 0x92;
//     }
//     // hexVal[0] = 0x88; //A
//     // hexVal[1] = 0xce; //R
//     // hexVal[2] = 0x86; //E
//     // hexVal[3] = 0x92; //S
//     // hexVal[4] = 0xfb; //I
//     // hexVal[5] = 0x88; //A
//     *AD_DISPLAY2=0xFFFFFFFF;
//     *AD_DISPLAY1=0xFFFFFFFF;
    
//     // *AD_DISPLAY2=0x8cce;
//     *AD_DISPLAY2=(/*(uint32_t)*/ hexVal[0]) << 8;
//     *AD_DISPLAY2|=(/*(uint32_t)*/ hexVal[1]);
    
//     *AD_DISPLAY1=(/*(uint32_t)*/ hexVal[2]) << 24;
//     *AD_DISPLAY1|=(/*(uint32_t)*/ hexVal[3]) << 16;
//     *AD_DISPLAY1|=(/*(uint32_t)*/ hexVal[4]) << 8;
//     *AD_DISPLAY1|=(/*(uint32_t)*/ hexVal[5]) ;
//     // *AD_DISPLAY1=0xc0c6bf86;
//     while(1);
    
// }


// ///////////////////////////////////////////////////////////////////////////////////////////////////////
// #define uint32_t unsigned int
// typedef __SIZE_TYPE__ size_t;
// // adresses in data memory

// int main(int argc, char *argv[]) {
//     ///////////////////VARIABLE////////////////
//     volatile int *AD_DISPLAY1 = (int *)0x80000004;
//     volatile int *AD_DISPLAY2 = (int *)0x80000008;
//     *AD_DISPLAY2=0xc0c0c0c0;
//     *AD_DISPLAY1=0xc0c0c0c0;
//     volatile uint32_t a,b,c ;
//     a = 0x123;
//     b = 0x321;
//     c = a+b;
//     while(1);
// }

///////////////////////////////////////////////////////////////////////////////////////////////////////

// #include <stdint.h>

// uint8_t getCode7Seg(uint8_t c)
// {
//     volatile uint8_t hexVal = 0;
//     switch(c)
//     {
//         case '0' : hexVal = 0xc0; break;
//         //case '0' : hexVal = 192; break;
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
//         case 'I' : hexVal = 0xfb; break;
//         case 'J' : hexVal = 0xe1; break;
//         case 'K' : hexVal = 0x8a; break;
//         case 'L' : hexVal = 0xc7; break;
//         case 'M' : hexVal = 0xaa; break;
//         case 'N' : hexVal = 0xab; break;
//         case 'O' : hexVal = 0xc0; break;
//         //case 'O' : hexVal = 192; break;
//         case 'P' : hexVal = 0x8c; break;
//         case 'Q' : hexVal = 0x98; break;
//         case 'R' : hexVal = 0xce; break;
//         case 'S' : hexVal = 0x92; break;
//         case 'T' : hexVal = 0x87; break;
//         case 'U' : hexVal = 0xc3; break;
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

// void main(void)
// {
//     char const * str = "PROCES";
//     volatile int *AD_DISPLAY1 = (int *)0x80000004;
//     volatile int *AD_DISPLAY2 = (int *)0x80000008;
    
//     volatile uint32_t len = 4;
//     volatile uint8_t hexVal[4];
    

//     // for (uint8_t i = 0 ; i < len ; i++)
//     // {
//     //     // hexVal[i] = getCode7Seg(str[i]);
//     //     hexVal[i] = getCode7Seg('S');
//     // }
//     *AD_DISPLAY2=0;
//     *AD_DISPLAY1=0;
    
//     *AD_DISPLAY2=0x8cce;
//     // *AD_DISPLAY2 =hexVal[0] << 8;
//     // *AD_DISPLAY2|=hexVal[1];
    
//     // *AD_DISPLAY1 =hexVal[2] << 24;
//     // *AD_DISPLAY1|=hexVal[3] << 16;
//     // *AD_DISPLAY1|=hexVal[4] << 8;
//     // *AD_DISPLAY1|=hexVal[5];
//     *AD_DISPLAY1=0xc0c6bf86;
//     while(1);
    
// }

// #define uint32_t unsigned int
// #define uint8_t unsigned char

// uint32_t getCode7Seg(uint8_t c)
// {
//     volatile uint8_t hexVal = 0;
//     switch(c)
//     {
//         case '0' : hexVal = 0xc0; break;
//         //case '0' : hexVal = 192; break;
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
//         case 'I' : hexVal = 0xfb; break;
//         case 'J' : hexVal = 0xe1; break;
//         case 'K' : hexVal = 0x8a; break;
//         case 'L' : hexVal = 0xc7; break;
//         case 'M' : hexVal = 0xaa; break;
//         case 'N' : hexVal = 0xab; break;
//         case 'O' : hexVal = 0xc0; break;
//         //case 'O' : hexVal = 192; break;
//         case 'P' : hexVal = 0x8c; break;
//         case 'Q' : hexVal = 0x98; break;
//         case 'R' : hexVal = 0xce; break;
//         case 'S' : hexVal = 0x92; break;
//         case 'T' : hexVal = 0x87; break;
//         case 'U' : hexVal = 0xc3; break;
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

// void main(void)
// {
//     volatile uint8_t * str = "1234";
//     volatile int *AD_DISPLAY1 = (int *)0x80000004;
//     volatile int *AD_DISPLAY2 = (int *)0x80000008;
    
//     volatile uint32_t len = 4;
//     volatile uint8_t hexVal[4];
    

//     for (uint8_t i = 0 ; i < len ; i++)
//     {
//         hexVal[i] = getCode7Seg(str[i]);
//         // hexVal[i] = getCode7Seg('S');
//     }
//     *AD_DISPLAY2=0;
//     *AD_DISPLAY1=0;
    
//     // *AD_DISPLAY2=0x8cce;
//     *AD_DISPLAY2=hexVal[0] << 8;
//     *AD_DISPLAY2|=hexVal[1];
    
//     *AD_DISPLAY1=hexVal[2] << 24;
//     *AD_DISPLAY1|=hexVal[3] << 16;
//     *AD_DISPLAY1|=hexVal[4] << 8;
//     *AD_DISPLAY1|=hexVal[5];
//     // *AD_DISPLAY1=0xc0c6bf86;
//     while(1);
// }


void main(void)
{
    volatile int *AD_DISPLAY1 = (int *)0x80000004;
    volatile int *AD_DISPLAY2 = (int *)0x80000FF8;
    
    *AD_DISPLAY2=0x83c0;
   
    *AD_DISPLAY1=0xc087ffff;
    while(1);
}