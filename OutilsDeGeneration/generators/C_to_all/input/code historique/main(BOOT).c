#define addrMax 0x7FFFFFFF
//#define Rx 0xC0000000
//#define Tx 0xC0000000
#define uint32_t unsigned int
#define uint8_t unsigned char
#define Tx_busy 1
#define Rx_full 4
#define Rx_error 2

void main(void)
{
        volatile int *AD_DISPLAY1 = (int *)0x80000004;
        volatile int *AD_DISPLAY2 = (int *)0x80000008;
    
    *AD_DISPLAY1=0xc087ffff;
    *AD_DISPLAY2=0x83c0;
    // uint32_t Lentmp;
    // uint32_t Len = 0x00000000;
    // uint8_t *addrSRAM = (uint8_t*)0x00000000;
    // uint32_t receptUart = 0;
    // uint32_t *UART_Status = (uint32_t*)0xC0000000;
    // uint32_t *Rx = (uint32_t*)0xC0000004;
    // uint32_t *Tx = (uint32_t*)0xC0000008;
    // uint8_t *ADDMax = (uint8_t*)addrMax;
    
    // for(int i = 0; i < 4; i++){
    //     while ((*UART_Status & Rx_full) != 0);
    //     while (*Rx==0);
    //     Lentmp = *Rx;
    //     while ((*UART_Status & Tx_busy) != 0);
    //     *Tx=0x30;
    //     //*Tx=Lentmp;
    //     Len |= Lentmp<<i*8;
    // }
    // while(Len > 0 && ADDMax > addrSRAM){
    //     while ((*UART_Status & Rx_full) != 0);
    //     while(*Rx == 0);
    //     receptUart=*Rx;
    //     while ((*UART_Status & Tx_busy) != 0);
    //     *Tx=0x30;
    //     //*Tx=receptUart;
    //     *addrSRAM=receptUart;
    //     Len--;
    //     addrSRAM++;
    // }

    while(1);
}