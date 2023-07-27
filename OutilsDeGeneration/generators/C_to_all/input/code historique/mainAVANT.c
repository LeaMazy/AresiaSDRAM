// #include <stdint.h>

// adresses in data memory
// #define AD_CPT32    *((volatile uint32_t *)0x80000000)
// #define AD_DISPLAY1 *((volatile uint32_t *)0x80000004)   // MSB=Hex3, Hex2, Hex1, LSB=Hex0 (Hex described in DE10LITE User manual)
// #define AD_DISPLAY2 *((volatile uint32_t *)0x80000005) 
// #define AD_DISPLAY3 *((volatile uint32_t *)0x80000006) 
// #define AD_DISPLAY4 *((volatile uint32_t *)0x80000007) 
// #define AD_DISPLAY5 *((volatile uint32_t *)0x80000008)   // MSB=X, X, Hex5, LSB=Hex4
// #define AD_DISPLAY6 *((volatile uint32_t *)0x80000009)   // MSB=X, X, Hex5, LSB=Hex4
// #define MASTER_CLK_RATE 50 * 1000000
// #define PLL_DIV  50
// #define CLK_RATE    MASTER_CLK_RATE/PLL_DIV
// #define MS_TICK    CLK_RATE/1000
// #define DISPLAY_NB 6

// void calcDisp(uint8_t pos, uint8_t* hexVal, uint8_t len)
// {
    // uint32_t disp1 = 0;
    // uint32_t disp2 = 0;
    
    // for (uint8_t i = DISPLAY_NB ; i > 4 ; i--)
    // {
        // uint8_t shift = ((i-5)*8);
        // int8_t index = (pos-i);
        // uint8_t condition = (index >= 0 && index < len);
        // disp2 |= (condition ? (hexVal[index] << shift) : (0xff << shift));
    // }
    
    // for (uint8_t i = 4 ; i > 0 ; i--)
    // {
        // uint8_t shift = ((i-1)*8);
        // int8_t index = (pos-i);
        // uint8_t condition = (index >= 0 && index < len);
        // disp1 |= (condition ? (hexVal[index] << shift) : (0xff << shift));
    // }
    // AD_DISPLAY2 = disp2;
    // AD_DISPLAY1 = disp1;
// }

// uint8_t getCode7Seg(uint8_t c)
// {
	// volatile uint32_t hexVal = 0;
    // switch(c)
    // {
        // case '0' : hexVal = 0xc0; break;
        // case '1' : hexVal = 0xf9; break;
        // case '2' : hexVal = 0xa4; break;
        // case '3' : hexVal = 0xb0; break;
        // case '4' : hexVal = 0x99; break;
        // case '5' : hexVal = 0x92; break;
        // case '6' : hexVal = 0x82; break;
        // case '7' : hexVal = 0xf8; break;
        // case '8' : hexVal = 0x80; break;
        // case '9' : hexVal = 0x90; break;
        // case 'A' : hexVal = 0x88; break;
        // case 'B' : hexVal = 0x83; break;
        // case 'C' : hexVal = 0xc6; break;
        // case 'D' : hexVal = 0xa1; break;
        // case 'E' : hexVal = 0x86; break;
        // case 'F' : hexVal = 0x8e; break;
        // case 'G' : hexVal = 0xc2; break;
        // case 'H' : hexVal = 0x8b; break;
        // case 'I' : hexVal = 0xfb; break;
        // case 'J' : hexVal = 0xe1; break;
        // case 'K' : hexVal = 0x8a; break;
        // case 'L' : hexVal = 0xc7; break;
        // case 'M' : hexVal = 0xaa; break;
        // case 'N' : hexVal = 0xab; break;
        // case 'O' : hexVal = 0xc0; break;
        // case 'P' : hexVal = 0x8c; break;
        // case 'Q' : hexVal = 0x98; break;
        // case 'R' : hexVal = 0xce; break;
        // case 'S' : hexVal = 0x92; break;
        // case 'T' : hexVal = 0x87; break;
        // case 'U' : hexVal = 0xc3; break;
        // case 'V' : hexVal = 0xb5; break;
        // case 'W' : hexVal = 0x95; break;
        // case 'X' : hexVal = 0x89; break;
        // case 'Y' : hexVal = 0x91; break;
        // case 'Z' : hexVal = 0xa4; break;
        // case ' ' : hexVal = 0xff; break;
        // case '.' : hexVal = 0x7f; break;
        // case '-' : hexVal = 0xbf; break;
        // case '_' : hexVal = 0xf7; break;
        // default  : hexVal = 0x7f; break;
	// }
    // return hexVal;
// }

void main(void)
{
    //char const * str = "PROCES";
	volatile int *AD_DISPLAY1 = (int *)0x80000004;
	volatile int *AD_DISPLAY2 = (int *)0x80000005;
	volatile int *AD_DISPLAY3 = (int *)0x80000006;
	volatile int *AD_DISPLAY4 = (int *)0x80000007;
	volatile int *AD_DISPLAY5 = (int *)0x80000008;
	volatile int *AD_DISPLAY6 = (int *)0x80000009;
    // volatile uint32_t len = 8;
    volatile int hexVal[6]= {0x8c,0xce,0xc0,0xc6,0xbf,0x86};
    
    // for (uint8_t i = 0 ; str[i] != '\0' ; i++)
    // {
        //len++;
    // }

    // for (uint8_t i = 0 ; i < len ; i++)
    // {
        // hexVal[i] = getCode7Seg(str[i]);
    // }
	
	*AD_DISPLAY1=hexVal[0];
	*AD_DISPLAY2=hexVal[1];
	*AD_DISPLAY3=hexVal[2];
	*AD_DISPLAY4=hexVal[3];
	*AD_DISPLAY5=hexVal[4];
	*AD_DISPLAY6=hexVal[5];

	//AD_DISPLAY2 = disp2;
	
    //AD_DISPLAY1 = disp1;
    //calcDisp(1, hexVal, len);
    // for(;;)
    // {
        // for (uint8_t pos = 0 ; pos < (len + DISPLAY_NB) ; pos++)
        // {
            // calcDisp(pos, hexVal, len);
            // while(AD_CPT32 < (MS_TICK * 500));  //500ms delay
            // AD_CPT32 = 0;
        // }
    // }
}