
main.elf:     file format elf32-littleriscv


Disassembly of section .text:

00000000 <_start>:
   0:	00001137          	lui	sp,0x1
   4:	00c000ef          	jal	ra,10 <main>
   8:	00100073          	ebreak
   c:	0000006f          	j	c <_start+0xc>

00000010 <main>:
//     }
//     delayCPT=0;
// }

void main(void)
{
  10:	ff010113          	addi	sp,sp,-16 # ff0 <main+0xfe0>
    uint32_t *ADDMax = (uint32_t*)addrMax;
    uint8_t *addrSRAMB = (uint8_t*)0x00000000;
    uint8_t *ADDMaxB = (uint8_t*)addrMax;
    uint32_t Lentmp;
    uint32_t Len = 0x00000000;
    volatile uint32_t receptUart = 0;
  14:	00012623          	sw	zero,12(sp)
    volatile uint32_t bufferWord = 0;
  18:	00012423          	sw	zero,8(sp)
    volatile uint32_t bufferByte = 0; 
  1c:	00012223          	sw	zero,4(sp)
    volatile uint32_t *Tx = (uint32_t*)0xC0000004;

    volatile int *AD_DISPLAY1 = (int *)0x80000004;
    volatile int *AD_DISPLAY2 = (int *)0x80000008;
    int tmp;
    *AD_DISPLAY2=0xff83;     //BO
  20:	80000737          	lui	a4,0x80000
  24:	000107b7          	lui	a5,0x10
  28:	f8378793          	addi	a5,a5,-125 # ff83 <main+0xff73>
  2c:	00f72423          	sw	a5,8(a4) # 80000008 <main+0x7ffffff8>
    *AD_DISPLAY1=0xc0c087ff; //OT
  30:	c0c087b7          	lui	a5,0xc0c08
  34:	7ff78793          	addi	a5,a5,2047 # c0c087ff <main+0xc0c087ef>
  38:	00f72223          	sw	a5,4(a4)
  3c:	01800693          	li	a3,24
    uint32_t Len = 0x00000000;
  40:	00000513          	li	a0,0
    //     while ((*UART_Status & Tx_busy) == 1);
    //     *Tx=Lentmp;
    //     // Len |= Lentmp<<i*8;
    // }
    for(int i = 3; i >= 0; i--){
        while ((*UART_Status & Rx_full) == 0);
  44:	c0000737          	lui	a4,0xc0000
    for(int i = 3; i >= 0; i--){
  48:	ff800593          	li	a1,-8
        while ((*UART_Status & Rx_full) == 0);
  4c:	00072783          	lw	a5,0(a4) # c0000000 <main+0xbffffff0>
  50:	0047f793          	andi	a5,a5,4
  54:	fe078ce3          	beqz	a5,4c <main+0x3c>
        Lentmp = *Rx;
  58:	00472603          	lw	a2,4(a4)
        while ((*UART_Status & Tx_busy) == 1);
  5c:	00072783          	lw	a5,0(a4)
  60:	0017f793          	andi	a5,a5,1
  64:	fe079ce3          	bnez	a5,5c <main+0x4c>
        *Tx=Lentmp;
  68:	00c72223          	sw	a2,4(a4)
        Len |= Lentmp<<i*8;
  6c:	00d61633          	sll	a2,a2,a3
  70:	00c56533          	or	a0,a0,a2
    for(int i = 3; i >= 0; i--){
  74:	ff868693          	addi	a3,a3,-8
  78:	fcb69ae3          	bne	a3,a1,4c <main+0x3c>

    }

    // Len = 20;
    while (Len > 0 && ADDMax >= addrSRAM){
  7c:	0e050463          	beqz	a0,164 <main+0x154>
  80:	ffffe337          	lui	t1,0xffffe
  84:	ffc30313          	addi	t1,t1,-4 # ffffdffc <main+0xffffdfec>
  88:	00650333          	add	t1,a0,t1
        Len |= Lentmp<<i*8;
  8c:	00050893          	mv	a7,a0
        bufferWord = 0;
        for(int i = 3; i >= 0; i--){
            while (((*UART_Status & Rx_full) == 0) && ((*UART_Status & Rx_error) == 0));
  90:	c00007b7          	lui	a5,0xc0000
                //     *AD_DISPLAY1=0x86;
                //     while(1);
                // }
            }    
            receptUart=*Rx;
            *AD_DISPLAY1=(receptUart<<(16));
  94:	80000637          	lui	a2,0x80000
        for(int i = 3; i >= 0; i--){
  98:	ff800593          	li	a1,-8
  9c:	0b80006f          	j	154 <main+0x144>
                tmp=*UART_Status;
  a0:	0007a703          	lw	a4,0(a5) # c0000000 <main+0xbffffff0>
                if((tmp & Rx_full) != 0) break;
  a4:	00477713          	andi	a4,a4,4
  a8:	fe070ce3          	beqz	a4,a0 <main+0x90>
            receptUart=*Rx;
  ac:	0047a703          	lw	a4,4(a5)
  b0:	00e12623          	sw	a4,12(sp)
            *AD_DISPLAY1=(receptUart<<(16));
  b4:	00c12703          	lw	a4,12(sp)
  b8:	01071713          	slli	a4,a4,0x10
  bc:	00e62223          	sw	a4,4(a2) # 80000004 <main+0x7ffffff4>
            bufferWord |= (receptUart<<(i*8));
  c0:	00c12703          	lw	a4,12(sp)
  c4:	00812803          	lw	a6,8(sp)
  c8:	00d71733          	sll	a4,a4,a3
  cc:	01076733          	or	a4,a4,a6
  d0:	00e12423          	sw	a4,8(sp)
        for(int i = 3; i >= 0; i--){
  d4:	ff868693          	addi	a3,a3,-8
  d8:	02b68063          	beq	a3,a1,f8 <main+0xe8>
            while (((*UART_Status & Rx_full) == 0) && ((*UART_Status & Rx_error) == 0));
  dc:	0007a703          	lw	a4,0(a5)
  e0:	00477713          	andi	a4,a4,4
  e4:	fa071ee3          	bnez	a4,a0 <main+0x90>
  e8:	0007a703          	lw	a4,0(a5)
  ec:	00277713          	andi	a4,a4,2
  f0:	fe0706e3          	beqz	a4,dc <main+0xcc>
  f4:	fadff06f          	j	a0 <main+0x90>
            // while ((*UART_Status & Tx_busy) != 0);
            // while ((*UART_Status & Tx_busy) == 1);

            // *Tx = receptUart;
            // *Tx = 0x46;
            Len = Len - 1;
  f8:	ffc88893          	addi	a7,a7,-4
            // *AD_DISPLAY1 |= receptUart;
            // *AD_DISPLAY1 |= 0x46;
        }

        *addrSRAM=bufferWord;
  fc:	00812703          	lw	a4,8(sp)
 100:	00ee2023          	sw	a4,0(t3)
        bufferWord=0;
 104:	00012423          	sw	zero,8(sp)
        bufferWord=*addrSRAM;
 108:	00e12423          	sw	a4,8(sp)
 10c:	01800693          	li	a3,24
        for(int i = 3; i>=0; i--){
            bufferByte = (bufferWord>>(i*8)) & 0xff;
 110:	00812703          	lw	a4,8(sp)
 114:	00d75733          	srl	a4,a4,a3
 118:	0ff77713          	zext.b	a4,a4
 11c:	00e12223          	sw	a4,4(sp)
            while ((*UART_Status & Tx_busy) == 1);
 120:	0007a703          	lw	a4,0(a5)
 124:	00177713          	andi	a4,a4,1
 128:	fe071ce3          	bnez	a4,120 <main+0x110>
            *Tx = bufferByte;
 12c:	00412703          	lw	a4,4(sp)
 130:	00e7a223          	sw	a4,4(a5)
            *AD_DISPLAY1 |= bufferByte;
 134:	00412703          	lw	a4,4(sp)
 138:	00462803          	lw	a6,4(a2)
 13c:	01076733          	or	a4,a4,a6
 140:	00e62223          	sw	a4,4(a2)
        for(int i = 3; i>=0; i--){
 144:	ff868693          	addi	a3,a3,-8
 148:	fcb694e3          	bne	a3,a1,110 <main+0x100>
    while (Len > 0 && ADDMax >= addrSRAM){
 14c:	00088c63          	beqz	a7,164 <main+0x154>
 150:	01130a63          	beq	t1,a7,164 <main+0x154>
 154:	41150e33          	sub	t3,a0,a7
        bufferWord = 0;
 158:	00012423          	sw	zero,8(sp)
 15c:	01800693          	li	a3,24
 160:	f7dff06f          	j	dc <main+0xcc>
        bufferWord=*addrSRAM;
 164:	00000813          	li	a6,0
    while (ADDMax > addrSRAM){
        bufferWord=0;
        bufferWord=*addrSRAM;
        for(int i = 3; i>=0; i--){
            bufferByte = (bufferWord>>(i*8)) & 0xff;
            while ((*UART_Status & Tx_busy) == 1);
 168:	c0000737          	lui	a4,0xc0000
            *Tx = bufferByte;
            *AD_DISPLAY1 |= bufferByte;
 16c:	80000637          	lui	a2,0x80000
        for(int i = 3; i>=0; i--){
 170:	ff800513          	li	a0,-8
    while (ADDMax > addrSRAM){
 174:	000028b7          	lui	a7,0x2
 178:	00c0006f          	j	184 <main+0x174>
        }

        addrSRAM = addrSRAM + 1;
 17c:	00480813          	addi	a6,a6,4
    while (ADDMax > addrSRAM){
 180:	05180a63          	beq	a6,a7,1d4 <main+0x1c4>
        bufferWord=0;
 184:	00012423          	sw	zero,8(sp)
        bufferWord=*addrSRAM;
 188:	00082783          	lw	a5,0(a6)
 18c:	00f12423          	sw	a5,8(sp)
 190:	01800693          	li	a3,24
            bufferByte = (bufferWord>>(i*8)) & 0xff;
 194:	00812783          	lw	a5,8(sp)
 198:	00d7d7b3          	srl	a5,a5,a3
 19c:	0ff7f793          	zext.b	a5,a5
 1a0:	00f12223          	sw	a5,4(sp)
            while ((*UART_Status & Tx_busy) == 1);
 1a4:	00072783          	lw	a5,0(a4) # c0000000 <main+0xbffffff0>
 1a8:	0017f793          	andi	a5,a5,1
 1ac:	fe079ce3          	bnez	a5,1a4 <main+0x194>
            *Tx = bufferByte;
 1b0:	00412783          	lw	a5,4(sp)
 1b4:	00f72223          	sw	a5,4(a4)
            *AD_DISPLAY1 |= bufferByte;
 1b8:	00412783          	lw	a5,4(sp)
 1bc:	00462583          	lw	a1,4(a2) # 80000004 <main+0x7ffffff4>
 1c0:	00b7e7b3          	or	a5,a5,a1
 1c4:	00f62223          	sw	a5,4(a2)
        for(int i = 3; i>=0; i--){
 1c8:	ff868693          	addi	a3,a3,-8
 1cc:	fca694e3          	bne	a3,a0,194 <main+0x184>
 1d0:	fadff06f          	j	17c <main+0x16c>
    }
    // Tx = (uint32_t *)1;
    // while(Tx==(uint32_t *)1){
    //     Tx = Tx + (*UART_Status);
    // }
    while(1);
 1d4:	0000006f          	j	1d4 <main+0x1c4>

Disassembly of section .riscv.attributes:

00000000 <.riscv.attributes>:
   0:	1b41                	addi	s6,s6,-16
   2:	0000                	unimp
   4:	7200                	flw	fs0,32(a2)
   6:	7369                	lui	t1,0xffffa
   8:	01007663          	bgeu	zero,a6,14 <main+0x4>
   c:	0011                	c.nop	4
   e:	0000                	unimp
{
  10:	1004                	addi	s1,sp,32
  12:	7205                	lui	tp,0xfffe1
    volatile uint32_t receptUart = 0;
  14:	3376                	fld	ft6,376(sp)
  16:	6932                	flw	fs2,12(sp)
    volatile uint32_t bufferWord = 0;
  18:	7032                	flw	ft0,44(sp)
  1a:	0030                	addi	a2,sp,8

Disassembly of section .comment:

00000000 <.comment>:
   0:	3a434347          	fmsub.d	ft6,ft6,ft4,ft7,rmm
   4:	2820                	fld	fs0,80(s0)
   6:	736f7263          	bgeu	t5,s6,72a <main+0x71a>
   a:	6f6f7473          	csrrci	s0,0x6f6,30
   e:	2d6c                	fld	fa1,216(a0)
{
  10:	474e                	lw	a4,208(sp)
  12:	3120                	fld	fs0,96(a0)
    volatile uint32_t receptUart = 0;
  14:	322e                	fld	ft4,232(sp)
  16:	2e34                	fld	fa3,88(a2)
    volatile uint32_t bufferWord = 0;
  18:	2e30                	fld	fa2,88(a2)
  1a:	3035                	jal	fffff846 <main+0xfffff836>
    volatile uint32_t bufferByte = 0; 
  1c:	5f30                	lw	a2,120(a4)
  1e:	3835                	jal	fffff85a <main+0xfffff84a>
    *AD_DISPLAY2=0xff83;     //BO
  20:	6534                	flw	fa3,72(a0)
  22:	3735                	jal	ffffff4e <main+0xffffff3e>
  24:	2965                	jal	4dc <main+0x4cc>
  26:	3120                	fld	fs0,96(a0)
  28:	2e31                	jal	344 <main+0x334>
  2a:	2e32                	fld	ft8,264(sp)
  2c:	0030                	addi	a2,sp,8

Disassembly of section .debug_info:

00000000 <.debug_info>:
   0:	01a1                	addi	gp,gp,8
   2:	0000                	unimp
   4:	0005                	c.nop	1
   6:	0401                	addi	s0,s0,0
   8:	0000                	unimp
   a:	0000                	unimp
   c:	00000d0b          	0xd0b
{
  10:	1d00                	addi	s0,sp,688
  12:	0000005b          	0x5b
    volatile uint32_t receptUart = 0;
  16:	0000                	unimp
    volatile uint32_t bufferWord = 0;
  18:	0000                	unimp
  1a:	0010                	0x10
    volatile uint32_t bufferByte = 0; 
  1c:	0000                	unimp
  1e:	01c8                	addi	a0,sp,196
    *AD_DISPLAY2=0xff83;     //BO
  20:	0000                	unimp
  22:	0000                	unimp
  24:	0000                	unimp
  26:	ab0c                	fsd	fa1,16(a4)
  28:	0000                	unimp
  2a:	0100                	addi	s0,sp,128
  2c:	0351                	addi	t1,t1,20
  2e:	1006                	c.slli	zero,0x21
    *AD_DISPLAY1=0xc0c087ff; //OT
  30:	0000                	unimp
  32:	c800                	sw	s0,16(s0)
  34:	0001                	nop
  36:	0100                	addi	s0,sp,128
  38:	719c                	flw	fa5,32(a1)
  3a:	0001                	nop
  3c:	0600                	addi	s0,sp,768
  3e:	000000c3          	fmadd.s	ft1,ft0,ft0,ft0,rne
    uint32_t Len = 0x00000000;
  42:	0355                	addi	t1,t1,21
        while ((*UART_Status & Rx_full) == 0);
  44:	0001710f          	0x1710f
    for(int i = 3; i >= 0; i--){
  48:	0c00                	addi	s0,sp,528
  4a:	0000                	unimp
        while ((*UART_Status & Rx_full) == 0);
  4c:	0700                	addi	s0,sp,896
  4e:	0056                	c.slli	zero,0x15
  50:	0000                	unimp
  52:	0356                	slli	t1,t1,0x15
  54:	0001710f          	0x1710f
        Lentmp = *Rx;
  58:	0d00                	addi	s0,sp,656
  5a:	00a1                	addi	ra,ra,8
        while ((*UART_Status & Tx_busy) == 1);
  5c:	0000                	unimp
  5e:	5701                	li	a4,-32
  60:	01820e03          	lb	t3,24(tp) # fffe1018 <main+0xfffe1008>
  64:	0000                	unimp
  66:	0700                	addi	s0,sp,896
        *Tx=Lentmp;
  68:	000000bb          	0xbb
        Len |= Lentmp<<i*8;
  6c:	0358                	addi	a4,sp,388
  6e:	820e                	mv	tp,gp
  70:	0001                	nop
  72:	0600                	addi	s0,sp,768
    for(int i = 3; i >= 0; i--){
  74:	0075                	c.nop	29
  76:	0000                	unimp
  78:	0359                	addi	t1,t1,22
  7a:	760e                	flw	fa2,224(sp)
    while (Len > 0 && ADDMax >= addrSRAM){
  7c:	0001                	nop
  7e:	5600                	lw	s0,40(a2)
  80:	0000                	unimp
  82:	0100                	addi	s0,sp,128
  84:	654c                	flw	fa1,12(a0)
  86:	006e                	c.slli	zero,0x1b
  88:	035a                	slli	t1,t1,0x16
  8a:	760e                	flw	fa2,224(sp)
        Len |= Lentmp<<i*8;
  8c:	0001                	nop
  8e:	6e00                	flw	fs0,24(a2)
            while (((*UART_Status & Rx_full) == 0) && ((*UART_Status & Rx_error) == 0));
  90:	0000                	unimp
  92:	0300                	addi	s0,sp,384
            *AD_DISPLAY1=(receptUart<<(16));
  94:	0000004b          	fnmsub.s	ft0,ft0,ft0,ft0,rne
        for(int i = 3; i >= 0; i--){
  98:	017d035b          	0x17d035b
  9c:	0000                	unimp
  9e:	9102                	jalr	sp
                tmp=*UART_Status;
  a0:	037c                	addi	a5,sp,396
  a2:	00b0                	addi	a2,sp,72
                if((tmp & Rx_full) != 0) break;
  a4:	0000                	unimp
  a6:	035c                	addi	a5,sp,388
  a8:	017d                	addi	sp,sp,31
  aa:	0000                	unimp
            receptUart=*Rx;
  ac:	9102                	jalr	sp
  ae:	0378                	addi	a4,sp,396
  b0:	007c                	addi	a5,sp,12
  b2:	0000                	unimp
            *AD_DISPLAY1=(receptUart<<(16));
  b4:	035d                	addi	t1,t1,23
  b6:	017d                	addi	sp,sp,31
  b8:	0000                	unimp
  ba:	9102                	jalr	sp
  bc:	0474                	addi	a3,sp,524
  be:	00000087          	0x87
            bufferWord |= (receptUart<<(i*8));
  c2:	035e                	slli	t1,t1,0x17
  c4:	8e18                	0x8e18
  c6:	0001                	nop
  c8:	8000                	0x8000
  ca:	8080                	0x8080
  cc:	7c80                	flw	fs0,56(s1)
  ce:	5208                	lw	a0,32(a2)
  d0:	0078                	addi	a4,sp,12
  d2:	035f 018e 0000      	0x18e035f
        for(int i = 3; i >= 0; i--){
  d8:	5408                	lw	a0,40(s0)
  da:	0078                	addi	a4,sp,12
            while (((*UART_Status & Rx_full) == 0) && ((*UART_Status & Rx_error) == 0));
  dc:	0360                	addi	s0,sp,396
  de:	018e                	slli	gp,gp,0x3
  e0:	0000                	unimp
  e2:	5d04                	lw	s1,56(a0)
  e4:	0000                	unimp
  e6:	6200                	flw	fs0,0(a2)
  e8:	01931303          	lh	t1,25(t1) # ffffa019 <main+0xffffa009>
  ec:	0000                	unimp
  ee:	8084                	0x8084
  f0:	8080                	0x8080
  f2:	0478                	addi	a4,sp,524
  f4:	0069                	c.nop	26
  f6:	0000                	unimp
            Len = Len - 1;
  f8:	93130363          	beq	t1,a7,fffff21e <main+0xfffff20e>
        *addrSRAM=bufferWord;
  fc:	0001                	nop
  fe:	8800                	0x8800
 100:	8080                	0x8080
 102:	7880                	flw	fs0,48(s1)
        bufferWord=0;
 104:	7401                	lui	s0,0xfffe0
 106:	706d                	c.lui	zero,0xffffb
        bufferWord=*addrSRAM;
 108:	6400                	flw	fs0,8(s0)
 10a:	01980903          	lb	s2,25(a6)
 10e:	0000                	unimp
            bufferByte = (bufferWord>>(i*8)) & 0xff;
 110:	0091                	addi	ra,ra,4
 112:	0000                	unimp
 114:	0c05                	addi	s8,s8,1
 116:	0000                	unimp
 118:	2c00                	fld	fs0,24(s0)
 11a:	0001                	nop
 11c:	0100                	addi	s0,sp,128
 11e:	0069                	c.nop	26
            while ((*UART_Status & Tx_busy) == 1);
 120:	0370                	addi	a2,sp,396
 122:	980d                	andi	s0,s0,-29
 124:	0001                	nop
 126:	9d00                	0x9d00
 128:	0000                	unimp
 12a:	0000                	unimp
            *Tx = bufferByte;
 12c:	2805                	jal	15c <main+0x14c>
 12e:	0000                	unimp
 130:	4400                	lw	s0,8(s0)
 132:	0001                	nop
            *AD_DISPLAY1 |= bufferByte;
 134:	0100                	addi	s0,sp,128
 136:	0069                	c.nop	26
 138:	037c                	addi	a5,sp,396
 13a:	9811                	andi	s0,s0,-28
 13c:	0001                	nop
 13e:	aa00                	fsd	fs0,16(a2)
 140:	0000                	unimp
 142:	0000                	unimp
        for(int i = 3; i>=0; i--){
 144:	3b05                	jal	fffffe74 <main+0xfffffe64>
 146:	0000                	unimp
 148:	5c00                	lw	s0,56(s0)
 14a:	0001                	nop
    while (Len > 0 && ADDMax >= addrSRAM){
 14c:	0100                	addi	s0,sp,128
 14e:	0069                	c.nop	26
 150:	0398                	addi	a4,sp,448
 152:	9811                	andi	s0,s0,-28
 154:	0001                	nop
 156:	b700                	fsd	fs0,40(a4)
        bufferWord = 0;
 158:	0000                	unimp
 15a:	0000                	unimp
 15c:	4e0e                	lw	t3,192(sp)
 15e:	0000                	unimp
 160:	0100                	addi	s0,sp,128
 162:	0069                	c.nop	26
        bufferWord=*addrSRAM;
 164:	03ad                	addi	t2,t2,11
 166:	9811                	andi	s0,s0,-28
            while ((*UART_Status & Tx_busy) == 1);
 168:	0001                	nop
 16a:	c400                	sw	s0,8(s0)
            *AD_DISPLAY1 |= bufferByte;
 16c:	0000                	unimp
 16e:	0000                	unimp
        for(int i = 3; i>=0; i--){
 170:	0200                	addi	s0,sp,256
 172:	0176                	slli	sp,sp,0x1d
    while (ADDMax > addrSRAM){
 174:	0000                	unimp
 176:	0409                	addi	s0,s0,2
 178:	00000007          	0x7
        addrSRAM = addrSRAM + 1;
 17c:	0a00                	addi	s0,sp,272
 17e:	0176                	slli	sp,sp,0x1d
    while (ADDMax > addrSRAM){
 180:	0000                	unimp
 182:	8702                	jr	a4
        bufferWord=0;
 184:	0001                	nop
 186:	0900                	addi	s0,sp,144
        bufferWord=*addrSRAM;
 188:	0801                	addi	a6,a6,0
 18a:	00000093          	li	ra,0
 18e:	7d02                	flw	fs10,32(sp)
 190:	0001                	nop
 192:	0200                	addi	s0,sp,256
            bufferByte = (bufferWord>>(i*8)) & 0xff;
 194:	019f 0000 040f      	0x40f0000019f
 19a:	6905                	lui	s2,0x1
 19c:	746e                	flw	fs0,248(sp)
 19e:	0a00                	addi	s0,sp,272
 1a0:	0198                	addi	a4,sp,192
 1a2:	0000                	unimp
	...

Disassembly of section .debug_abbrev:

00000000 <.debug_abbrev>:
   0:	3401                	jal	fffffa00 <main+0xfffff9f0>
   2:	0300                	addi	s0,sp,384
   4:	3a08                	fld	fa0,48(a2)
   6:	0121                	addi	sp,sp,8
   8:	0b39053b          	0xb39053b
   c:	1349                	addi	t1,t1,-14
   e:	1702                	slli	a4,a4,0x20
{
  10:	0000                	unimp
  12:	0f02                	c.slli64	t5
    volatile uint32_t receptUart = 0;
  14:	0b00                	addi	s0,sp,400
  16:	0421                	addi	s0,s0,8
    volatile uint32_t bufferWord = 0;
  18:	1349                	addi	t1,t1,-14
  1a:	0000                	unimp
    volatile uint32_t bufferByte = 0; 
  1c:	03003403          	0x3003403
    *AD_DISPLAY2=0xff83;     //BO
  20:	3a0e                	fld	fs4,224(sp)
  22:	0121                	addi	sp,sp,8
  24:	2139053b          	0x2139053b
  28:	02134917          	auipc	s2,0x2134
  2c:	0018                	0x18
  2e:	0400                	addi	s0,sp,512
    *AD_DISPLAY1=0xc0c087ff; //OT
  30:	0034                	addi	a3,sp,8
  32:	213a0e03          	lb	t3,531(s4)
  36:	3b01                	jal	fffffd46 <main+0xfffffd36>
  38:	3905                	jal	fffffc68 <main+0xfffffc58>
  3a:	1c13490b          	0x1c13490b
  3e:	000d                	c.nop	3
    uint32_t Len = 0x00000000;
  40:	0500                	addi	s0,sp,640
  42:	1755010b          	0x1755010b
        while ((*UART_Status & Rx_full) == 0);
  46:	1301                	addi	t1,t1,-32
    for(int i = 3; i >= 0; i--){
  48:	0000                	unimp
  4a:	3406                	fld	fs0,96(sp)
        while ((*UART_Status & Rx_full) == 0);
  4c:	0300                	addi	s0,sp,384
  4e:	3a0e                	fld	fs4,224(sp)
  50:	0121                	addi	sp,sp,8
  52:	0b39053b          	0xb39053b
  56:	1349                	addi	t1,t1,-14
        Lentmp = *Rx;
  58:	1702                	slli	a4,a4,0x20
  5a:	0000                	unimp
        while ((*UART_Status & Tx_busy) == 1);
  5c:	03003407          	fld	fs0,48(zero) # 30 <main+0x20>
  60:	3a0e                	fld	fs4,224(sp)
  62:	0121                	addi	sp,sp,8
  64:	0b39053b          	0xb39053b
        *Tx=Lentmp;
  68:	1349                	addi	t1,t1,-14
  6a:	211c                	fld	fa5,0(a0)
        Len |= Lentmp<<i*8;
  6c:	c080                	sw	s0,0(s1)
  6e:	0000                	unimp
  70:	0800                	addi	s0,sp,16
  72:	0034                	addi	a3,sp,8
    for(int i = 3; i >= 0; i--){
  74:	213a0803          	lb	a6,531(s4)
  78:	3b01                	jal	fffffd88 <main+0xfffffd78>
  7a:	3905                	jal	fffffcaa <main+0xfffffc9a>
    while (Len > 0 && ADDMax >= addrSRAM){
  7c:	1821                	addi	a6,a6,-24
  7e:	1349                	addi	t1,t1,-14
  80:	211c                	fld	fa5,0(a0)
  82:	8084                	0x8084
  84:	8080                	0x8080
  86:	007c                	addi	a5,sp,12
  88:	0900                	addi	s0,sp,144
  8a:	0024                	addi	s1,sp,8
        Len |= Lentmp<<i*8;
  8c:	0b3e0b0b          	0xb3e0b0b
            while (((*UART_Status & Rx_full) == 0) && ((*UART_Status & Rx_error) == 0));
  90:	00000e03          	lb	t3,0(zero) # 0 <_start>
            *AD_DISPLAY1=(receptUart<<(16));
  94:	350a                	fld	fa0,160(sp)
  96:	4900                	lw	s0,16(a0)
        for(int i = 3; i >= 0; i--){
  98:	0b000013          	li	zero,176
  9c:	0111                	addi	sp,sp,4
  9e:	0e25                	addi	t3,t3,9
                tmp=*UART_Status;
  a0:	1f030b13          	addi	s6,t1,496
                if((tmp & Rx_full) != 0) break;
  a4:	01111f1b          	0x1111f1b
  a8:	0612                	slli	a2,a2,0x4
  aa:	1710                	addi	a2,sp,928
            receptUart=*Rx;
  ac:	0000                	unimp
  ae:	2e0c                	fld	fa1,24(a2)
  b0:	3f01                	jal	ffffffc0 <main+0xffffffb0>
  b2:	0319                	addi	t1,t1,6
            *AD_DISPLAY1=(receptUart<<(16));
  b4:	3a0e                	fld	fs4,224(sp)
  b6:	39053b0b          	0x39053b0b
  ba:	1119270b          	0x1119270b
  be:	1201                	addi	tp,tp,-32
            bufferWord |= (receptUart<<(i*8));
  c0:	4006                	0x4006
  c2:	7a18                	flw	fa4,48(a2)
  c4:	0119                	addi	sp,sp,6
  c6:	0d000013          	li	zero,208
  ca:	0034                	addi	a3,sp,8
  cc:	0b3a0e03          	lb	t3,179(s4)
  d0:	0b39053b          	0xb39053b
        for(int i = 3; i >= 0; i--){
  d4:	1349                	addi	t1,t1,-14
  d6:	0b1c                	addi	a5,sp,400
  d8:	0000                	unimp
  da:	0b0e                	slli	s6,s6,0x3
            while (((*UART_Status & Rx_full) == 0) && ((*UART_Status & Rx_error) == 0));
  dc:	5501                	li	a0,-32
  de:	0f000017          	auipc	zero,0xf000
  e2:	0024                	addi	s1,sp,8
  e4:	0b3e0b0b          	0xb3e0b0b
  e8:	00000803          	lb	a6,0(zero) # 0 <_start>
	...

Disassembly of section .debug_loclists:

00000000 <.debug_loclists>:
   0:	00cd                	addi	ra,ra,19
   2:	0000                	unimp
   4:	0005                	c.nop	1
   6:	0004                	0x4
   8:	0000                	unimp
   a:	0000                	unimp
   c:	00001407          	0x1407
{
  10:	a000                	fsd	fs0,0(s0)
  12:	0000                	unimp
    volatile uint32_t receptUart = 0;
  14:	0200                	addi	s0,sp,256
  16:	9f30                	0x9f30
    volatile uint32_t bufferWord = 0;
  18:	0000a007          	flw	ft0,0(ra)
    volatile uint32_t bufferByte = 0; 
  1c:	4c00                	lw	s0,24(s0)
  1e:	0001                	nop
    *AD_DISPLAY2=0xff83;     //BO
  20:	0100                	addi	s0,sp,128
  22:	076c                	addi	a1,sp,908
  24:	014c                	addi	a1,sp,132
  26:	0000                	unimp
  28:	0154                	addi	a3,sp,132
  2a:	0000                	unimp
  2c:	7a06                	flw	fs4,96(sp)
  2e:	8100                	0x8100
    *AD_DISPLAY1=0xc0c087ff; //OT
  30:	1c00                	addi	s0,sp,560
  32:	079f 0158 0000      	0x158079f
  38:	0164                	addi	s1,sp,140
  3a:	0000                	unimp
  3c:	6c01                	0x6c01
  3e:	00016407          	0x16407
    uint32_t Len = 0x00000000;
  42:	7c00                	flw	fs0,56(s0)
        while ((*UART_Status & Rx_full) == 0);
  44:	0001                	nop
  46:	0200                	addi	s0,sp,256
    for(int i = 3; i >= 0; i--){
  48:	9f30                	0x9f30
  4a:	00017c07          	0x17c07
        while ((*UART_Status & Rx_full) == 0);
  4e:	d800                	sw	s0,48(s0)
  50:	0001                	nop
  52:	0100                	addi	s0,sp,128
  54:	0060                	addi	s0,sp,12
  56:	00005c07          	0x5c07
        Lentmp = *Rx;
  5a:	7000                	flw	fs0,32(s0)
        while ((*UART_Status & Tx_busy) == 1);
  5c:	0000                	unimp
  5e:	0100                	addi	s0,sp,128
  60:	075c                	addi	a5,sp,900
  62:	0070                	addi	a2,sp,12
  64:	0000                	unimp
  66:	00a0                	addi	s0,sp,72
        *Tx=Lentmp;
  68:	0000                	unimp
  6a:	7e02                	flw	ft8,32(sp)
        Len |= Lentmp<<i*8;
  6c:	0004                	0x4
  6e:	00001407          	0x1407
  72:	4c00                	lw	s0,24(s0)
    for(int i = 3; i >= 0; i--){
  74:	0000                	unimp
  76:	0200                	addi	s0,sp,256
  78:	9f30                	0x9f30
  7a:	00007407          	0x7407
    while (Len > 0 && ADDMax >= addrSRAM){
  7e:	a000                	fsd	fs0,0(s0)
  80:	0000                	unimp
  82:	0100                	addi	s0,sp,128
  84:	075a                	slli	a4,a4,0x16
  86:	014c                	addi	a1,sp,132
  88:	0000                	unimp
  8a:	0164                	addi	s1,sp,140
        Len |= Lentmp<<i*8;
  8c:	0000                	unimp
  8e:	6101                	addi	sp,sp,0
            while (((*UART_Status & Rx_full) == 0) && ((*UART_Status & Rx_error) == 0));
  90:	0700                	addi	s0,sp,896
  92:	00a4                	addi	s1,sp,72
            *AD_DISPLAY1=(receptUart<<(16));
  94:	0000                	unimp
  96:	00a8                	addi	a0,sp,72
        for(int i = 3; i >= 0; i--){
  98:	0000                	unimp
  9a:	5e01                	li	t3,-32
  9c:	0700                	addi	s0,sp,896
  9e:	003c                	addi	a5,sp,8
                tmp=*UART_Status;
  a0:	0000                	unimp
  a2:	004c                	addi	a1,sp,4
                if((tmp & Rx_full) != 0) break;
  a4:	0000                	unimp
  a6:	3302                	fld	ft6,32(sp)
  a8:	009f 5c07 0001      	0x15c07009f
            receptUart=*Rx;
  ae:	6400                	flw	fs0,8(s0)
  b0:	0001                	nop
  b2:	0200                	addi	s0,sp,256
            *AD_DISPLAY1=(receptUart<<(16));
  b4:	07009f33          	0x7009f33
  b8:	010c                	addi	a1,sp,128
  ba:	0000                	unimp
  bc:	0110                	addi	a2,sp,128
  be:	0000                	unimp
            bufferWord |= (receptUart<<(i*8));
  c0:	3302                	fld	ft6,32(sp)
  c2:	009f 9007 0001      	0x19007009f
  c8:	9400                	0x9400
  ca:	0001                	nop
  cc:	0200                	addi	s0,sp,256
  ce:	          	sll	t5,ra,zero

Disassembly of section .debug_aranges:

00000000 <.debug_aranges>:
   0:	001c                	0x1c
   2:	0000                	unimp
   4:	0002                	c.slli64	zero
   6:	0000                	unimp
   8:	0000                	unimp
   a:	0004                	0x4
   c:	0000                	unimp
   e:	0000                	unimp
{
  10:	0010                	0x10
  12:	0000                	unimp
    volatile uint32_t receptUart = 0;
  14:	01c8                	addi	a0,sp,196
	...

Disassembly of section .debug_rnglists:

00000000 <.debug_rnglists>:
   0:	0066                	c.slli	zero,0x19
   2:	0000                	unimp
   4:	0005                	c.nop	1
   6:	0004                	0x4
   8:	0000                	unimp
   a:	0000                	unimp
   c:	3c06                	fld	fs8,96(sp)
   e:	0000                	unimp
{
  10:	3c00                	fld	fs0,56(s0)
  12:	0000                	unimp
    volatile uint32_t receptUart = 0;
  14:	0600                	addi	s0,sp,768
  16:	0044                	addi	s1,sp,4
    volatile uint32_t bufferWord = 0;
  18:	0000                	unimp
  1a:	007c                	addi	a5,sp,12
    volatile uint32_t bufferByte = 0; 
  1c:	0000                	unimp
  1e:	8c06                	mv	s8,ra
    *AD_DISPLAY2=0xff83;     //BO
  20:	0000                	unimp
  22:	9000                	0x9000
  24:	0000                	unimp
  26:	0000                	unimp
  28:	9006                	c.add	zero,ra
  2a:	0000                	unimp
  2c:	fc00                	fsw	fs0,56(s0)
  2e:	0000                	unimp
    *AD_DISPLAY1=0xc0c087ff; //OT
  30:	0600                	addi	s0,sp,768
  32:	015c                	addi	a5,sp,132
  34:	0000                	unimp
  36:	015c                	addi	a5,sp,132
  38:	0000                	unimp
  3a:	0600                	addi	s0,sp,768
  3c:	010c                	addi	a1,sp,128
  3e:	0000                	unimp
    uint32_t Len = 0x00000000;
  40:	010c                	addi	a1,sp,128
  42:	0000                	unimp
        while ((*UART_Status & Rx_full) == 0);
  44:	1006                	c.slli	zero,0x21
  46:	0001                	nop
    for(int i = 3; i >= 0; i--){
  48:	4c00                	lw	s0,24(s0)
  4a:	0001                	nop
        while ((*UART_Status & Rx_full) == 0);
  4c:	0000                	unimp
  4e:	6806                	flw	fa6,64(sp)
  50:	0001                	nop
  52:	7400                	flw	fs0,40(s0)
  54:	0001                	nop
  56:	0600                	addi	s0,sp,768
        Lentmp = *Rx;
  58:	0190                	addi	a2,sp,192
  5a:	0000                	unimp
        while ((*UART_Status & Tx_busy) == 1);
  5c:	0190                	addi	a2,sp,192
  5e:	0000                	unimp
  60:	9406                	add	s0,s0,ra
  62:	0001                	nop
  64:	d400                	sw	s0,40(s0)
  66:	0001                	nop
	...

Disassembly of section .debug_line:

00000000 <.debug_line>:
   0:	060d                	addi	a2,a2,3
   2:	0000                	unimp
   4:	0005                	c.nop	1
   6:	0004                	0x4
   8:	002e                	c.slli	zero,0xb
   a:	0000                	unimp
   c:	0101                	addi	sp,sp,0
   e:	fb01                	bnez	a4,ffffff1e <main+0xffffff0e>
{
  10:	0d0e                	slli	s10,s10,0x3
  12:	0100                	addi	s0,sp,128
    volatile uint32_t receptUart = 0;
  14:	0101                	addi	sp,sp,0
  16:	0001                	nop
    volatile uint32_t bufferWord = 0;
  18:	0000                	unimp
  1a:	0001                	nop
    volatile uint32_t bufferByte = 0; 
  1c:	0100                	addi	s0,sp,128
  1e:	0101                	addi	sp,sp,0
    *AD_DISPLAY2=0xff83;     //BO
  20:	021f 0000 0000      	0x21f
  26:	0068                	addi	a0,sp,12
  28:	0000                	unimp
  2a:	0102                	c.slli64	sp
  2c:	021f 020f 005b      	0x5b020f021f
    *AD_DISPLAY1=0xc0c087ff; //OT
  32:	0000                	unimp
  34:	6100                	flw	fs0,0(a0)
  36:	0000                	unimp
  38:	0100                	addi	s0,sp,128
  3a:	0105                	addi	sp,sp,1
  3c:	0500                	addi	s0,sp,640
  3e:	1002                	c.slli	zero,0x20
    uint32_t Len = 0x00000000;
  40:	0000                	unimp
  42:	0300                	addi	s0,sp,384
        while ((*UART_Status & Rx_full) == 0);
  44:	06d1                	addi	a3,a3,20
  46:	0501                	addi	a0,a0,0
    for(int i = 3; i >= 0; i--){
  48:	0305                	addi	t1,t1,1
  4a:	00040903          	lb	s2,0(s0) # fffe0000 <main+0xfffdfff0>
        while ((*UART_Status & Rx_full) == 0);
  4e:	0301                	addi	t1,t1,0
  50:	0901                	addi	s2,s2,0
  52:	0000                	unimp
  54:	0301                	addi	t1,t1,0
  56:	0901                	addi	s2,s2,0
        Lentmp = *Rx;
  58:	0000                	unimp
  5a:	0301                	addi	t1,t1,0
        while ((*UART_Status & Tx_busy) == 1);
  5c:	0901                	addi	s2,s2,0
  5e:	0000                	unimp
  60:	0301                	addi	t1,t1,0
  62:	0901                	addi	s2,s2,0
  64:	0000                	unimp
  66:	0301                	addi	t1,t1,0
        *Tx=Lentmp;
  68:	0901                	addi	s2,s2,0
  6a:	0000                	unimp
        Len |= Lentmp<<i*8;
  6c:	0301                	addi	t1,t1,0
  6e:	0901                	addi	s2,s2,0
  70:	0000                	unimp
  72:	0501                	addi	a0,a0,0
    for(int i = 3; i >= 0; i--){
  74:	00030617          	auipc	a2,0x30
  78:	0009                	c.nop	2
  7a:	0100                	addi	s0,sp,128
    while (Len > 0 && ADDMax >= addrSRAM){
  7c:	0505                	addi	a0,a0,1
  7e:	0306                	slli	t1,t1,0x1
  80:	0901                	addi	s2,s2,0
  82:	0004                	0x4
  84:	0501                	addi	a0,a0,0
  86:	00030617          	auipc	a2,0x30
  8a:	0009                	c.nop	2
        Len |= Lentmp<<i*8;
  8c:	0100                	addi	s0,sp,128
  8e:	0505                	addi	a0,a0,1
            while (((*UART_Status & Rx_full) == 0) && ((*UART_Status & Rx_error) == 0));
  90:	0306                	slli	t1,t1,0x1
  92:	0901                	addi	s2,s2,0
            *AD_DISPLAY1=(receptUart<<(16));
  94:	0004                	0x4
  96:	0501                	addi	a0,a0,0
        for(int i = 3; i >= 0; i--){
  98:	00030617          	auipc	a2,0x30
  9c:	0009                	c.nop	2
  9e:	0100                	addi	s0,sp,128
                tmp=*UART_Status;
  a0:	0505                	addi	a0,a0,1
  a2:	0306                	slli	t1,t1,0x1
                if((tmp & Rx_full) != 0) break;
  a4:	0901                	addi	s2,s2,0
  a6:	0004                	0x4
  a8:	0301                	addi	t1,t1,0
  aa:	0901                	addi	s2,s2,0
            receptUart=*Rx;
  ac:	0000                	unimp
  ae:	0301                	addi	t1,t1,0
  b0:	0901                	addi	s2,s2,0
  b2:	0000                	unimp
            *AD_DISPLAY1=(receptUart<<(16));
  b4:	0301                	addi	t1,t1,0
  b6:	0902                	c.slli64	s2
  b8:	0000                	unimp
  ba:	0301                	addi	t1,t1,0
  bc:	0901                	addi	s2,s2,0
  be:	0000                	unimp
            bufferWord |= (receptUart<<(i*8));
  c0:	0301                	addi	t1,t1,0
  c2:	0901                	addi	s2,s2,0
  c4:	0000                	unimp
  c6:	0301                	addi	t1,t1,0
  c8:	0901                	addi	s2,s2,0
  ca:	0000                	unimp
  cc:	0501                	addi	a0,a0,0
  ce:	0611                	addi	a2,a2,4
  d0:	00090003          	lb	zero,0(s2) # 2134028 <main+0x2134018>
        for(int i = 3; i >= 0; i--){
  d4:	0100                	addi	s0,sp,128
  d6:	0505                	addi	a0,a0,1
  d8:	0306                	slli	t1,t1,0x1
  da:	0901                	addi	s2,s2,0
            while (((*UART_Status & Rx_full) == 0) && ((*UART_Status & Rx_error) == 0));
  dc:	0010                	0x10
  de:	0501                	addi	a0,a0,0
  e0:	0611                	addi	a2,a2,4
  e2:	00090003          	lb	zero,0(s2)
  e6:	0100                	addi	s0,sp,128
  e8:	0505                	addi	a0,a0,1
  ea:	0306                	slli	t1,t1,0x1
  ec:	090a                	slli	s2,s2,0x2
  ee:	000c                	0xc
  f0:	0501                	addi	a0,a0,0
  f2:	0309                	addi	t1,t1,2
  f4:	0900                	addi	s0,sp,144
  f6:	0000                	unimp
            Len = Len - 1;
  f8:	0501                	addi	a0,a0,0
  fa:	0316                	slli	t1,t1,0x5
        *addrSRAM=bufferWord;
  fc:	0900                	addi	s0,sp,144
  fe:	0000                	unimp
 100:	0501                	addi	a0,a0,0
 102:	0611                	addi	a2,a2,4
        bufferWord=0;
 104:	00097603          	0x97603
        bufferWord=*addrSRAM;
 108:	0100                	addi	s0,sp,128
 10a:	0e05                	addi	t3,t3,1
 10c:	04097403          	0x4097403
            bufferByte = (bufferWord>>(i*8)) & 0xff;
 110:	0100                	addi	s0,sp,128
 112:	1105                	addi	sp,sp,-31
 114:	04091703          	lh	a4,64(s2)
 118:	0100                	addi	s0,sp,128
 11a:	1605                	addi	a2,a2,-31
 11c:	04097f03          	0x4097f03
            while ((*UART_Status & Tx_busy) == 1);
 120:	0100                	addi	s0,sp,128
 122:	2905                	jal	552 <main+0x542>
 124:	0200                	addi	s0,sp,256
 126:	0104                	addi	s1,sp,128
 128:	0306                	slli	t1,t1,0x1
 12a:	0901                	addi	s2,s2,0
            *Tx = bufferByte;
 12c:	0004                	0x4
 12e:	0501                	addi	a0,a0,0
 130:	0011                	c.nop	4
 132:	0402                	c.slli64	s0
            *AD_DISPLAY1 |= bufferByte;
 134:	0601                	addi	a2,a2,0
 136:	00090003          	lb	zero,0(s2)
 13a:	0100                	addi	s0,sp,128
 13c:	1e05                	addi	t3,t3,-31
 13e:	0200                	addi	s0,sp,256
 140:	0104                	addi	s1,sp,128
 142:	04090003          	lb	zero,64(s2)
        for(int i = 3; i>=0; i--){
 146:	0100                	addi	s0,sp,128
 148:	2905                	jal	578 <main+0x568>
 14a:	0200                	addi	s0,sp,256
    while (Len > 0 && ADDMax >= addrSRAM){
 14c:	0104                	addi	s1,sp,128
 14e:	04090003          	lb	zero,64(s2)
 152:	0100                	addi	s0,sp,128
 154:	0905                	addi	s2,s2,1
 156:	0306                	slli	t1,t1,0x1
        bufferWord = 0;
 158:	0901                	addi	s2,s2,0
 15a:	0004                	0x4
 15c:	0501                	addi	a0,a0,0
 15e:	0610                	addi	a2,sp,768
 160:	00090003          	lb	zero,0(s2)
        bufferWord=*addrSRAM;
 164:	0100                	addi	s0,sp,128
 166:	0905                	addi	s2,s2,1
            while ((*UART_Status & Tx_busy) == 1);
 168:	0306                	slli	t1,t1,0x1
 16a:	0901                	addi	s2,s2,0
            *AD_DISPLAY1 |= bufferByte;
 16c:	0004                	0x4
 16e:	0501                	addi	a0,a0,0
        for(int i = 3; i>=0; i--){
 170:	0029                	c.nop	10
 172:	0402                	c.slli64	s0
    while (ADDMax > addrSRAM){
 174:	0301                	addi	t1,t1,0
 176:	0900                	addi	s0,sp,144
 178:	0000                	unimp
 17a:	0501                	addi	a0,a0,0
        addrSRAM = addrSRAM + 1;
 17c:	0011                	c.nop	4
 17e:	0402                	c.slli64	s0
    while (ADDMax > addrSRAM){
 180:	0601                	addi	a2,a2,0
 182:	00090003          	lb	zero,0(s2)
        bufferWord=0;
 186:	0100                	addi	s0,sp,128
        bufferWord=*addrSRAM;
 188:	1e05                	addi	t3,t3,-31
 18a:	0200                	addi	s0,sp,256
 18c:	0104                	addi	s1,sp,128
 18e:	04090003          	lb	zero,64(s2)
 192:	0100                	addi	s0,sp,128
            bufferByte = (bufferWord>>(i*8)) & 0xff;
 194:	2905                	jal	5c4 <main+0x5b4>
 196:	0200                	addi	s0,sp,256
 198:	0104                	addi	s1,sp,128
 19a:	04090003          	lb	zero,64(s2)
 19e:	0100                	addi	s0,sp,128
 1a0:	0905                	addi	s2,s2,1
 1a2:	0200                	addi	s0,sp,256
            while ((*UART_Status & Tx_busy) == 1);
 1a4:	0204                	addi	s1,sp,256
 1a6:	0306                	slli	t1,t1,0x1
 1a8:	0901                	addi	s2,s2,0
 1aa:	0004                	0x4
 1ac:	0501                	addi	a0,a0,0
 1ae:	000c                	0xc
            *Tx = bufferByte;
 1b0:	0402                	c.slli64	s0
 1b2:	0602                	c.slli64	a2
 1b4:	00090003          	lb	zero,0(s2)
            *AD_DISPLAY1 |= bufferByte;
 1b8:	0100                	addi	s0,sp,128
 1ba:	0905                	addi	s2,s2,1
 1bc:	0200                	addi	s0,sp,256
 1be:	0204                	addi	s1,sp,256
 1c0:	0306                	slli	t1,t1,0x1
 1c2:	0901                	addi	s2,s2,0
 1c4:	0004                	0x4
 1c6:	0501                	addi	a0,a0,0
        for(int i = 3; i>=0; i--){
 1c8:	0016                	c.slli	zero,0x5
 1ca:	0402                	c.slli64	s0
 1cc:	0602                	c.slli64	a2
 1ce:	00090003          	lb	zero,0(s2)
 1d2:	0100                	addi	s0,sp,128
    while(1);
 1d4:	0d05                	addi	s10,s10,1
 1d6:	0200                	addi	s0,sp,256
 1d8:	0204                	addi	s1,sp,256
 1da:	04090003          	lb	zero,64(s2)
 1de:	0100                	addi	s0,sp,128
 1e0:	1d05                	addi	s10,s10,-31
 1e2:	0200                	addi	s0,sp,256
 1e4:	0204                	addi	s1,sp,256
 1e6:	0306                	slli	t1,t1,0x1
 1e8:	0004097b          	0x4097b
 1ec:	0501                	addi	a0,a0,0
 1ee:	0016                	c.slli	zero,0x5
 1f0:	0402                	c.slli64	s0
 1f2:	0302                	c.slli64	t1
 1f4:	0900                	addi	s0,sp,144
 1f6:	0000                	unimp
 1f8:	0501                	addi	a0,a0,0
 1fa:	0314                	addi	a3,sp,384
 1fc:	090a                	slli	s2,s2,0x2
 1fe:	0008                	0x8
 200:	0501                	addi	a0,a0,0
 202:	060d                	addi	a2,a2,3
 204:	10097b03          	0x10097b03
 208:	0100                	addi	s0,sp,128
 20a:	1605                	addi	a2,a2,-31
 20c:	04090803          	lb	a6,64(s2)
 210:	0100                	addi	s0,sp,128
 212:	1905                	addi	s2,s2,-31
 214:	04090a03          	lb	s4,64(s2)
 218:	0100                	addi	s0,sp,128
 21a:	1a05                	addi	s4,s4,-31
 21c:	04097503          	0x4097503
 220:	0100                	addi	s0,sp,128
 222:	0d05                	addi	s10,s10,1
 224:	0306                	slli	t1,t1,0x1
 226:	0902                	c.slli64	s2
 228:	0008                	0x8
 22a:	0501                	addi	a0,a0,0
 22c:	0311                	addi	t1,t1,4
 22e:	0901                	addi	s2,s2,0
 230:	0000                	unimp
 232:	0501                	addi	a0,a0,0
 234:	0615                	addi	a2,a2,5
 236:	00090003          	lb	zero,0(s2)
 23a:	0100                	addi	s0,sp,128
 23c:	1105                	addi	sp,sp,-31
 23e:	0306                	slli	t1,t1,0x1
 240:	0901                	addi	s2,s2,0
 242:	0004                	0x4
 244:	0501                	addi	a0,a0,0
 246:	0619                	addi	a2,a2,6
 248:	00090003          	lb	zero,0(s2)
 24c:	0100                	addi	s0,sp,128
 24e:	1305                	addi	t1,t1,-31
 250:	04090003          	lb	zero,64(s2)
 254:	0100                	addi	s0,sp,128
 256:	0d05                	addi	s10,s10,1
 258:	0200                	addi	s0,sp,256
 25a:	0204                	addi	s1,sp,256
 25c:	0306                	slli	t1,t1,0x1
 25e:	0906                	slli	s2,s2,0x1
 260:	0004                	0x4
 262:	0501                	addi	a0,a0,0
 264:	0018                	0x18
 266:	0402                	c.slli64	s0
 268:	0602                	c.slli64	a2
 26a:	00090003          	lb	zero,0(s2)
 26e:	0100                	addi	s0,sp,128
 270:	1705                	addi	a4,a4,-31
 272:	0200                	addi	s0,sp,256
 274:	0204                	addi	s1,sp,256
 276:	04090003          	lb	zero,64(s2)
 27a:	0100                	addi	s0,sp,128
 27c:	0d05                	addi	s10,s10,1
 27e:	0200                	addi	s0,sp,256
 280:	0204                	addi	s1,sp,256
 282:	0306                	slli	t1,t1,0x1
 284:	0901                	addi	s2,s2,0
 286:	0004                	0x4
 288:	0501                	addi	a0,a0,0
 28a:	0025                	c.nop	9
 28c:	0402                	c.slli64	s0
 28e:	0602                	c.slli64	a2
 290:	00090003          	lb	zero,0(s2)
 294:	0100                	addi	s0,sp,128
 296:	1905                	addi	s2,s2,-31
 298:	0200                	addi	s0,sp,256
 29a:	0204                	addi	s1,sp,256
 29c:	08090003          	lb	zero,128(s2)
 2a0:	0100                	addi	s0,sp,128
 2a2:	0d05                	addi	s10,s10,1
 2a4:	0200                	addi	s0,sp,256
 2a6:	0204                	addi	s1,sp,256
 2a8:	0306                	slli	t1,t1,0x1
 2aa:	0901                	addi	s2,s2,0
 2ac:	0004                	0x4
 2ae:	0501                	addi	a0,a0,0
 2b0:	0026                	c.slli	zero,0x9
 2b2:	0402                	c.slli64	s0
 2b4:	0602                	c.slli64	a2
 2b6:	00090003          	lb	zero,0(s2)
 2ba:	0100                	addi	s0,sp,128
 2bc:	1805                	addi	a6,a6,-31
 2be:	0200                	addi	s0,sp,256
 2c0:	0204                	addi	s1,sp,256
 2c2:	04090003          	lb	zero,64(s2)
 2c6:	0100                	addi	s0,sp,128
 2c8:	2605                	jal	5e8 <main+0x5d8>
 2ca:	0200                	addi	s0,sp,256
 2cc:	0204                	addi	s1,sp,256
 2ce:	04090003          	lb	zero,64(s2)
 2d2:	0100                	addi	s0,sp,128
 2d4:	1805                	addi	a6,a6,-31
 2d6:	0200                	addi	s0,sp,256
 2d8:	0204                	addi	s1,sp,256
 2da:	04090003          	lb	zero,64(s2)
 2de:	0100                	addi	s0,sp,128
 2e0:	0d05                	addi	s10,s10,1
 2e2:	0200                	addi	s0,sp,256
 2e4:	0204                	addi	s1,sp,256
 2e6:	0306                	slli	t1,t1,0x1
 2e8:	0908                	addi	a0,sp,144
 2ea:	0008                	0x8
 2ec:	0501                	addi	a0,a0,0
 2ee:	0021                	c.nop	8
 2f0:	0402                	c.slli64	s0
 2f2:	0302                	c.slli64	t1
 2f4:	096c                	addi	a1,sp,156
 2f6:	0000                	unimp
 2f8:	0501                	addi	a0,a0,0
 2fa:	001a                	c.slli	zero,0x6
 2fc:	0402                	c.slli64	s0
 2fe:	0302                	c.slli64	t1
 300:	0900                	addi	s0,sp,144
 302:	0000                	unimp
 304:	0501                	addi	a0,a0,0
 306:	0034                	addi	a3,sp,8
 308:	0402                	c.slli64	s0
 30a:	0302                	c.slli64	t1
 30c:	0901                	addi	s2,s2,0
 30e:	0008                	0x8
 310:	0501                	addi	a0,a0,0
 312:	0016                	c.slli	zero,0x5
 314:	0402                	c.slli64	s0
 316:	0602                	c.slli64	a2
 318:	00090003          	lb	zero,0(s2)
 31c:	0100                	addi	s0,sp,128
 31e:	2305                	jal	83e <main+0x82e>
 320:	0200                	addi	s0,sp,256
 322:	0204                	addi	s1,sp,256
 324:	04090003          	lb	zero,64(s2)
 328:	0100                	addi	s0,sp,128
 32a:	3405                	jal	fffffd4a <main+0xfffffd3a>
 32c:	0200                	addi	s0,sp,256
 32e:	0204                	addi	s1,sp,256
 330:	04090003          	lb	zero,64(s2)
 334:	0100                	addi	s0,sp,128
 336:	3905                	jal	ffffff66 <main+0xffffff56>
 338:	0200                	addi	s0,sp,256
 33a:	0104                	addi	s1,sp,128
 33c:	04090003          	lb	zero,64(s2)
 340:	0100                	addi	s0,sp,128
 342:	4605                	li	a2,1
 344:	0200                	addi	s0,sp,256
 346:	0104                	addi	s1,sp,128
 348:	04090003          	lb	zero,64(s2)
 34c:	0100                	addi	s0,sp,128
 34e:	3405                	jal	fffffd6e <main+0xfffffd5e>
 350:	0200                	addi	s0,sp,256
 352:	0104                	addi	s1,sp,128
 354:	04090003          	lb	zero,64(s2)
 358:	0100                	addi	s0,sp,128
 35a:	1105                	addi	sp,sp,-31
 35c:	08091303          	lh	t1,128(s2)
 360:	0100                	addi	s0,sp,128
 362:	0905                	addi	s2,s2,1
 364:	0306                	slli	t1,t1,0x1
 366:	0905                	addi	s2,s2,1
 368:	0004                	0x4
 36a:	0501                	addi	a0,a0,0
 36c:	0612                	slli	a2,a2,0x4
 36e:	00090003          	lb	zero,0(s2)
 372:	0100                	addi	s0,sp,128
 374:	0905                	addi	s2,s2,1
 376:	0306                	slli	t1,t1,0x1
 378:	0901                	addi	s2,s2,0
 37a:	0008                	0x8
 37c:	0501                	addi	a0,a0,0
 37e:	00030613          	mv	a2,t1
 382:	0009                	c.nop	2
 384:	0100                	addi	s0,sp,128
 386:	0905                	addi	s2,s2,1
 388:	0306                	slli	t1,t1,0x1
 38a:	0901                	addi	s2,s2,0
 38c:	0004                	0x4
 38e:	0501                	addi	a0,a0,0
 390:	00030613          	mv	a2,t1
 394:	0009                	c.nop	2
 396:	0100                	addi	s0,sp,128
 398:	0905                	addi	s2,s2,1
 39a:	0306                	slli	t1,t1,0x1
 39c:	0901                	addi	s2,s2,0
 39e:	0004                	0x4
 3a0:	0501                	addi	a0,a0,0
 3a2:	030d                	addi	t1,t1,3
 3a4:	0900                	addi	s0,sp,144
 3a6:	0000                	unimp
 3a8:	0501                	addi	a0,a0,0
 3aa:	0319                	addi	t1,t1,6
 3ac:	0900                	addi	s0,sp,144
 3ae:	0000                	unimp
 3b0:	0501                	addi	a0,a0,0
 3b2:	7f030613          	addi	a2,t1,2032
 3b6:	0009                	c.nop	2
 3b8:	0100                	addi	s0,sp,128
 3ba:	0d05                	addi	s10,s10,1
 3bc:	0306                	slli	t1,t1,0x1
 3be:	0902                	c.slli64	s2
 3c0:	0004                	0x4
 3c2:	0501                	addi	a0,a0,0
 3c4:	0625                	addi	a2,a2,9
 3c6:	00090003          	lb	zero,0(s2)
 3ca:	0100                	addi	s0,sp,128
 3cc:	2e05                	jal	6fc <main+0x6ec>
 3ce:	08090003          	lb	zero,128(s2)
 3d2:	0100                	addi	s0,sp,128
 3d4:	1805                	addi	a6,a6,-31
 3d6:	04090003          	lb	zero,64(s2)
 3da:	0100                	addi	s0,sp,128
 3dc:	0d05                	addi	s10,s10,1
 3de:	0306                	slli	t1,t1,0x1
 3e0:	0901                	addi	s2,s2,0
 3e2:	0004                	0x4
 3e4:	0501                	addi	a0,a0,0
 3e6:	002d                	c.nop	11
 3e8:	0402                	c.slli64	s0
 3ea:	0301                	addi	t1,t1,0
 3ec:	0900                	addi	s0,sp,144
 3ee:	0000                	unimp
 3f0:	0501                	addi	a0,a0,0
 3f2:	0015                	c.nop	5
 3f4:	0402                	c.slli64	s0
 3f6:	0601                	addi	a2,a2,0
 3f8:	00090003          	lb	zero,0(s2)
 3fc:	0100                	addi	s0,sp,128
 3fe:	2205                	jal	51e <main+0x50e>
 400:	0200                	addi	s0,sp,256
 402:	0104                	addi	s1,sp,128
 404:	04090003          	lb	zero,64(s2)
 408:	0100                	addi	s0,sp,128
 40a:	2d05                	jal	a3a <main+0xa2a>
 40c:	0200                	addi	s0,sp,256
 40e:	0104                	addi	s1,sp,128
 410:	04090003          	lb	zero,64(s2)
 414:	0100                	addi	s0,sp,128
 416:	0d05                	addi	s10,s10,1
 418:	0200                	addi	s0,sp,256
 41a:	0204                	addi	s1,sp,256
 41c:	0306                	slli	t1,t1,0x1
 41e:	0901                	addi	s2,s2,0
 420:	0004                	0x4
 422:	0501                	addi	a0,a0,0
 424:	0011                	c.nop	4
 426:	0402                	c.slli64	s0
 428:	0602                	c.slli64	a2
 42a:	00090003          	lb	zero,0(s2)
 42e:	0100                	addi	s0,sp,128
 430:	0d05                	addi	s10,s10,1
 432:	0200                	addi	s0,sp,256
 434:	0204                	addi	s1,sp,256
 436:	0306                	slli	t1,t1,0x1
 438:	0901                	addi	s2,s2,0
 43a:	0008                	0x8
 43c:	0501                	addi	a0,a0,0
 43e:	001a                	c.slli	zero,0x6
 440:	0402                	c.slli64	s0
 442:	0602                	c.slli64	a2
 444:	04090003          	lb	zero,64(s2)
 448:	0100                	addi	s0,sp,128
 44a:	1f05                	addi	t5,t5,-31
 44c:	0200                	addi	s0,sp,256
 44e:	0204                	addi	s1,sp,256
 450:	0306                	slli	t1,t1,0x1
 452:	097c                	addi	a5,sp,156
 454:	000c                	0xc
 456:	0501                	addi	a0,a0,0
 458:	0019                	c.nop	6
 45a:	0402                	c.slli64	s0
 45c:	0302                	c.slli64	t1
 45e:	0900                	addi	s0,sp,144
 460:	0000                	unimp
 462:	0501                	addi	a0,a0,0
 464:	0309                	addi	t1,t1,2
 466:	00080907          	0x80907
 46a:	0501                	addi	a0,a0,0
 46c:	0314                	addi	a3,sp,384
 46e:	0000095b          	0x95b
 472:	0001                	nop
 474:	0402                	c.slli64	s0
 476:	0601                	addi	a2,a2,0
 478:	04090003          	lb	zero,64(s2)
 47c:	0100                	addi	s0,sp,128
 47e:	0905                	addi	s2,s2,1
 480:	0306                	slli	t1,t1,0x1
 482:	0901                	addi	s2,s2,0
 484:	0008                	0x8
 486:	0501                	addi	a0,a0,0
 488:	0614                	addi	a3,sp,768
 48a:	00090003          	lb	zero,0(s2)
 48e:	0100                	addi	s0,sp,128
 490:	0905                	addi	s2,s2,1
 492:	0306                	slli	t1,t1,0x1
 494:	0901                	addi	s2,s2,0
 496:	0004                	0x4
 498:	0501                	addi	a0,a0,0
 49a:	030d                	addi	t1,t1,3
 49c:	0900                	addi	s0,sp,144
 49e:	0000                	unimp
 4a0:	0501                	addi	a0,a0,0
 4a2:	031a                	slli	t1,t1,0x6
 4a4:	0900                	addi	s0,sp,144
 4a6:	0000                	unimp
 4a8:	0501                	addi	a0,a0,0
 4aa:	0614                	addi	a3,sp,768
 4ac:	00097f03          	0x97f03
 4b0:	0100                	addi	s0,sp,128
 4b2:	1305                	addi	t1,t1,-31
 4b4:	0306                	slli	t1,t1,0x1
 4b6:	0008092f          	0x8092f
 4ba:	0601                	addi	a2,a2,0
 4bc:	00096d03          	0x96d03
 4c0:	0100                	addi	s0,sp,128
 4c2:	1505                	addi	a0,a0,-31
 4c4:	04091803          	lh	a6,64(s2)
 4c8:	0100                	addi	s0,sp,128
 4ca:	1a05                	addi	s4,s4,-31
 4cc:	04090203          	lb	tp,64(s2)
 4d0:	0100                	addi	s0,sp,128
 4d2:	1905                	addi	s2,s2,-31
 4d4:	04097c03          	0x4097c03
 4d8:	0100                	addi	s0,sp,128
 4da:	1305                	addi	t1,t1,-31
 4dc:	04097d03          	0x4097d03
 4e0:	0100                	addi	s0,sp,128
 4e2:	0905                	addi	s2,s2,1
 4e4:	0306                	slli	t1,t1,0x1
 4e6:	090a                	slli	s2,s2,0x2
 4e8:	0008                	0x8
 4ea:	0501                	addi	a0,a0,0
 4ec:	0612                	slli	a2,a2,0x4
 4ee:	00090003          	lb	zero,0(s2)
 4f2:	0100                	addi	s0,sp,128
 4f4:	1305                	addi	t1,t1,-31
 4f6:	0306                	slli	t1,t1,0x1
 4f8:	0976                	slli	s2,s2,0x1d
 4fa:	0004                	0x4
 4fc:	0501                	addi	a0,a0,0
 4fe:	0309                	addi	t1,t1,2
 500:	0901                	addi	s2,s2,0
 502:	0004                	0x4
 504:	0501                	addi	a0,a0,0
 506:	00030613          	mv	a2,t1
 50a:	0009                	c.nop	2
 50c:	0100                	addi	s0,sp,128
 50e:	0905                	addi	s2,s2,1
 510:	0306                	slli	t1,t1,0x1
 512:	0901                	addi	s2,s2,0
 514:	0004                	0x4
 516:	0501                	addi	a0,a0,0
 518:	0614                	addi	a3,sp,768
 51a:	00090003          	lb	zero,0(s2)
 51e:	0100                	addi	s0,sp,128
 520:	1305                	addi	t1,t1,-31
 522:	04090003          	lb	zero,64(s2)
 526:	0100                	addi	s0,sp,128
 528:	0905                	addi	s2,s2,1
 52a:	0306                	slli	t1,t1,0x1
 52c:	0901                	addi	s2,s2,0
 52e:	0004                	0x4
 530:	0501                	addi	a0,a0,0
 532:	030d                	addi	t1,t1,3
 534:	0900                	addi	s0,sp,144
 536:	0000                	unimp
 538:	0501                	addi	a0,a0,0
 53a:	0319                	addi	t1,t1,6
 53c:	0900                	addi	s0,sp,144
 53e:	0000                	unimp
 540:	0501                	addi	a0,a0,0
 542:	7f030613          	addi	a2,t1,2032
 546:	0009                	c.nop	2
 548:	0100                	addi	s0,sp,128
 54a:	0d05                	addi	s10,s10,1
 54c:	0306                	slli	t1,t1,0x1
 54e:	0902                	c.slli64	s2
 550:	0004                	0x4
 552:	0501                	addi	a0,a0,0
 554:	0625                	addi	a2,a2,9
 556:	00090003          	lb	zero,0(s2)
 55a:	0100                	addi	s0,sp,128
 55c:	2e05                	jal	88c <main+0x87c>
 55e:	08090003          	lb	zero,128(s2)
 562:	0100                	addi	s0,sp,128
 564:	1805                	addi	a6,a6,-31
 566:	04090003          	lb	zero,64(s2)
 56a:	0100                	addi	s0,sp,128
 56c:	0d05                	addi	s10,s10,1
 56e:	0306                	slli	t1,t1,0x1
 570:	0901                	addi	s2,s2,0
 572:	0004                	0x4
 574:	0501                	addi	a0,a0,0
 576:	002d                	c.nop	11
 578:	0402                	c.slli64	s0
 57a:	0301                	addi	t1,t1,0
 57c:	0900                	addi	s0,sp,144
 57e:	0000                	unimp
 580:	0501                	addi	a0,a0,0
 582:	0015                	c.nop	5
 584:	0402                	c.slli64	s0
 586:	0601                	addi	a2,a2,0
 588:	00090003          	lb	zero,0(s2)
 58c:	0100                	addi	s0,sp,128
 58e:	2205                	jal	6ae <main+0x69e>
 590:	0200                	addi	s0,sp,256
 592:	0104                	addi	s1,sp,128
 594:	04090003          	lb	zero,64(s2)
 598:	0100                	addi	s0,sp,128
 59a:	2d05                	jal	bca <main+0xbba>
 59c:	0200                	addi	s0,sp,256
 59e:	0104                	addi	s1,sp,128
 5a0:	04090003          	lb	zero,64(s2)
 5a4:	0100                	addi	s0,sp,128
 5a6:	0d05                	addi	s10,s10,1
 5a8:	0200                	addi	s0,sp,256
 5aa:	0204                	addi	s1,sp,256
 5ac:	0306                	slli	t1,t1,0x1
 5ae:	0901                	addi	s2,s2,0
 5b0:	0004                	0x4
 5b2:	0501                	addi	a0,a0,0
 5b4:	0011                	c.nop	4
 5b6:	0402                	c.slli64	s0
 5b8:	0602                	c.slli64	a2
 5ba:	00090003          	lb	zero,0(s2)
 5be:	0100                	addi	s0,sp,128
 5c0:	0d05                	addi	s10,s10,1
 5c2:	0200                	addi	s0,sp,256
 5c4:	0204                	addi	s1,sp,256
 5c6:	0306                	slli	t1,t1,0x1
 5c8:	0901                	addi	s2,s2,0
 5ca:	0008                	0x8
 5cc:	0501                	addi	a0,a0,0
 5ce:	001a                	c.slli	zero,0x6
 5d0:	0402                	c.slli64	s0
 5d2:	0602                	c.slli64	a2
 5d4:	04090003          	lb	zero,64(s2)
 5d8:	0100                	addi	s0,sp,128
 5da:	1f05                	addi	t5,t5,-31
 5dc:	0200                	addi	s0,sp,256
 5de:	0204                	addi	s1,sp,256
 5e0:	0306                	slli	t1,t1,0x1
 5e2:	097c                	addi	a5,sp,156
 5e4:	000c                	0xc
 5e6:	0501                	addi	a0,a0,0
 5e8:	0019                	c.nop	6
 5ea:	0402                	c.slli64	s0
 5ec:	0302                	c.slli64	t1
 5ee:	0900                	addi	s0,sp,144
 5f0:	0000                	unimp
 5f2:	0501                	addi	a0,a0,0
 5f4:	0005                	c.nop	1
 5f6:	0402                	c.slli64	s0
 5f8:	0301                	addi	t1,t1,0
 5fa:	090d                	addi	s2,s2,3
 5fc:	000c                	0xc
 5fe:	0501                	addi	a0,a0,0
 600:	000a                	c.slli	zero,0x2
 602:	0402                	c.slli64	s0
 604:	0301                	addi	t1,t1,0
 606:	0900                	addi	s0,sp,144
 608:	0000                	unimp
 60a:	0901                	addi	s2,s2,0
 60c:	0004                	0x4
 60e:	0100                	addi	s0,sp,128
 610:	01              	Address 0x0000000000000610 is out of bounds.


Disassembly of section .debug_str:

00000000 <.debug_str>:
   0:	6e75                	lui	t3,0x1d
   2:	6e676973          	csrrsi	s2,0x6e6,14
   6:	6465                	lui	s0,0x19
   8:	6920                	flw	fs0,80(a0)
   a:	746e                	flw	fs0,248(sp)
   c:	4700                	lw	s0,8(a4)
   e:	554e                	lw	a0,240(sp)
{
  10:	4320                	lw	s0,64(a4)
  12:	3731                	jal	ffffff1e <main+0xffffff0e>
    volatile uint32_t receptUart = 0;
  14:	3120                	fld	fs0,96(a0)
  16:	2e31                	jal	332 <main+0x322>
    volatile uint32_t bufferWord = 0;
  18:	2e32                	fld	ft8,264(sp)
  1a:	2030                	fld	fa2,64(s0)
    volatile uint32_t bufferByte = 0; 
  1c:	6d2d                	lui	s10,0xb
  1e:	6261                	lui	tp,0x18
    *AD_DISPLAY2=0xff83;     //BO
  20:	3d69                	jal	fffffeba <main+0xfffffeaa>
  22:	6c69                	lui	s8,0x1a
  24:	3370                	fld	fa2,224(a4)
  26:	2032                	fld	ft0,264(sp)
  28:	6d2d                	lui	s10,0xb
  2a:	7261                	lui	tp,0xffff8
  2c:	723d6863          	bltu	s10,gp,75c <main+0x74c>
    *AD_DISPLAY1=0xc0c087ff; //OT
  30:	3376                	fld	ft6,376(sp)
  32:	6932                	flw	fs2,12(sp)
  34:	2d20                	fld	fs0,88(a0)
  36:	4f2d2067          	0x4f2d2067
  3a:	2031                	jal	46 <main+0x36>
  3c:	662d                	lui	a2,0xb
  3e:	7266                	flw	ft4,120(sp)
    uint32_t Len = 0x00000000;
  40:	6565                	lui	a0,0x19
  42:	6e617473          	csrrci	s0,0x6e6,2
        while ((*UART_Status & Rx_full) == 0);
  46:	6964                	flw	fs1,84(a0)
    for(int i = 3; i >= 0; i--){
  48:	676e                	flw	fa4,216(sp)
  4a:	7200                	flw	fs0,32(a2)
        while ((*UART_Status & Rx_full) == 0);
  4c:	6365                	lui	t1,0x19
  4e:	7065                	c.lui	zero,0xffff9
  50:	5574                	lw	a3,108(a0)
  52:	7261                	lui	tp,0xffff8
  54:	0074                	addi	a3,sp,12
  56:	4441                	li	s0,16
        Lentmp = *Rx;
  58:	4d44                	lw	s1,28(a0)
  5a:	7861                	lui	a6,0xffff8
        while ((*UART_Status & Tx_busy) == 1);
  5c:	4100                	lw	s0,0(a0)
  5e:	5f44                	lw	s1,60(a4)
  60:	4944                	lw	s1,20(a0)
  62:	414c5053          	0x414c5053
  66:	3159                	jal	fffffcec <main+0xfffffcdc>
        *Tx=Lentmp;
  68:	4100                	lw	s0,0(a0)
  6a:	5f44                	lw	s1,60(a4)
        Len |= Lentmp<<i*8;
  6c:	4944                	lw	s1,20(a0)
  6e:	414c5053          	0x414c5053
  72:	3259                	jal	fffff9f8 <main+0xfffff9e8>
    for(int i = 3; i >= 0; i--){
  74:	4c00                	lw	s0,24(s0)
  76:	6e65                	lui	t3,0x19
  78:	6d74                	flw	fa3,92(a0)
  7a:	0070                	addi	a2,sp,12
    while (Len > 0 && ADDMax >= addrSRAM){
  7c:	7562                	flw	fa0,56(sp)
  7e:	6666                	flw	fa2,88(sp)
  80:	7265                	lui	tp,0xffff9
  82:	7942                	flw	fs2,48(sp)
  84:	6574                	flw	fa3,76(a0)
  86:	5500                	lw	s0,40(a0)
  88:	5241                	li	tp,-16
  8a:	5f54                	lw	a3,60(a4)
        Len |= Lentmp<<i*8;
  8c:	74617453          	0x74617453
            while (((*UART_Status & Rx_full) == 0) && ((*UART_Status & Rx_error) == 0));
  90:	7375                	lui	t1,0xffffd
  92:	7500                	flw	fs0,40(a0)
            *AD_DISPLAY1=(receptUart<<(16));
  94:	736e                	flw	ft6,248(sp)
  96:	6769                	lui	a4,0x1a
        for(int i = 3; i >= 0; i--){
  98:	656e                	flw	fa0,216(sp)
  9a:	2064                	fld	fs1,192(s0)
  9c:	72616863          	bltu	sp,t1,7cc <main+0x7bc>
                tmp=*UART_Status;
  a0:	6100                	flw	fs0,0(a0)
  a2:	6464                	flw	fs1,76(s0)
                if((tmp & Rx_full) != 0) break;
  a4:	5372                	lw	t1,60(sp)
  a6:	4152                	lw	sp,20(sp)
  a8:	424d                	li	tp,19
  aa:	6d00                	flw	fs0,24(a0)
            receptUart=*Rx;
  ac:	6961                	lui	s2,0x18
  ae:	006e                	c.slli	zero,0x1b
  b0:	7562                	flw	fa0,56(sp)
  b2:	6666                	flw	fa2,88(sp)
            *AD_DISPLAY1=(receptUart<<(16));
  b4:	7265                	lui	tp,0xffff9
  b6:	64726f57          	0x64726f57
  ba:	4100                	lw	s0,0(a0)
  bc:	4444                	lw	s1,12(s0)
  be:	614d                	addi	sp,sp,176
            bufferWord |= (receptUart<<(i*8));
  c0:	4278                	lw	a4,68(a2)
  c2:	6100                	flw	fs0,0(a0)
  c4:	6464                	flw	fs1,76(s0)
  c6:	5372                	lw	t1,60(sp)
  c8:	4152                	lw	sp,20(sp)
  ca:	004d                	c.nop	19

Disassembly of section .debug_line_str:

00000000 <.debug_line_str>:
   0:	555c3a43          	0x555c3a43
   4:	73726573          	csrrsi	a0,0x737,4
   8:	6c5c                	flw	fa5,28(s0)
   a:	6165                	addi	sp,sp,112
   c:	616d                	addi	sp,sp,240
   e:	415c                	lw	a5,4(a0)
{
  10:	6572                	flw	fa0,28(sp)
  12:	5c616973          	csrrsi	s2,0x5c6,2
    volatile uint32_t receptUart = 0;
  16:	614d                	addi	sp,sp,176
    volatile uint32_t bufferWord = 0;
  18:	7369                	lui	t1,0xffffa
  1a:	696e6e6f          	jal	t3,e66b0 <main+0xe66a0>
    volatile uint32_t bufferByte = 0; 
  1e:	7265                	lui	tp,0xffff9
    *AD_DISPLAY2=0xff83;     //BO
  20:	7241                	lui	tp,0xffff0
  22:	6962                	flw	fs2,24(sp)
  24:	7274                	flw	fa3,100(a2)
  26:	5265                	li	tp,-7
  28:	7369                	lui	t1,0xffffa
  2a:	535f5663          	bge	t5,s5,556 <main+0x546>
  2e:	7379                	lui	t1,0xffffe
    *AD_DISPLAY1=0xc0c087ff; //OT
  30:	5c646f43          	0x5c646f43
  34:	6974754f          	fnmadd.s	fa0,fs0,fs7,fa3
  38:	736c                	flw	fa1,100(a4)
  3a:	6544                	flw	fs1,12(a0)
  3c:	656e6547          	0x656e6547
    uint32_t Len = 0x00000000;
  40:	6172                	flw	ft2,28(sp)
  42:	6974                	flw	fa3,84(a0)
        while ((*UART_Status & Rx_full) == 0);
  44:	675c6e6f          	jal	t3,c6eb8 <main+0xc6ea8>
    for(int i = 3; i >= 0; i--){
  48:	6e65                	lui	t3,0x19
  4a:	7265                	lui	tp,0xffff9
        while ((*UART_Status & Rx_full) == 0);
  4c:	7461                	lui	s0,0xffff8
  4e:	5c73726f          	jal	tp,37e14 <main+0x37e04>
  52:	6f745f43          	fmadd.q	ft10,fs0,fs7,fa3,unknown
  56:	615f 6c6c 6900      	0x69006c6c615f
        while ((*UART_Status & Tx_busy) == 1);
  5c:	706e                	flw	ft0,248(sp)
  5e:	7475                	lui	s0,0xffffd
  60:	69616d2f          	0x69616d2f
  64:	2e6e                	fld	ft8,216(sp)
  66:	6e690063          	beq	s2,t1,746 <main+0x736>
        *Tx=Lentmp;
  6a:	7570                	flw	fa2,108(a0)
        Len |= Lentmp<<i*8;
  6c:	0074                	addi	a3,sp,12

Disassembly of section .debug_frame:

00000000 <.debug_frame>:
   0:	000c                	0xc
   2:	0000                	unimp
   4:	ffff                	0xffff
   6:	ffff                	0xffff
   8:	7c010003          	lb	zero,1984(sp)
   c:	0d01                	addi	s10,s10,0
   e:	0002                	c.slli64	zero
{
  10:	0010                	0x10
  12:	0000                	unimp
    volatile uint32_t receptUart = 0;
  14:	0000                	unimp
  16:	0000                	unimp
    volatile uint32_t bufferWord = 0;
  18:	0010                	0x10
  1a:	0000                	unimp
    volatile uint32_t bufferByte = 0; 
  1c:	01c8                	addi	a0,sp,196
  1e:	0000                	unimp
    *AD_DISPLAY2=0xff83;     //BO
  20:	0e44                	addi	s1,sp,788
  22:	0010                	0x10
