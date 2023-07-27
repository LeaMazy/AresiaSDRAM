.globl _start
################################# BOOT SEQUENCE ################################
_start:
    # Trick to jump over PC = 8 to avoid simulator to stop
    jal x0, _lui
    nop
    nop

################################ LUI      OPCODE ###############################
# Instruction :
    # LUI -done
_lui:
    # Test to fill all registers
    lui x0 , 0x0
    lui x1 , 0x1
    lui x2 , 0xff
    lui x3 , 0xffff
    lui x4 , 0xfffff
    lui x5 , 0xfffff
    lui x6 , 0xfffff
    lui x7 , 0xfffff
    lui x8 , 0xfffff
    lui x9 , 0xfffff
    lui x10, 0xfffff
    lui x11, 0xfffff
    lui x12, 0xfffff
    lui x13, 0xfffff
    lui x14, 0xfffff
    lui x15, 0xfffff
    lui x16, 0xfffff
    lui x17, 0xfffff
    lui x18, 0xfffff
    lui x19, 0xfffff
    lui x20, 0xfffff
    lui x21, 0xfffff
    lui x22, 0xfffff
    lui x23, 0xfffff
    lui x24, 0xfffff
    lui x25, 0xfffff
    lui x26, 0xfffff
    lui x27, 0xfffff
    lui x28, 0xfffff
    lui x29, 0xfffff
    lui x30, 0xfffff
    lui x31, 0xfffff
    lui x31, 0xfffff

################################ AUIPC    OPCODE ###############################
_auipc :
# Instruction :
    # AUIPC -done
    auipc x3, 0xfffff
    auipc x3, 0
    auipc x3, 4
    j _jal
################################ JAL      OPCODE ###############################
_jalNegOffset :
    j _jalPosOffset
# Instruction :
    # JAL -done
_jal :
    jal x0, _jalNegOffset

_jalPosOffset :
    jal x2, _jalr
################################ JALR     OPCODE ###############################
_jalr :
# Instruction :
    # JALR -done
    # test if LSB is set to 0 and test positive offset
    jalr x3, x2, 5
    # Set PC in reg3
    auipc x3, 0
    # Add 8 to prepare the test negative offset
    addi x3, x3, 8
    # Jump to PC + 8 - 4 aka next instruction
    jalr x2, x3, -4
################################ BRANCH   OPCODE ###############################
# Instructions :
    # BEQ   -Done
    # BNE   -Done
    # BLT   -Done
    # BGE   -Done
    # BLTU  -Done
    # BGEU  -Done

# Load registers used in branch functions
_loadBranchRegs :
    li x12, 3
    li x13, 4
    li x14, 4
    li x15, -5
    li x16, -4
    li x17, -4
    j _beq

# labels to test negative jump offset
_beqNegOffset:
    j _bne

_bneNegOffset:
    j _blt

_bltNegOffset :
    j _bltu

_bltuNegOffset :
    j _bge

_bgeNegOffset :
    j _bgeu

_bgeuNegOffset :
    j _sb

# Branch if equals
_beq:
    # branch false
    beq x12, x13, _end      #  3 ==  4
    beq x12, x15, _end      #  3 == -5
    beq x15, x16, _end      # -5 == -4

    # Branch true
    beq x13, x14, _beqPosOk #  4 ==  4
    j _end
    _beqPosOk:
    beq x16, x17, _beqNegOffset # -4 == -4
    j _end

# Branch if not equals
_bne :
    # branch false
    bne x13, x14, _end  #   4 !=  4
    bne x16, x17, _end  #  -4 != -4

    # Branch true
    bne x12, x13,  _bnePosOk # 3 != 4
    j _end
    _bnePosOk :
    bne x12, x15, _bnePosNegOk #  3 != -5
    j _end
    _bnePosNegOk :
    bne x15, x16, _bneNegOffset # -5 != -4
    j _end

