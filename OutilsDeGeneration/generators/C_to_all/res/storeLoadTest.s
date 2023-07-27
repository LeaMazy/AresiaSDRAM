.globl _start
################################# BOOT SEQUENCE ################################
_start:
    # Trick to jump over PC = 8 to avoid simulator to stop
    jal x0, _regs
    nop
    nop

_regs :
    li x18, 0xff0 # base address
    li x19, 0xff4 # base address

    lui x20, 0x11111
    addi x20, x20, 0x111

    lui x21, 0x22222
    addi x21, x21, 0x222

    lui x22, 0x33333
    addi x22, x22, 0x333

    lui x23, 0x44444
    addi x23, x23, 0x444

    lui x24, 0x55555
    addi x24, x24, 0x555

    lui x25, 0x66666
    addi x25, x25, 0x666

_sb :
    sb x20, 0(x18)
    sb x21, 0(x19)

_sh :
    sh x22, -4(x18)
    sh x23, 4(x19)

_sw :
    sw x24, -8(x18)
    sw x25, 8(x19)

    nop
    nop
    nop

_lbu:
    lbu x5, 0(x18)
_lb :
    lb x6, -4(x18)
_lh :
    lh x7, -8(x18)
_lhu:
    lhu x8, 4(x19)
_lw:
    lw x9,  8(x19)

###################################### END #####################################
_end:
    # Set return address to 8 because the simulator stop at PC = 8
    li x1, 0x8
    ret