-- Projet de fin d'études : RISC-V
-- ECE Paris / SECAPEM

-- LIBRARIES
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.numeric_std.ALL;
USE work.simulPkg.ALL;

-- ENTITY
ENTITY debuger IS
	PORT (
		-- INPUTS
		enable 		: IN STD_LOGIC;
		SW8, SW7, SW6, SW5, SW4, SW3: IN STD_LOGIC; --PC, instruction, RF (RFin, RFout1, RFout2)
		--reset    	: IN STD_LOGIC; --SW0
		PCregister  : IN STD_LOGIC_VECTOR(15 downto 0);
		Instruction : IN STD_LOGIC_VECTOR(31 downto 0);
 	   RFin			: IN STD_LOGIC_VECTOR(31 downto 0);
		RFout1		: IN STD_LOGIC_VECTOR(31 downto 0);
	   RFout2		: IN STD_LOGIC_VECTOR(31 downto 0);

		
		--OUTPUTS
		TOPdisplay2 : OUT STD_LOGIC_VECTOR(31 DOWNTO 0) := (others => '1'); --0x80000004
		TOPdisplay1 : OUT STD_LOGIC_VECTOR(31 DOWNTO 0) := (others => '1')  --0x80000004

	);
END ENTITY;	

ARCHITECTURE archi OF debuger IS

SIGNAL display1, display2, display3, display4, display5, display6 : STD_LOGIC_VECTOR(3 downto 0);
SIGNAL SW : STD_LOGIC_VECTOR(5 downto 0);

COMPONENT SegmentDecoder IS
	PORT (
		Sin7seg  : in  STD_LOGIC_VECTOR(3 downto 0);
		decodeOut : out  STD_LOGIC_VECTOR(7 downto 0));
END COMPONENT;


begin
SW <= SW8 & SW7 & SW6 & SW5 & SW4 & SW3;

display1 <= PCregister(15 downto 12) when enable='1' AND SW="100000" else
				Instruction(31 downto 28) when enable='1' AND SW="000000" else
				Instruction(23 downto 20) when enable='1' AND SW="010000" else
				RFin(31 downto 28) when enable='1' AND SW="001000" else
				RFin(23 downto 20) when enable='1' AND SW="001001" else
				RFout1(31 downto 28) when enable='1' AND SW="001010" else
				RFout1(23 downto 20) when enable='1' AND SW="001011" else
				RFout2(31 downto 28) when enable='1' AND SW="001100" else
				RFout2(23 downto 20) when enable='1' AND SW="001101" else
				x"f";
				
display2 <= PCregister(11 downto 8) when enable='1' AND SW="100000" else
				Instruction(27 downto 24) when enable='1' AND SW="000000" else
				Instruction(19 downto 16) when enable='1' AND SW="010000" else
				RFin(27 downto 24) when enable='1' AND SW="001000" else
				RFin(19 downto 16) when enable='1' AND SW="001001" else
				RFout1(27 downto 24) when enable='1' AND SW="001010" else
				RFout1(19 downto 16) when enable='1' AND SW="001011" else
				RFout2(27 downto 24) when enable='1' AND SW="001100" else
				RFout2(19 downto 16) when enable='1' AND SW="001101" else
				x"f";
				
display3 <= PCregister(7 downto 4) when enable='1' AND SW="100000" else
				Instruction(23 downto 20) when enable='1' AND SW="000000" else
				Instruction(15 downto 12) when enable='1' AND SW="010000" else
				RFin(23 downto 20) when enable='1' AND SW="001000" else
				RFin(15 downto 12) when enable='1' AND SW="001001" else
				RFout1(23 downto 20) when enable='1' AND SW="001010" else
				RFout1(15 downto 12) when enable='1' AND SW="001011" else
				RFout2(23 downto 20) when enable='1' AND SW="001100" else
				RFout2(15 downto 12) when enable='1' AND SW="001101" else
				x"f";
				
display4 <= PCregister(3 downto 0) when enable='1' AND SW="100000" else
				Instruction(19 downto 16) when enable='1' AND SW="000000" else
				Instruction(11 downto 8) when enable='1' AND SW="010000" else
				RFin(19 downto 16) when enable='1' AND SW="001000" else
				RFin(11 downto 8) when enable='1' AND SW="001001" else
				RFout1(19 downto 16) when enable='1' AND SW="001010" else
				RFout1(11 downto 8) when enable='1' AND SW="001011" else
				RFout2(19 downto 16) when enable='1' AND SW="001100" else
				RFout2(11 downto 8) when enable='1' AND SW="001101" else
				x"f";
				
display5 <= x"f" when enable='1' AND SW="100000" else
				Instruction(15 downto 12) when enable='1' AND SW="000000" else
				Instruction(7 downto 4) when enable='1' AND SW="010000" else
				RFin(15 downto 12) when enable='1' AND SW="001000" else
				RFin(7 downto 4) when enable='1' AND SW="001001" else
				RFout1(15 downto 12) when enable='1' AND SW="001010" else
				RFout1(7 downto 4) when enable='1' AND SW="001011" else
				RFout2(15 downto 12) when enable='1' AND SW="001100" else
				RFout2(7 downto 4) when enable='1' AND SW="001101" else
				x"f";
				
display6 <= x"f" when enable='1' AND SW="100000" else
				Instruction(11 downto 8) when enable='1' AND SW="000000" else
				Instruction(3 downto 0) when enable='1' AND SW="010000" else
				RFin(11 downto 8) when enable='1' AND SW="001000" else
				RFin(3 downto 0) when enable='1' AND SW="001001" else
				RFout1(11 downto 8) when enable='1' AND SW="001010" else
				RFout1(3 downto 0) when enable='1' AND SW="001011" else
				RFout2(11 downto 8) when enable='1' AND SW="001100" else
				RFout2(3 downto 0) when enable='1' AND SW="001101" else
				x"f";

decoder1 : SegmentDecoder
PORT MAP(
	Sin7seg		=>	display1,
	decodeOut	=> TOPdisplay2(15 downto 8)
);
decoder2 : SegmentDecoder
PORT MAP(
	Sin7seg		=> display2,
	decodeOut	=> TOPdisplay2(7 downto 0)
);
decoder3 : SegmentDecoder
PORT MAP(
	Sin7seg		=> display3,
	decodeOut	=> TOPdisplay1(31 downto 24)
);
decoder4 : SegmentDecoder
PORT MAP(
	Sin7seg		=> display4,
	decodeOut	=> TOPdisplay1(23 downto 16)
);
decoder5 : SegmentDecoder
PORT MAP(
	Sin7seg		=> display5,
	decodeOut	=> TOPdisplay1(15 downto 8)
);
decoder6 : SegmentDecoder
PORT MAP(
	Sin7seg		=> display6,
	decodeOut	=> TOPdisplay1(7 downto 0)
);
	
END archi;
-- END FILE

