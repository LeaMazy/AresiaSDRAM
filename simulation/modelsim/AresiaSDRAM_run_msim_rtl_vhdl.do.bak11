transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

vlog -vlog01compat -work work +incdir+C:/Users/leama/Aresia/GIT_SDRAM/AresiaSDRAM/db {C:/Users/leama/Aresia/GIT_SDRAM/AresiaSDRAM/db/clock1m_altpll.v}
vcom -93 -work work {C:/Users/leama/Aresia/GIT_SDRAM/AresiaSDRAM/SDRAM_package.vhd}
vcom -93 -work work {C:/Users/leama/Aresia/GIT_SDRAM/AresiaSDRAM/simul_var_pkg.vhd}
vcom -93 -work work {C:/Users/leama/Aresia/GIT_SDRAM/AresiaSDRAM/SegmentDecoder.vhd}
vcom -93 -work work {C:/Users/leama/Aresia/GIT_SDRAM/AresiaSDRAM/RAM8x4.vhd}
vcom -93 -work work {C:/Users/leama/Aresia/GIT_SDRAM/AresiaSDRAM/RAM8_3.vhd}
vcom -93 -work work {C:/Users/leama/Aresia/GIT_SDRAM/AresiaSDRAM/RAM8_2.vhd}
vcom -93 -work work {C:/Users/leama/Aresia/GIT_SDRAM/AresiaSDRAM/RAM8_1.vhd}
vcom -93 -work work {C:/Users/leama/Aresia/GIT_SDRAM/AresiaSDRAM/RAM8_0.vhd}
vcom -93 -work work {C:/Users/leama/Aresia/GIT_SDRAM/AresiaSDRAM/ProgramCounter.vhd}
vcom -93 -work work {C:/Users/leama/Aresia/GIT_SDRAM/AresiaSDRAM/Processor.vhd}
vcom -93 -work work {C:/Users/leama/Aresia/GIT_SDRAM/AresiaSDRAM/InstructionDecoder.vhd}
vcom -93 -work work {C:/Users/leama/Aresia/GIT_SDRAM/AresiaSDRAM/Counter.vhd}
vcom -93 -work work {C:/Users/leama/Aresia/GIT_SDRAM/AresiaSDRAM/clock1M.vhd}
vcom -93 -work work {C:/Users/leama/Aresia/GIT_SDRAM/AresiaSDRAM/Alu.vhd}
vcom -93 -work work {C:/Users/leama/Aresia/GIT_SDRAM/AresiaSDRAM/Alignment.vhd}
vcom -93 -work work {C:/Users/leama/Aresia/GIT_SDRAM/AresiaSDRAM/Bootloader.vhd}
vcom -93 -work work {C:/Users/leama/Aresia/GIT_SDRAM/AresiaSDRAM/UART.vhd}
vcom -93 -work work {C:/Users/leama/Aresia/GIT_SDRAM/AresiaSDRAM/UARTComm.vhd}
vcom -93 -work work {C:/Users/leama/Aresia/GIT_SDRAM/AresiaSDRAM/GPIO.vhd}
vcom -93 -work work {C:/Users/leama/Aresia/GIT_SDRAM/AresiaSDRAM/SDRAM_controller.vhd}
vcom -93 -work work {C:/Users/leama/Aresia/GIT_SDRAM/AresiaSDRAM/SDRAM_32b.vhd}
vcom -93 -work work {C:/Users/leama/Aresia/GIT_SDRAM/AresiaSDRAM/miniCache.vhd}
vcom -93 -work work {C:/Users/leama/Aresia/GIT_SDRAM/AresiaSDRAM/Top.vhd}
vcom -93 -work work {C:/Users/leama/Aresia/GIT_SDRAM/AresiaSDRAM/RegisterFile.vhd}
vcom -93 -work work {C:/Users/leama/Aresia/GIT_SDRAM/AresiaSDRAM/DEBUGER.vhd}

vcom -93 -work work {C:/Users/leama/Aresia/GIT_SDRAM/AresiaSDRAM/TestBench.vhd}

vsim -t 1ps -L altera -L lpm -L sgate -L altera_mf -L altera_lnsim -L fiftyfivenm -L rtl_work -L work -voptargs="+acc"  TestBench

do C:/Users/leama/Aresia/GIT_SDRAM/AresiaSDRAM/simulation/modelsim/my_custom_view.do
