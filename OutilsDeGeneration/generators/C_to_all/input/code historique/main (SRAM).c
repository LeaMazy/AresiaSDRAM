#define uint32_t unsigned int
typedef __SIZE_TYPE__ size_t;
// adresses in data memory
#define AD_TOPleds  *((volatile uint32_t *)0x8000000c)
#define AD_DISPLAY1 *((volatile uint32_t *)0x80000004)  // MSB=Hex3, Hex2, Hex1, LSB=Hex0 (Hex described in DE10LITE User manual)
#define AD_DISPLAY2 *((volatile uint32_t *)0x80000008)  // MSB=X, X, Hex5, LSB=Hex4
#define MASTER_CLK_RATE 50 * 1000000
#define PLL_DIV  50
#define CLK_RATE    MASTER_CLK_RATE/PLL_DIV
#define MS_TICK    CLK_RATE/1000
#define DISPLAY_NB 6

void wait(int delay){
	volatile uint32_t delayCPT=0;
	for(int i=0; i<delay; i++){
		delayCPT++;
	}
	delayCPT=0;
}

int main(int argc, char *argv[]) {
	///////////////////VARIABLE////////////////

	volatile uint32_t ledPos=1;
	volatile uint32_t sens = 1; // 1 vers la droite, 0 vers la gauche

	
	////////////////INITIALISATION/////////////
	AD_DISPLAY1 = 0xFFFFFFFF;
	AD_DISPLAY2 = 0xffff;
	AD_TOPleds = 0xccc;
	
	////////////////////CODE///////////////////

    for(;;)
    {
		for(uint32_t i=0; i<5; i++){
			AD_TOPleds = ledPos;
			if(ledPos==512) sens=1;
			if(ledPos==1) sens=0;
			if(sens==1){ ledPos = ledPos>>1;}
			else{ 		ledPos = ledPos<<1; }
			wait(1000);
		}
    }
	
	return 0;
}

