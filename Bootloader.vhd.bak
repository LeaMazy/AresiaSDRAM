-- Projet de stage ING4 : RISC-V
-- ECE Paris / ARESIA
-- BOOTLOADER VHDL

-- LIBRARIES
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

-- ENTITY
entity Bootloader is
	port (
		--INPUTS
		clk 					: in std_logic;
		CS 					: in std_logic; 							--chip select
		addrInstBoot		: in std_logic_vector(11 downto 0); --addr of boot instruction
		--OUTPUT
		instBoot				: out std_logic_vector(31 downto 0)    --output boot instruction
	);
end entity;

-- ARCHITECTURE
architecture archi of Bootloader is
	TYPE ROM IS ARRAY(0 TO 71) OF std_logic_vector(0 to 31);
	SIGNAL rom_block : ROM :=(
		x"00001137" , x"00c000ef" , x"00100073" , x"0000006f" , x"80000737" , x"000107b7" , x"fff78793" , x"00f72423",
		x"83c0c7b7" , x"08778793" , x"00f72223" , x"01800693" , x"00000313" , x"c0000737" , x"ff800613" , x"00072783",
		x"0047f793" , x"fe078ce3" , x"00472783" , x"00072503" , x"00157513" , x"fe051ce3" , x"00f72223" , x"00d797b3",
		x"00f36333" , x"ff868693" , x"fcc69ae3" , x"08030263" , x"fffffe37" , x"004e0e13" , x"01c30e33" , x"00030893",
		x"c0000737" , x"ff800813" , x"00c0006f" , x"06088263" , x"071e0063" , x"41130633" , x"00050593" , x"01800693",
		x"00072783" , x"0047f793" , x"fe078ce3" , x"00472783" , x"00d797b3" , x"00f5e5b3" , x"ff868693" , x"ff0692e3",
		x"ffc88893" , x"00b62023" , x"01800693" , x"00d5d633" , x"0ff67613" , x"00072783" , x"0017f793" , x"fe079ce3",
		x"00c72223" , x"ff868693" , x"ff0692e3" , x"fa1ff06f" , x"00000693" , x"c0000737" , x"00001637" , x"ffc60613",
		x"00072783" , x"0017f793" , x"fe079ce3" , x"0006c783" , x"00f72223" , x"00168693" , x"fec694e3" , x"0000006f"
	);
	
	signal sigad : integer;
	signal sigpc : std_logic_vector(11 downto 0);
	
	begin
		sigpc <= addrInstBoot(11 downto 0);
		instBoot <= rom_block(sigad) when rising_edge(clk);
		sigad <= 71 when ((unsigned(sigpc) > 71)) else to_integer(unsigned(sigpc));
	
end archi;
-- END FILE