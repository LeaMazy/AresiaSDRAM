onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate -radix hexadecimal /testbenchtop/reset
add wave -noupdate -radix hexadecimal /testbenchtop/ck
add wave -noupdate -radix decimal /testbenchtop/counter
add wave -noupdate -radix hexadecimal /testbenchtop/progcounter
add wave -noupdate -radix hexadecimal /testbenchtop/instr
add wave -noupdate -radix hexadecimal /testbenchtop/dataAddr
add wave -noupdate -radix hexadecimal /testbenchtop/load
add wave -noupdate -radix hexadecimal /testbenchtop/store
add wave -noupdate -radix hexadecimal /testbenchtop/dataLength
add wave -noupdate -radix hexadecimal /testbenchtop/inputData
add wave -noupdate -radix hexadecimal /testbenchtop/outputData
add wave -noupdate -radix hexadecimal /testbenchtop/iTop/minicacheInst/PROChold
add wave -noupdate -radix hexadecimal /testbenchtop/iTop/minicacheInst/currentState
add wave -noupdate -radix hexadecimal /testbenchtop/iTop/minicacheInst/currentStateInit
add wave -noupdate -radix hexadecimal /testbenchtop/iTop/minicacheInst/nextState
add wave -noupdate -radix hexadecimal /testbenchtop/iTop/minicacheInst/nextStateInit
add wave -noupdate -radix hexadecimal /testbenchtop/outputData_SDRAM
add wave -noupdate -radix hexadecimal /testbenchtop/inputData_SDRAM
add wave -noupdate -radix hexadecimal /testbenchtop/AddrSDRAM
add wave -noupdate -radix hexadecimal /testbenchtop/AddrSDRAM2
add wave -noupdate -radix hexadecimal /testbenchtop/AddrSDRAM3
add wave -noupdate -radix hexadecimal /testbenchtop/AddrSDRAM4
add wave -noupdate -radix hexadecimal /testbenchtop/AddrSDRAM5
add wave -noupdate -radix hexadecimal /testbenchtop/AddrSDRAM6
add wave -noupdate -radix hexadecimal /testbenchtop/DQM_SDRAM
add wave -noupdate -radix hexadecimal /testbenchtop/dataReady_SDRAM
add wave -noupdate -radix hexadecimal /testbenchtop/SDRAMselect
add wave -noupdate -radix hexadecimal /testbenchtop/SDRAMwrite
add wave -noupdate -radix hexadecimal /testbenchtop/reg00
add wave -noupdate -radix hexadecimal /testbenchtop/reg01
add wave -noupdate -radix hexadecimal /testbenchtop/reg02
add wave -noupdate -radix hexadecimal /testbenchtop/reg03
add wave -noupdate -radix hexadecimal /testbenchtop/reg04
add wave -noupdate -radix hexadecimal /testbenchtop/reg05
add wave -noupdate -radix hexadecimal /testbenchtop/reg06
add wave -noupdate -radix hexadecimal /testbenchtop/reg07
add wave -noupdate -radix hexadecimal /testbenchtop/reg08
add wave -noupdate -radix hexadecimal /testbenchtop/reg09
add wave -noupdate -radix hexadecimal /testbenchtop/reg0A
add wave -noupdate -radix hexadecimal /testbenchtop/reg0B
add wave -noupdate -radix hexadecimal /testbenchtop/reg0C
add wave -noupdate -radix hexadecimal /testbenchtop/reg0D
add wave -noupdate -radix hexadecimal /testbenchtop/reg0E
add wave -noupdate -radix hexadecimal /testbenchtop/reg0F
add wave -noupdate -radix hexadecimal /testbenchtop/reg10
add wave -noupdate -radix hexadecimal /testbenchtop/reg11
add wave -noupdate -radix hexadecimal /testbenchtop/reg12
add wave -noupdate -radix hexadecimal /testbenchtop/reg13
add wave -noupdate -radix hexadecimal /testbenchtop/reg14
add wave -noupdate -radix hexadecimal /testbenchtop/reg15
add wave -noupdate -radix hexadecimal /testbenchtop/reg16
add wave -noupdate -radix hexadecimal /testbenchtop/reg17
add wave -noupdate -radix hexadecimal /testbenchtop/reg18
add wave -noupdate -radix hexadecimal /testbenchtop/reg19
add wave -noupdate -radix hexadecimal /testbenchtop/reg1A
add wave -noupdate -radix hexadecimal /testbenchtop/reg1B
add wave -noupdate -radix hexadecimal /testbenchtop/reg1C
add wave -noupdate -radix hexadecimal /testbenchtop/reg1D
add wave -noupdate -radix hexadecimal /testbenchtop/reg1E
add wave -noupdate -radix hexadecimal /testbenchtop/reg1F
add wave -noupdate -radix hexadecimal /testbenchtop/SigTOPdisplay1
add wave -noupdate -radix hexadecimal /testbenchtop/SigTOPdisplay2
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {376000000 ps} 0}
quietly wave cursor active 1
configure wave -namecolwidth 243
configure wave -valuecolwidth 100
configure wave -justifyvalue left
configure wave -signalnamewidth 0
configure wave -snapdistance 10
configure wave -datasetprefix 0
configure wave -rowmargin 4
configure wave -childrowmargin 2
configure wave -gridoffset 0
configure wave -gridperiod 1
configure wave -griddelta 40
configure wave -timeline 0
configure wave -timelineunits ps
update
WaveRestoreZoom {0 ps} {9173048320 ps}
