#define uint32_t unsigned int
typedef __SIZE_TYPE__ size_t;
// adresses in data memory
#define AD_TOPleds    *((volatile uint32_t *)0x8000000c)
#define AD_DISPLAY1 *((volatile uint32_t *)0x80000004)   // MSB=Hex3, Hex2, Hex1, LSB=Hex0 (Hex described in DE10LITE User manual)
#define AD_DISPLAY2 *((volatile uint32_t *)0x80000008)   // MSB=X, X, Hex5, LSB=Hex4
#define MASTER_CLK_RATE 50 * 1000000
#define PLL_DIV  50
#define CLK_RATE    MASTER_CLK_RATE/PLL_DIV
#define MS_TICK    CLK_RATE/1000
#define DISPLAY_NB 6

int main(int argc, char *argv[]) {
	///////////////////VARIABLE////////////////
	//int error=0;
	volatile int dogo=0;
	volatile int dogo2=0;
	// volatile int A=0x12345678;
	volatile int result=0;
	// volatile int *PTI;
	volatile char *PTC, *PTT;
	
	PTC = (int *) &dogo;
	PTT = (int *) &dogo2;
	
	if ((*PTC & 3)!=0 && (*PTT & 3)!= 0){
		result=1;
	}else{
		result=2;
	}

	return 0;
}

