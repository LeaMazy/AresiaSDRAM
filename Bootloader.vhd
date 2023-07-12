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
	TYPE ROM IS ARRAY(0 TO 97) OF std_logic_vector(0 to 31);

	SIGNAL rom_block : ROM :=(
		x"00001137" , x"03c000ef" , x"00100073" , x"0000006f" , x"ff010113" , x"00012623" , x"00000793" , x"00a7c863",
		x"00012623" , x"01010113" , x"00008067" , x"00c12703" , x"00178793" , x"00170713" , x"00e12623" , x"fe1ff06f",
		x"ff010113" , x"00012223" , x"00012423" , x"000107b7" , x"00012623" , x"80000737" , x"f8378793" , x"00f72423",
		x"c0c087b7" , x"7ff78793" , x"00f72223" , x"00000513" , x"01800793" , x"c00006b7" , x"ff800593" , x"0006a703",
		x"00477713" , x"fe070ce3" , x"0046a703" , x"0006a603" , x"00167613" , x"fe061ce3" , x"00e6a223" , x"00f71733",
		x"ff878793" , x"00e56533" , x"fcb79ae3" , x"000025b7" , x"00000713" , x"00458593" , x"c00007b7" , x"80000837",
		x"ff800893" , x"00012423" , x"00a70463" , x"00b71663" , x"00002783" , x"00100073" , x"01800613" , x"0007a683",
		x"0046f693" , x"00069863" , x"0007a683" , x"0026f693" , x"fe0686e3" , x"0007a683" , x"0046f693" , x"fe068ce3",
		x"0047a683" , x"00d12223" , x"00412683" , x"01069693" , x"00d82223" , x"00412683" , x"00812303" , x"00c696b3",
		x"0066e6b3" , x"00d12423" , x"ff860613" , x"fb1618e3" , x"00812683" , x"00012423" , x"01800613" , x"00d72023",
		x"00d12423" , x"00812683" , x"00c6d6b3" , x"0ff6f693" , x"00d12623" , x"0007a683" , x"0016f693" , x"fe069ce3",
		x"00c12683" , x"ff860613" , x"00d7a223" , x"00c12683" , x"00482303" , x"0066e6b3" , x"00d82223" , x"fd1614e3",
		x"00470713" , x"f41ff06f"
		
	);
	
	signal sigad : integer;
	signal sigpc : std_logic_vector(11 downto 0) := "000000000000";
	
	begin
		sigpc <= addrInstBoot(11 downto 0);
		instBoot <= rom_block(sigad) when rising_edge(clk);
		sigad <= 0 when (CS ='1' and sigpc=x"FFF") else 
					(to_integer(unsigned(sigpc))) when (CS ='1' and (unsigned(sigpc) < 98)) else 
					97;
			
end archi;
-- END FILE