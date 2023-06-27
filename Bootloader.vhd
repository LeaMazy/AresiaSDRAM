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
--	TYPE ROM IS ARRAY(0 TO 11) OF std_logic_vector(0 to 31);
--	TYPE ROM IS ARRAY(0 TO 20) OF std_logic_vector(0 to 31);
--	TYPE ROM IS ARRAY(0 TO 23) OF std_logic_vector(0 to 31);
	TYPE ROM IS ARRAY(0 TO 71) OF std_logic_vector(0 to 31);

	SIGNAL rom_block : ROM :=(
		--71 
		x"00001137" , x"00c000ef" , x"00100073" , x"0000006f" , x"000107b7" , x"80000737" , x"f8378793" , x"00f72423",
		x"c0c087b7" , x"7ff78793" , x"00f72223" , x"00000513" , x"01800793" , x"c00006b7" , x"ff800593" , x"0006a703",
		x"00477713" , x"fe070ce3" , x"0046a703" , x"0006a603" , x"00167613" , x"fe061ce3" , x"00e6a223" , x"00f71733",
		x"ff878793" , x"00e56533" , x"fcb79ae3" , x"000015b7" , x"00000713" , x"ffc58593" , x"c00007b7" , x"ff800313",
		x"00a70463" , x"02b71a63" , x"00001737" , x"00000793" , x"c0000637" , x"ffc70713" , x"00062683" , x"0016f693",
		x"fe069ce3" , x"0007c683" , x"00178793" , x"00d62223" , x"fee794e3" , x"0000006f" , x"00000613" , x"01800693",
		x"0007a803" , x"00487813" , x"fe080ce3" , x"0047a883" , x"00d89833" , x"01066633" , x"0007a803" , x"00187813",
		x"fe081ce3" , x"0117a223" , x"ff868693" , x"fc669ae3" , x"00c72023" , x"01800693" , x"00d65833" , x"0ff87813",
		x"0007a883" , x"0018f893" , x"fe089ce3" , x"0107a223" , x"ff868693" , x"fe6692e3" , x"00470713" , x"f65ff06f"
	
	
	--73
--		x"00001137" , x"064000ef" , x"00100073" , x"0000006f" , x"ff010113" , x"00012623" , x"00000793" , x"00a7c863",
--		x"00012623" , x"01010113" , x"00008067" , x"00c12703" , x"00178793" , x"00170713" , x"00e12623" , x"fe1ff06f",
--		x"fe050513" , x"0ff57513" , x"03f00793" , x"00a7ea63" , x"0e800793" , x"00a787b3" , x"0007c503" , x"00008067",
--		x"07f00513" , x"00008067" , x"ff010113" , x"00112623" , x"00812423" , x"00912223" , x"01212023" , x"800007b7",
--		x"0107a703" , x"20077713" , x"fe070ce3" , x"00010737" , x"fff70713" , x"00e7a423" , x"83c0c737" , x"08770713",
--		x"00e7a223" , x"0e000413" , x"04500493" , x"80000937" , x"00044783" , x"00040713" , x"00140413" , x"00979463",
--		x"0000006f" , x"00044783" , x"0ff7f793" , x"00f70023" , x"00074503" , x"f6dff0ef" , x"00a92223" , x"fd5ff06f",
--		x"44434241" , x"00000045" , x"7f7f7fff" , x"7f7f7f7f" , x"7f7f7f7f" , x"7f7fbf7f" , x"b0a4f9c0" , x"f8829299",
--		x"7f7f9080" , x"7f7f7f7f" , x"c683887f" , x"c28e86a1" , x"8ae1f98b" , x"c0abaac7" , x"92ce988c" , x"95b5c187",
--		x"7fa49189" , x"f77f7f7f" 
	
	
	
	--52
--		x"00001137" , x"064000ef" , x"00100073" , x"0000006f" , x"ff010113" , x"00012623" , x"00000793" , x"00a7c863",
--		x"00012623" , x"01010113" , x"00008067" , x"00c12703" , x"00178793" , x"00170713" , x"00e12623" , x"fe1ff06f",
--		x"fe050513" , x"0ff57513" , x"03f00793" , x"00a7ea63" , x"09400793" , x"00a787b3" , x"0007c503" , x"00008067",
--		x"07f00513" , x"00008067" , x"800007b7" , x"0107a703" , x"20077713" , x"fe070ce3" , x"00010737" , x"fff70713",
--		x"00e7a423" , x"83c0c737" , x"08770713" , x"00e7a223" , x"0000006f" , x"7f7f7fff" , x"7f7f7f7f" , x"7f7f7f7f",
--		x"7f7fbf7f" , x"b0a4f9c0" , x"f8829299" , x"7f7f9080" , x"7f7f7f7f" , x"c683887f" , x"c28e86a1" , x"8ae1f98b",
--		x"c0abaac7" , x"92ce988c" , x"95b5c187" , x"7fa49189" , x"f77f7f7f"
--	
--		x"00001137" , x"198000ef" , x"00100073" , x"0000006f" , x"ff010113" , x"00012623" , x"00a05e63" , x"00000713",
--		x"00c12783" , x"00178793" , x"00f12623" , x"00170713" , x"fee518e3" , x"00012623" , x"01010113" , x"00008067",
--		x"fe050513" , x"0ff57713" , x"03f00793" , x"14e7e063" , x"00271513" , x"25000793" , x"00f50533" , x"00052783",
--		x"00078067" , x"0f900513" , x"00008067" , x"0a400513" , x"00008067" , x"0b000513" , x"00008067" , x"09900513",
--		x"00008067" , x"09200513" , x"00008067" , x"08200513" , x"00008067" , x"0f800513" , x"00008067" , x"08000513",
--		x"00008067" , x"09000513" , x"00008067" , x"08800513" , x"00008067" , x"08300513" , x"00008067" , x"0c600513",
--		x"00008067" , x"0a100513" , x"00008067" , x"08600513" , x"00008067" , x"08e00513" , x"00008067" , x"0c200513",
--		x"00008067" , x"08b00513" , x"00008067" , x"0e100513" , x"00008067" , x"08a00513" , x"00008067" , x"0c700513",
--		x"00008067" , x"0aa00513" , x"00008067" , x"0ab00513" , x"00008067" , x"08c00513" , x"00008067" , x"09800513",
--		x"00008067" , x"0ce00513" , x"00008067" , x"09200513" , x"00008067" , x"08700513" , x"00008067" , x"0c100513",
--		x"00008067" , x"0b500513" , x"00008067" , x"09500513" , x"00008067" , x"08900513" , x"00008067" , x"09100513",
--		x"00008067" , x"0a400513" , x"00008067" , x"0ff00513" , x"00008067" , x"07f00513" , x"00008067" , x"0bf00513",
--		x"00008067" , x"0f700513" , x"00008067" , x"07f00513" , x"00008067" , x"0c000513" , x"00008067" , x"fd010113",
--		x"02112623" , x"02812423" , x"02912223" , x"03212023" , x"01312e23" , x"80000737" , x"000107b7" , x"fff78793",
--		x"00f72423" , x"83c0c7b7" , x"08778793" , x"00f72223" , x"444347b7" , x"24178793" , x"00f12223" , x"484747b7",
--		x"64578793" , x"00f12423" , x"4c4b57b7" , x"a4978793" , x"00f12623" , x"00414783" , x"0ff7f793" , x"04c00713",
--		x"04e78663" , x"00000413" , x"800009b7" , x"04c00913" , x"00040793" , x"00140413" , x"01040713" , x"002704b3",
--		x"ff44c703" , x"0ff77713" , x"01078793" , x"002787b3" , x"fee78a23" , x"ff47c503" , x"e09ff0ef" , x"00a9a223",
--		x"ff44c783" , x"0ff7f793" , x"fd2794e3" , x"0000006f" , x"0000016c" , x"0000018c" , x"0000018c" , x"0000018c",
--		x"0000018c" , x"0000018c" , x"0000018c" , x"0000018c" , x"0000018c" , x"0000018c" , x"0000018c" , x"0000018c",
--		x"0000018c" , x"0000017c" , x"00000174" , x"0000018c" , x"00000194" , x"00000064" , x"0000006c" , x"00000074",
--		x"0000007c" , x"00000084" , x"0000008c" , x"00000094" , x"0000009c" , x"000000a4" , x"0000018c" , x"0000018c",
--		x"0000018c" , x"0000018c" , x"0000018c" , x"0000018c" , x"0000018c" , x"000000ac" , x"000000b4" , x"000000bc",
--		x"000000c4" , x"000000cc" , x"000000d4" , x"000000dc" , x"000000e4" , x"00000064" , x"000000ec" , x"000000f4",
--		x"000000fc" , x"00000104" , x"0000010c" , x"00000194" , x"00000114" , x"0000011c" , x"00000124" , x"0000012c",
--		x"00000134" , x"0000013c" , x"00000144" , x"0000014c" , x"00000154" , x"0000015c" , x"00000164" , x"0000018c",
--		x"0000018c" , x"0000018c" , x"0000018c" , x"00000184"
--	
	
	--57
--		x"00001137" , x"00c000ef" , x"00100073" , x"0000006f" , x"000107b7" , x"80000737" , x"fff78793" , x"00f72423",
--		x"83c0c7b7" , x"08778793" , x"00f72223" , x"c0000637" , x"01800713" , x"00000793" , x"ff800513" , x"00062683",
--		x"0046f693" , x"fe068ce3" , x"00462683" , x"00062583" , x"0015f593" , x"fe059ce3" , x"00d62223" , x"00e696b3",
--		x"ff870713" , x"00d7e7b3" , x"fca71ae3" , x"fffff737" , x"00470713" , x"00e78733" , x"c00006b7" , x"00078463",
--		x"02e79a63" , x"00001737" , x"00000793" , x"c0000637" , x"ffc70713" , x"00062683" , x"0016f693" , x"fe069ce3",
--		x"0007c683" , x"00178793" , x"00d62223" , x"fee794e3" , x"0000006f" , x"00400613" , x"0006a583" , x"0045f593",
--		x"fe058ce3" , x"0046a503" , x"0006a583" , x"0015f593" , x"fe059ce3" , x"00a6a223" , x"fff60613" , x"fc061ee3",
--		x"ffc78793" , x"f99ff06f" 
		
--		71
--		x"00001137" , x"00c000ef" , x"00100073" , x"0000006f" , x"80000737" , x"000107b7" , x"fff78793" , x"00f72423",
--		x"83c0c7b7" , x"08778793" , x"00f72223" , x"01800693" , x"00000313" , x"c0000737" , x"ff800613" , x"00072783",
--		x"0047f793" , x"fe078ce3" , x"00472783" , x"00072503" , x"00157513" , x"fe051ce3" , x"00f72223" , x"00d797b3",
--		x"00f36333" , x"ff868693" , x"fcc69ae3" , x"08030263" , x"fffffe37" , x"004e0e13" , x"01c30e33" , x"00030893",
--		x"c0000737" , x"ff800813" , x"00c0006f" , x"06088263" , x"071e0063" , x"41130633" , x"00050593" , x"01800693",
--		x"00072783" , x"0047f793" , x"fe078ce3" , x"00472783" , x"00d797b3" , x"00f5e5b3" , x"ff868693" , x"ff0692e3",
--		x"ffc88893" , x"00b62023" , x"01800693" , x"00d5d633" , x"0ff67613" , x"00072783" , x"0017f793" , x"fe079ce3",
--		x"00c72223" , x"ff868693" , x"ff0692e3" , x"fa1ff06f" , x"00000693" , x"c0000737" , x"00001637" , x"ffc60613",
--		x"00072783" , x"0017f793" , x"fe079ce3" , x"0006c783" , x"00f72223" , x"00168693" , x"fec694e3" , x"0000006f"
		
--		x"00001137" , x"00c000ef" , x"00100073" , x"0000006f" , x"80000737" , x"000107b7" , x"fff78793" , x"00f72423",
--		x"83c0c7b7" , x"08778793" , x"00f72223" , x"0000006f"

--		x"00001137" , x"00c000ef" , x"00100073" , x"0000006f" , x"80000737" , x"000107b7" , x"fff78793" , x"00f72423",
--		x"83c0c7b7" , x"08778793" , x"00f72223" , x"00400693" , x"c0000737" , x"00072783" , x"0047f793" , x"fe078ce3",
--		x"00472603" , x"00072783" , x"0017f793" , x"fe079ce3" , x"00c72223" , x"fff68693" , x"fc069ee3" , x"0000006f"

--		x"00001137" , x"00c000ef" , x"00100073" , x"0000006f" , x"80000737" , x"000107b7" , x"fff78793" , x"00f72423",
--		x"83c0c7b7" , x"08778793" , x"00f72223" , x"c0000737" , x"00072783" , x"0047f793" , x"fe078ce3" , x"00472683",
--		x"00072783" , x"0017f793" , x"fe079ce3" , x"00d72223" , x"fe1ff06f"
	);
	
	signal sigad : integer;
	signal sigpc : std_logic_vector(11 downto 0) := "000000000000";
	
	begin
		sigpc <= addrInstBoot(11 downto 0);
		instBoot <= rom_block(sigad) when rising_edge(clk);
		sigad <= 0 when (CS ='1' and sigpc=x"FFF") else 
					(to_integer(unsigned(sigpc))) when (CS ='1' and (unsigned(sigpc) < 72)) else 
					71;
	
end archi;
-- END FILE