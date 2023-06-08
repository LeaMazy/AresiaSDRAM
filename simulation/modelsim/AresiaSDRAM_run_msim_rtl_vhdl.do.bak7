transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

vlog -vlog01compat -work work +incdir+C:/ECE/Aresia/TestProcGit/AresiaSDRAM/db {C:/ECE/Aresia/TestProcGit/AresiaSDRAM/db/clock1m_altpll.v}
vcom -93 -work work {C:/ECE/Aresia/TestProcGit/AresiaSDRAM/SDRAM_package.vhd}
vcom -93 -work work {C:/ECE/Aresia/TestProcGit/AresiaSDRAM/simul_var_pkg.vhd}
vcom -93 -work work {C:/ECE/Aresia/TestProcGit/AresiaSDRAM/SegmentDecoder.vhd}
vcom -93 -work work {C:/ECE/Aresia/TestProcGit/AresiaSDRAM/RAM8x4.vhd}
vcom -93 -work work {C:/ECE/Aresia/TestProcGit/AresiaSDRAM/RAM8_3.vhd}
vcom -93 -work work {C:/ECE/Aresia/TestProcGit/AresiaSDRAM/RAM8_2.vhd}
vcom -93 -work work {C:/ECE/Aresia/TestProcGit/AresiaSDRAM/RAM8_1.vhd}
vcom -93 -work work {C:/ECE/Aresia/TestProcGit/AresiaSDRAM/RAM8_0.vhd}
vcom -93 -work work {C:/ECE/Aresia/TestProcGit/AresiaSDRAM/ProgramCounter.vhd}
vcom -93 -work work {C:/ECE/Aresia/TestProcGit/AresiaSDRAM/Processor.vhd}
vcom -93 -work work {C:/ECE/Aresia/TestProcGit/AresiaSDRAM/InstructionDecoder.vhd}
vcom -93 -work work {C:/ECE/Aresia/TestProcGit/AresiaSDRAM/Displays.vhd}
vcom -93 -work work {C:/ECE/Aresia/TestProcGit/AresiaSDRAM/Counter.vhd}
vcom -93 -work work {C:/ECE/Aresia/TestProcGit/AresiaSDRAM/clock1M.vhd}
vcom -93 -work work {C:/ECE/Aresia/TestProcGit/AresiaSDRAM/Alu.vhd}
vcom -93 -work work {C:/ECE/Aresia/TestProcGit/AresiaSDRAM/Alignment.vhd}
vcom -93 -work work {C:/ECE/Aresia/TestProcGit/AresiaSDRAM/Bootloader.vhd}
vcom -93 -work work {C:/ECE/Aresia/TestProcGit/AresiaSDRAM/UART.vhd}
vcom -93 -work work {C:/ECE/Aresia/TestProcGit/AresiaSDRAM/UARTComm.vhd}
vcom -93 -work work {C:/ECE/Aresia/TestProcGit/AresiaSDRAM/SDRAM_controller.vhd}
vcom -93 -work work {C:/ECE/Aresia/TestProcGit/AresiaSDRAM/SDRAM_32b.vhd}
vcom -93 -work work {C:/ECE/Aresia/TestProcGit/AresiaSDRAM/miniCache.vhd}
vcom -93 -work work {C:/ECE/Aresia/TestProcGit/AresiaSDRAM/Top.vhd}
vcom -93 -work work {C:/ECE/Aresia/TestProcGit/AresiaSDRAM/RegisterFile.vhd}
vcom -93 -work work {C:/ECE/Aresia/TestProcGit/AresiaSDRAM/DEBUGER.vhd}

vcom -93 -work work {C:/ECE/Aresia/TestProcGit/AresiaSDRAM/TestBench.vhd}

vsim -t 1ps -L altera -L lpm -L sgate -L altera_mf -L altera_lnsim -L fiftyfivenm -L rtl_work -L work -voptargs="+acc"  TestBench

do C:/ECE/Aresia/TestProcGit/AresiaSDRAM/simulation/modelsim/my_custom_view.do