# Branch if lesser
_blt :
    # branch false
    blt x13, x13, _end # 4 < 4
    blt x16, x15, _end # -4 < -5
    blt x14, x13, _end # 4 < 3

    # Branch true
    blt x12, x14, _bltPosOk # 3 < 4
    j _end
    _bltPosOk :
    blt x15, x16, _bltNegOk # -5 < -4
    j _end
    _bltNegOk :
    blt x15, x13, _bltNegOffset # -5 < 4
    j _end

# Branch if lesser unsigned
_bltu :
    # branch false
    bltu x13, x13, _end # 4 < 4
    bltu x16, x15, _end # xFFFC < xFFFB
    bltu x14, x12, _end # 4 < 3

    # Branch true
    bltu x12, x14, _bltuPosOk # 3 < 4
    j _end
    _bltuPosOk :
    bltu x15, x16, _bltuNegOk # xFFFB < xFFFC
    j _end
    _bltuNegOk :
    bltu x13, x15, _bltuNegOffset # 4 < xFFFB
    j _end

# Branch if bigger
_bge :
    # branch false
    bge x15,x16, _end # -5 > -4
    bge x12,x14, _end # 3 > 4

    # Branch true
    bge x13,x13, _bgeEqualOk # 4 > 4
    j _end
    _bgeEqualOk :
    bge x14, x12, _bgePosOk # 4 > 3
    j _end
    _bgePosOk :
    bge x16, x15, _bgeNegOk #  -4 > -5
    j _end
    _bgeNegOk :
    bge x13, x15, _bgeNegOffset #  4 > -5
    j _end

# Branch if bigger unsigned
_bgeu :
    # branch false
    bgeu  x15, x16, _end # xFFFB > xFFFC
    bgeu  x12, x14, _end # 3 > 4

    # Branch true
    bge x13,x13, _bgeuEqualOk # 4 > 4
    j _end
    _bgeuEqualOk :
    bgeu x14, x12, _bgeuPosOk # 4 > 3
    j _end
    _bgeuPosOk :
    bgeu x16, x15, _bgeuNegOk # xFFFC > xFFFB
    j _end
    _bgeuNegOk :
    bgeu x15, x13, _bgeuNegOffset #  xFFFB > 4
    j _end

################################ STORE    OPCODE ###############################
#Instructions
    # SB -done
    # SH -done
    # SW -done
_sb :
    li x18, 0x205 # base address
    li x19, 0x201 # base address
    li x20, 0x1400 # base address

    lui x21, 0x80808
    addi x21, x21, 0x080
    lui x22, 0x11223
    addi x22, x22, 0x344

    sb x21, -2048(x20) #  dataMem[reg[20] - 2048] = reg[21] => dataMem[0x800] = 0x80
    sb x22, 0x7ff(x19) #  dataMem[reg[0]  + 2047] = reg[21] => dataMem[0x7ff] = 0x44

_sh :
    sh x21, -2048(x20) #  dataMem[reg[20] - 2048] = reg[21] => dataMem[0x800] = 0x8080
    sh x22, 0x7ff(x19) #  dataMem[reg[0]  + 2047] = reg[21] => dataMem[0x7ff] = 0x3344

_sw :
    sw x21, -2048(x20) #  dataMem[reg[20] - 2048] = reg[21] => dataMem[0x800] = 0x80808080
    sw x22, 0x7ff(x19) #  dataMem[reg[0]  + 2047] = reg[21] => dataMem[0x7ff] = 0x11223344

################################ LOAD     OPCODE ###############################
#Instructions
    # LB    -done
    # LH    -done
    # LW    -done
    # LBU   -done
    # LHU   -done
_lb :
    lb x5, -2048(x20)   # load signed value
    lb x5, 0x7ff(x19)   # load unsigned value

_lbu:
    lbu x5, -2048(x20)  # load signed value
    lbu x5, 0x7ff(x19)  # load unsigned value

_lh :
    lh x5, -2048(x20)   # load signed value
    lh x5, 0x7ff(x19)   # load unsigned value

