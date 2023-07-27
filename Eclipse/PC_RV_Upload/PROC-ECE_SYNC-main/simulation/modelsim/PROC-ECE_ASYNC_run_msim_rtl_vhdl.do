transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

vlog -vlog01compat -work work +incdir+C:/ECE/Aresia/TestProcGit/PROC-ECE_SYNC-main/db {C:/ECE/Aresia/TestProcGit/PROC-ECE_SYNC-main/db/clock1m_altpll.v}
vcom -93 -work work {C:/ECE/Aresia/TestProcGit/PROC-ECE_SYNC-main/vhdl_files/SDRAM_package.vhd}
vcom -93 -work work {C:/ECE/Aresia/TestProcGit/PROC-ECE_SYNC-main/vhdl_files/SegmentDecoder.vhd}
vcom -93 -work work {C:/ECE/Aresia/TestProcGit/PROC-ECE_SYNC-main/custom_lib/simul_var_pkg.vhd}
vcom -93 -work work {C:/ECE/Aresia/TestProcGit/PROC-ECE_SYNC-main/vhdl_files/ProgramCounter.vhd}
vcom -93 -work work {C:/ECE/Aresia/TestProcGit/PROC-ECE_SYNC-main/vhdl_files/Processor.vhd}
vcom -93 -work work {C:/ECE/Aresia/TestProcGit/PROC-ECE_SYNC-main/vhdl_files/InstructionDecoder.vhd}
vcom -93 -work work {C:/ECE/Aresia/TestProcGit/PROC-ECE_SYNC-main/vhdl_files/Displays.vhd}
vcom -93 -work work {C:/ECE/Aresia/TestProcGit/PROC-ECE_SYNC-main/vhdl_files/Counter.vhd}
vcom -93 -work work {C:/ECE/Aresia/TestProcGit/PROC-ECE_SYNC-main/vhdl_files/Alu.vhd}
vcom -93 -work work {C:/ECE/Aresia/TestProcGit/PROC-ECE_SYNC-main/RAM_2PORT.vhd}
vcom -93 -work work {C:/ECE/Aresia/TestProcGit/PROC-ECE_SYNC-main/clock1M.vhd}
vcom -93 -work work {C:/ECE/Aresia/TestProcGit/PROC-ECE_SYNC-main/vhdl_files/miniCache.vhd}
vcom -93 -work work {C:/ECE/Aresia/TestProcGit/PROC-ECE_SYNC-main/vhdl_files/SDRAM_controller.vhd}
vcom -93 -work work {C:/ECE/Aresia/TestProcGit/PROC-ECE_SYNC-main/vhdl_files/SDRAM_32b.vhd}
vcom -93 -work work {C:/ECE/Aresia/TestProcGit/PROC-ECE_SYNC-main/vhdl_files/Top.vhd}
vcom -93 -work work {C:/ECE/Aresia/TestProcGit/PROC-ECE_SYNC-main/vhdl_files/RegisterFile.vhd}
vcom -93 -work work {C:/ECE/Aresia/TestProcGit/PROC-ECE_SYNC-main/vhdl_files/DEBUGER.vhd}

vcom -93 -work work {C:/ECE/Aresia/TestProcGit/PROC-ECE_SYNC-main/vhdl_files/TestBench.vhd}

vsim -t 1ps -L altera -L lpm -L sgate -L altera_mf -L altera_lnsim -L fiftyfivenm -L rtl_work -L work -voptargs="+acc"  TestBenchTop

do C:/ECE/Aresia/TestProcGit/PROC-ECE_SYNC-main/simulation/modelsim/my_custom_view.do
