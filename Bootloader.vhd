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
	TYPE ROM IS ARRAY(0 TO 82) OF std_logic_vector(0 to 31);

	SIGNAL rom_block : ROM :=(
		x"00001137" , x"00c000ef" , x"00100073" , x"0000006f" , x"ff010113" , x"00012623" , x"00012423" , x"00012223",
		x"80000737" , x"000107b7" , x"fff78793" , x"00f72423" , x"83c0c7b7" , x"08778793" , x"00f72223" , x"01800613",
		x"00000893" , x"c0000737" , x"ff800593" , x"00072783" , x"0047f793" , x"fe078ce3" , x"00472683" , x"00072783",
		x"0017f793" , x"fe079ce3" , x"00d72223" , x"00c696b3" , x"00d8e8b3" , x"ff860613" , x"fcb61ae3" , x"0a088263",
		x"ffffe337" , x"00688333" , x"00088513" , x"c0000737" , x"ff800613" , x"00c0006f" , x"08050463" , x"08a30263",
		x"40a88833" , x"00012423" , x"01800693" , x"00072783" , x"0047f793" , x"fe078ce3" , x"00472783" , x"00f12623",
		x"00c12783" , x"00812583" , x"00d797b3" , x"00b7e7b3" , x"00f12423" , x"ff868693" , x"fcc69ae3" , x"ffc50513",
		x"00812783" , x"00f82023" , x"00f12423" , x"01800693" , x"00812783" , x"00d7d7b3" , x"0ff7f793" , x"00f12223",
		x"00072783" , x"0017f793" , x"fe079ce3" , x"00412783" , x"00f72223" , x"ff868693" , x"fcc69ce3" , x"f7dff06f",
		x"00000693" , x"c0000737" , x"00002637" , x"00072783" , x"0017f793" , x"fe079ce3" , x"0006c783" , x"00f72223",
		x"00168693" , x"fec694e3" , x"0000006f"
	);
	
	signal sigad : integer;
	signal sigpc : std_logic_vector(11 downto 0) := "000000000000";
	
	begin
		sigpc <= addrInstBoot(11 downto 0);
		instBoot <= rom_block(sigad) when rising_edge(clk);
		sigad <= 0 when (CS ='1' and sigpc=x"FFF") else 
					(to_integer(unsigned(sigpc))) when (CS ='1' and (unsigned(sigpc) < 83)) else 
					82;
			
end archi;
-- END FILE