_lhu:
    lhu x5, -2048(x20)  # load signed value
    lhu x5, 0x7ff(x19)  # load unsigned value

_lw:
    lw x5, -2048(x20)  # load signed value
    lw x5, 0x7ff(x19)  # load unsigned value

################################ OP_IMM   OPCODE ###############################
#Instructions
    # ADDI  -done
    # SLTI  -done
    # SLTIU -done
    # XORI  -done
    # ORI   -done
    # ANDI  -done
    # SLLI  -done
    # SRLI  -done
    # SRAI  -done
_addi:
    addi x12, x0, -2048     # Crash if 0x800
    addi x12, x12, 0x7ff    # reg12 = -2048 + 2047 = -1
    addi x12, x12, 0x7ff    # reg12 = -1 + 2047 = 2046
    addi x12, x0, 0         # reg12 = 0

_slti:
    li x15, 6
    slti x5, x15,  6 # false reg5 = ( 6 <  6 ) ? = 0
    slti x5, x15, -7 # false reg5 = ( 6 < -7 ) ? = 0
    slti x5, x15,  7 # true  reg5 = ( 6 <  7 ) ? = 1
    li x15, -6
    slti x5, x15, -7 # false reg5 = (-6 < -7 ) ? = 0
    slti x5, x15,  0 # true  reg5 = (-6 < 0  ) ? = 1

_sltiu :
    sltiu x5, x15,  6 # false reg5 = ( 0xfffffffa < 0x6 ) ? = 0
    sltiu x5, x15, -7 # false reg5 = ( 0xfffffffa < 0xfffffff9 ) ? = 0
    sltiu x5, x15, -5 # true  reg5 = ( 0xfffffffa < 0xfffffffb ) ? = 1
    li x15, 6

_xori :
    addi x10, x0, -1     # reg10 = 0xffffffff
    xori x10, x10, 0x555 # reg10 = 0xffffffff ^ 0x555 = 0xfffffaaa
    xori x10, x10, 0x555 # reg10 = 0xfffffaaa ^ 0x555 = 0xffffffff
    xori x10, x10, -241  # reg10 = 0xffffffff ^ 0xffffff0f = 0xf0

_ori :
    addi x10, x0, 0     # reg10 = 0
    ori x10, x10, -241  # reg10 =  0x0 | 0xffffff0f = 0xffffff0f
    ori x10, x10, 0x7ff # reg10 =  0xffffff0f | 0x7ff = 0xffffffff

_andi :
    andi x10, x10, 0xff #reg10 = 0xffffffff & 0xff = 0xff
    andi x10, x10, -241 #reg10 = 0xff & 0xffffff0f = 0xf

_slli :
    addi x9 , x0,  1    # reg9  = 1
    slli x12, x9,  31   # reg12 = 1 << 31 = 0x80000000
    slli x11, x9,  8    # reg11 = 1 << 8  = 0x100
    slli x11, x11, 16   # reg11 = 1 << 16 = 0x1000000

_srli :
    addi x11, x0, -1    # reg11 = 0xffffffff
    srli x10, x11, 31   # reg10 = 0xffffffff >> 31 = 0x1
    srli x10, x11, 8    # reg10 = 0xffffffff >> 8 = 0xffffff
    srli x10, x10, 16   # reg10 = 0xffffff   >> 16 = 0xff
    srli x10, x10, 24   # reg10 = 0xffffff   >> 24 = 0x0

_srai :
    srai x11, x12, 31   # reg11 = 0x80000000 >> (int) 31 = 0xffffffff
    srai x11, x12, 8    # reg11 = 0x80000000 >> (int) 8  = 0xff800000
    srai x11, x11, 16   # reg11 = 0xff800000 >> (int) 16 = 0xffffff80
    srai x11, x11, 24   # reg11 = 0xffffff80 >> (int) 24 = 0xffffffff
