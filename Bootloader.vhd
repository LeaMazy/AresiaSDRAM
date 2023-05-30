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
	TYPE ROM IS ARRAY(0 TO 50) OF std_logic_vector(0 to 31);
	SIGNAL rom_block : ROM :=(
--		x"00001137" , x"00c000ef" , x"00100073" , x"0000006f" , x"000087b7" , x"80000737" , x"3c078793" , x"00f72423",
--		x"c08807b7" , x"fff78793" , x"00f72223" , x"0000006f"

		x"00001137" , x"00c000ef" , x"00100073" , x"0000006f" , x"000107b7" , x"80000737" , x"fff78793" , x"00f72423",
		x"83c0c7b7" , x"08778793" , x"00f72223" , x"00000613" , x"01800793" , x"c00006b7" , x"ff800513" , x"0006a703",
		x"00477713" , x"fe070ce3" , x"0046a703" , x"0006a583" , x"0015f593" , x"fe059ce3" , x"00e6a223" , x"00f71733",
		x"ff878793" , x"00e66633" , x"fca79ae3" , x"00000793" , x"800008b7" , x"c0000737" , x"ff800313" , x"00c78463",
		x"01179463" , x"0000006f" , x"00000593" , x"01800693" , x"00072503" , x"00457513" , x"fe050ce3" , x"00472803",
		x"00d81533" , x"00a5e5b3" , x"00072503" , x"00157513" , x"fe051ce3" , x"01072223" , x"ff868693" , x"fc669ae3",
		x"00b7a023" , x"00478793" , x"fb5ff06f"

--		x"00001137" , x"00c000ef" , x"00100073" , x"0000006f" , x"00400713" , x"c00007b7" , x"0007a683" , x"0046f693",
--		x"fe068ce3" , x"0047a603" , x"0007a683" , x"0016f693" , x"fe069ce3" , x"00c7a223" , x"fff70713" , x"fc071ee3",
--		x"0000006f"
	);
	
	signal sigad : integer;
	signal sigpc : std_logic_vector(11 downto 0);
	
	begin
		sigpc <= addrInstBoot(11 downto 0);
		instBoot <= rom_block(sigad) when rising_edge(clk);
		sigad <= 50 when ((unsigned(sigpc) > 50)) else to_integer(unsigned(sigpc));
	
end archi;
-- END FILE