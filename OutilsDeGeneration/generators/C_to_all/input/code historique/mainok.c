#define uint32_t unsigned int
typedef __SIZE_TYPE__ size_t;
// adresses in data memory
#define AD_CPT32    *((volatile uint32_t *)0x80000000)
#define AD_TOPleds    *((volatile uint32_t *)0x8000000c)
#define AD_DISPLAY1 *((volatile uint32_t *)0x80000004)   // MSB=Hex3, Hex2, Hex1, LSB=Hex0 (Hex described in DE10LITE User manual)
#define AD_DISPLAY2 *((volatile uint32_t *)0x80000008)   // MSB=X, X, Hex5, LSB=Hex4
// #define MASTER_CLK_RATE 50 * 1000000
// #define PLL_DIV  50
// #define CLK_RATE    MASTER_CLK_RATE/PLL_DIV
// #define MS_TICK    CLK_RATE/1000
// #define DISPLAY_NB 6

// void calcDisp(int pos, int* hexVal, int len)
// {
    // uint32_t disp1 = 0;
    // uint32_t disp2 = 0;
    
    // for (int i = DISPLAY_NB ; i > 4 ; i--)
    // {
        // int shift = ((i-5)*8);
        // int index = (pos-i);
        // int condition = (index >= 0 && index < len);
        // disp2 |= (condition ? (hexVal[index] << shift) : (0xff << shift));
    // }
    
    // for (int i = 4 ; i > 0 ; i--)
    // {
        // int shift = ((i-1)*8);
        // int index = (pos-i);
        // int condition = (index >= 0 && index < len);
        // disp1 |= (condition ? (hexVal[index] << shift) : (0xff << shift));
    // }
    // AD_DISPLAY2 = disp2;
    // AD_DISPLAY1 = disp1;
// }

int getCode7SegNum(int c)
{
	int hexVal = 0;
    switch(c)
    {
        case 0 : hexVal = 192; break;
        case 1 : hexVal = 0xf9; break;
        case 2 : hexVal = 0xa4; break;
        case 3 : hexVal = 0xb0; break;
        case 4 : hexVal = 0x99; break;
        case 5 : hexVal = 0x92; break;
        case 6 : hexVal = 0x82; break;
        case 7 : hexVal = 0xf8; break;
        case 8 : hexVal = 0x80; break;
        case 9 : hexVal = 0x90; break;
        case 10 : hexVal = 0x88; break;
        case 11 : hexVal = 0x83; break;
        case 12 : hexVal = 0xc6; break;
        case 13 : hexVal = 0xa1; break;
        case 14 : hexVal = 0x86; break;
        case 15 : hexVal = 0x8e; break;
        
        default  : hexVal = 0x7f; break;
	}
    return hexVal;
}

uint32_t getCode7Seg(char c)
{
	uint32_t hexVal=0;
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
        case 'I' : hexVal = 0xfb; break;
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
    return hexVal;
}

void *memcpy(void *restrict d, const void *restrict s, size_t n){
    const char *s8 = s;
    char *d8 = d;
    while (n--)
        *(d8++) = *(s8++);
    return d;
}

// void delay(int delay){
	// for(int i=0; i<delay; i++){
		// i++;
	// }
	// return;
// }

// void char2seg(char * str){
	// for (int i = 0 ; str[i] != '\0' ; i++)
    // {
        // hexVal[i] = getCode7Seg(str[i]);
    // }
// }

void main(void){
	
	////////////////////// VARIABLES///////////////////////
	volatile char *str = "HELLOW";
	volatile uint32_t len = 6;
	volatile int hexVal[6]= {0x8c,0xce,0xc0,0xc6,0xbf,0x86};
	
	
	/////////////////////INITIALISATION///////////////////
	AD_DISPLAY1 = 0xffffffff;
	AD_DISPLAY2 = 0xffff;
	AD_TOPleds=15;
	
	/////////////////////CODE////////////////////////////

	// AD_TOPleds = len;
	
	// for (int i = 0 ; i<len ; i++){
		// hexVal[i] = getCode7Seg(str[i]);
	// }
	
	// hexVal[0] = getCode7SegNum(len);
	// hexVal[0] = getCode7Seg(str[0]);
	// hexVal[1] = getCode7Seg(str[1]);
	// hexVal[2] = getCode7Seg(str[2]);
	// hexVal[3] = getCode7Seg(str[3]);
	// hexVal[4] = getCode7Seg(str[4]);
	// hexVal[5] = getCode7Seg(str[5]);
// str[i] != '\0'
    // for (int i = 0 ; i<len; i++)
    // {
        // hexVal[i] = getCode7Seg(str[i]);
    // }

	// AD_DISPLAY1 = (hexVal[2] << 24);
	// AD_DISPLAY1|= (hexVal[3] << 16);
	// AD_DISPLAY1|= (hexVal[4] << 8);
	// AD_DISPLAY1|= (hexVal[5]);
	
	// AD_DISPLAY2 = (hexVal[0] << 8);
	// AD_DISPLAY2 |= (hexVal[1]);
	
	// delay(200000);
	
	// str="TEST2";
	// for (int i = 0 ; str[i] != '\0' ; i++)
    // {
        // hexVal[i] = getCode7Seg(str[i]);
    // }
	
	// AD_DISPLAY1 = (hexVal[2] << 24);
	// AD_DISPLAY1|= (hexVal[3] << 16);
	// AD_DISPLAY1|= (hexVal[4] << 8);
	// AD_DISPLAY1|= (hexVal[5]);
	
	// AD_DISPLAY2 = (hexVal[0] << 8);
	// AD_DISPLAY2 |= (hexVal[1]);
	// delay(20000);
	
	
	
    // calcDisp(1, hexVal, len);
    // for(;;)
    // {
        // for (int pos = 0 ; pos < (len + DISPLAY_NB) ; pos++)
        // {
            // calcDisp(pos, hexVal, len);
            // while(AD_CPT32 < (MS_TICK * 500));  //500ms delay
            // AD_CPT32 = 0;
        // }
    // }
}

