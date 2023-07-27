#define addrMax 0x2000
#define uint32_t unsigned int
#define uint8_t unsigned char
#define Tx_busy 1  //0001
#define Rx_error 2 //0010
#define Rx_full 4  //0100

void main(void)
{
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
    *AD_DISPLAY2=0xffff;     //ffff
    *AD_DISPLAY1=0x83c0c087; //BOOT

    // IMhex len
    for(int i = 3; i >= 0; i--){
        while ((*UART_Status & Rx_full) == 0);
        Lentmp = *Rx;
        while ((*UART_Status & Tx_busy) == 1);
        *Tx=Lentmp;
        Len |= Lentmp<<i*8;
    }

    // Transfer IMhex
    while (Len > 0 && ADDMax > addrSRAM){
        bufferWord = 0;
        // Concatenate 4 bytes to make 1 word of 32 bits 
        for(int i = 3; i >= 0; i--){
            while ((*UART_Status & Rx_full) == 0);
            receptUart=*Rx;            
            bufferWord |= (receptUart<<(i*8));
            Len = Len - 1;
        }
        // Store in SRAM
        *addrSRAM=bufferWord;
        // Load from SRAM 
        bufferWord=*addrSRAM;
        // Divide to resend in TX
        for(int i = 3; i>=0; i--){
            bufferByte = (bufferWord>>(i*8)) & 0xff;
            while ((*UART_Status & Tx_busy) == 1);
            *Tx = bufferByte;
        }

        addrSRAM = addrSRAM + 1;

    }
    // Resend through TX all SRAM
    addrSRAMB = 0;
    while (ADDMaxB > addrSRAMB){
        while ((*UART_Status & Tx_busy) == 1);
        *Tx = *addrSRAMB;
        addrSRAMB = addrSRAMB + 1;
    }
    while(1);
}