################################ OP       OPCODE ###############################
    # ADD   - done
    # SUB   - done
    # SLL   - done
    # SLT   - done
    # SLTU  - done
    # XOR   - done
    # SRL   - done
    # SRA   - done
    # OR    - done
    # AND   - done
_add:
    li x3, -2048
    li x4, 0x7ff

    add x12, x0, x3     # reg12 = 0x0 + 0xfffff800 = 0xfffff800
    add x12, x12, x4    # reg12 = -2048 + 2047 = -1
    add x12, x12, x4    # reg12 = -1 + 2047 = 2046
    add x12, x0 , x0    # reg12 = 0

_slt:
    li x15, 6
    li x7, -7
    li x8,  7
    li x9, -6
    li x10,-5

    slt x5, x15, x15 # false reg5 = ( 6 <  6 ) ? = 0
    slt x5, x15, x7  # false reg5 = ( 6 < -7 ) ? = 0
    slt x5, x15, x8  # true  reg5 = ( 6 <  7 ) ? = 1

    slt x5, x9, x7  # false reg5 = (-6 < -7 ) ? = 0
    slt x5, x9, x0  # true  reg5 = (-6 <  0 ) ? = 1

_sltu :
    sltu x5, x9, x15 # false reg5 = ( 0xfffffffa < 0x6 ) ? = 0
    sltu x5, x9, x7  # false reg5 = ( 0xfffffffa < 0xfffffff9 ) ? = 0
    sltu x5, x9, x10 # true  reg5 = ( 0xfffffffa < 0xfffffffb ) ? = 1

_xor :
    li x10, -1
    li x11, 0x555
    li x12, -241
    xor x10, x10, x11 # reg10 = 0xffffffff ^ 0x555 = 0xfffffaaa
    xor x10, x10, x11 # reg10 = 0xfffffaaa ^ 0x555 = 0xffffffff
    xor x10, x10, x12 # reg10 = 0xffffffff ^ 0xffffff0f = 0xf0

_or :
    li x13, 0x7ff
    or x10, x0, x12  # reg10 =  0x0 | 0xffffff0f = 0xffffff0f
    or x10, x10, x13 # reg10 =  0xffffff0f | 0x7ff = 0xffffffff

_and :
    li x11, 0xff
    and x10, x10, x11 #reg10 = 0xffffffff & 0xff = 0xff
    and x10, x10, x12 #reg10 = 0xff & 0xffffff0f = 0xf

_sll :
    li x13, 1
    li x14, 31
    li x15, 8
    li x16, 16
    li x17, 24
    sll x12, x13, x14   # reg12 = 1 << 31 = 0x80000000
    sll x11, x13, x15   # reg11 = 1 << 8  = 0x100
    sll x11, x11, x16   # reg11 = 0x100 << 16 = 0x1000000
    sll x11, x11, x17   # reg11 = 0x1000000 << 24 = 0x0

_srl :
    addi x11, x0, -1    # reg11 = 0xffffffff
    srl x10, x11, x14   # reg10 = 0xffffffff >> 31 = 0x1
    srl x10, x11, x15   # reg10 = 0xffffffff >> 8 = 0xffffff
    srl x10, x10, x16   # reg10 = 0xffffff   >> 16 = 0xff
    srl x10, x10, x17   # reg10 = 0xffffff   >> 24 = 0x0

_sra :
    sra x11, x12, x14   # reg11 = 0x80000000 >> (int) 31 = 0xffffffff
    sra x11, x12, x15   # reg11 = 0x80000000 >> (int) 8  = 0xff800000
    sra x11, x11, x16   # reg11 = 0xff800000 >> (int) 16 = 0xffffff80
    sra x11, x11, x17   # reg11 = 0xffffff80 >> (int) 24 = 0xffffffff
################################ MISC_MEM OPCODE ###############################
    # FENCE
    # ECALL
    # EBREA
################################ SYSTEM   OPCODE ###############################

###################################### END #####################################
_end:
    # Set return address to 8 because the simulator stop at PC = 8
    li x1, 0x8
    ret