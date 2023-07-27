.globl _start
_start:
    lui x2, 0x1
    jal main
    sbreak
    j .
    