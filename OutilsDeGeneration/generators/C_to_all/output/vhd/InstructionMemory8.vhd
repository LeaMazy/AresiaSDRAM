-- Projet de fin d'études : RISC-V
-- ECE Paris / SECAPEM
-- Instruction Memory VHDL

-- LIBRARIES
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

-- ENTITY
entity InstructionMemory is
	port (
		-- INPUTS
		IMclock			: in std_logic;
		IMreset			: in std_logic;
		IMprogcounter	: in std_logic_vector(31 downto 0);
		-- OUTPUTS
		IMout				: out std_logic_vector(31 downto 0)
	);
end entity;

-- ARCHITECTURE
architecture archi of InstructionMemory is
	type mem is array(0 to 471) of std_logic_vector(7 downto 0);
	signal SigIMmemory : mem :=(
		x"37",x"11",x"00",x"00",  x"ef",x"00",x"c0",x"00",  x"73",x"00",x"10",x"00",  x"6f",x"00",x"00",x"00",  x"13",x"01",x"01",x"ff",  x"23",x"26",x"01",x"00",  x"23",x"24",x"01",x"00",  x"23",x"22",x"01",x"00",
		x"37",x"07",x"00",x"80",  x"b7",x"07",x"01",x"00",  x"93",x"87",x"37",x"f8",  x"23",x"24",x"f7",x"00",  x"b7",x"87",x"c0",x"c0",  x"93",x"87",x"f7",x"7f",  x"23",x"22",x"f7",x"00",  x"93",x"06",x"80",x"01",
		x"13",x"05",x"00",x"00",  x"37",x"07",x"00",x"c0",  x"93",x"05",x"80",x"ff",  x"83",x"27",x"07",x"00",  x"93",x"f7",x"47",x"00",  x"e3",x"8c",x"07",x"fe",  x"03",x"26",x"47",x"00",  x"83",x"27",x"07",x"00",
		x"93",x"f7",x"17",x"00",  x"e3",x"9c",x"07",x"fe",  x"23",x"22",x"c7",x"00",  x"33",x"16",x"d6",x"00",  x"33",x"65",x"c5",x"00",  x"93",x"86",x"86",x"ff",  x"e3",x"9a",x"b6",x"fc",  x"63",x"04",x"05",x"0e",
		x"37",x"e3",x"ff",x"ff",  x"13",x"03",x"c3",x"ff",  x"33",x"03",x"65",x"00",  x"93",x"08",x"05",x"00",  x"b7",x"07",x"00",x"c0",  x"37",x"06",x"00",x"80",  x"93",x"05",x"80",x"ff",  x"6f",x"00",x"80",x"0b",
		x"03",x"a7",x"07",x"00",  x"13",x"77",x"47",x"00",  x"e3",x"0c",x"07",x"fe",  x"03",x"a7",x"47",x"00",  x"23",x"26",x"e1",x"00",  x"03",x"27",x"c1",x"00",  x"13",x"17",x"07",x"01",  x"23",x"22",x"e6",x"00",
		x"03",x"27",x"c1",x"00",  x"03",x"28",x"81",x"00",  x"33",x"17",x"d7",x"00",  x"33",x"67",x"07",x"01",  x"23",x"24",x"e1",x"00",  x"93",x"86",x"86",x"ff",  x"63",x"80",x"b6",x"02",  x"03",x"a7",x"07",x"00",
		x"13",x"77",x"47",x"00",  x"e3",x"1e",x"07",x"fa",  x"03",x"a7",x"07",x"00",  x"13",x"77",x"27",x"00",  x"e3",x"06",x"07",x"fe",  x"6f",x"f0",x"df",x"fa",  x"93",x"88",x"c8",x"ff",  x"03",x"27",x"81",x"00",
		x"23",x"20",x"ee",x"00",  x"23",x"24",x"01",x"00",  x"23",x"24",x"e1",x"00",  x"93",x"06",x"80",x"01",  x"03",x"27",x"81",x"00",  x"33",x"57",x"d7",x"00",  x"13",x"77",x"f7",x"0f",  x"23",x"22",x"e1",x"00",
		x"03",x"a7",x"07",x"00",  x"13",x"77",x"17",x"00",  x"e3",x"1c",x"07",x"fe",  x"03",x"27",x"41",x"00",  x"23",x"a2",x"e7",x"00",  x"03",x"27",x"41",x"00",  x"03",x"28",x"46",x"00",  x"33",x"67",x"07",x"01",
		x"23",x"22",x"e6",x"00",  x"93",x"86",x"86",x"ff",  x"e3",x"94",x"b6",x"fc",  x"63",x"8c",x"08",x"00",  x"63",x"0a",x"13",x"01",  x"33",x"0e",x"15",x"41",  x"23",x"24",x"01",x"00",  x"93",x"06",x"80",x"01",
		x"6f",x"f0",x"df",x"f7",  x"13",x"08",x"00",x"00",  x"37",x"07",x"00",x"c0",  x"37",x"06",x"00",x"80",  x"13",x"05",x"80",x"ff",  x"b7",x"28",x"00",x"00",  x"6f",x"00",x"c0",x"00",  x"13",x"08",x"48",x"00",
		x"63",x"0a",x"18",x"05",  x"23",x"24",x"01",x"00",  x"83",x"27",x"08",x"00",  x"23",x"24",x"f1",x"00",  x"93",x"06",x"80",x"01",  x"83",x"27",x"81",x"00",  x"b3",x"d7",x"d7",x"00",  x"93",x"f7",x"f7",x"0f",
		x"23",x"22",x"f1",x"00",  x"83",x"27",x"07",x"00",  x"93",x"f7",x"17",x"00",  x"e3",x"9c",x"07",x"fe",  x"83",x"27",x"41",x"00",  x"23",x"22",x"f7",x"00",  x"83",x"27",x"41",x"00",  x"83",x"25",x"46",x"00",
		x"b3",x"e7",x"b7",x"00",  x"23",x"22",x"f6",x"00",  x"93",x"86",x"86",x"ff",  x"e3",x"94",x"a6",x"fc",  x"6f",x"f0",x"df",x"fa",  x"6f",x"00",x"00",x"00"
	);
	
	signal SigIMprogcounter00 : integer;
	signal SigIMprogcounter08 : integer;
	signal SigIMprogcounter16 : integer;
	signal SigIMprogcounter24 : integer;
	
begin

	SigIMprogcounter00 <= 0 when ((unsigned(IMprogcounter) > 471) OR IMreset = '1') else to_integer(unsigned(IMprogcounter));
	SigIMprogcounter08 <= 0 when ((unsigned(IMprogcounter) > 471) OR IMreset = '1') else to_integer(unsigned(IMprogcounter) + 1);
	SigIMprogcounter16 <= 0 when ((unsigned(IMprogcounter) > 471) OR IMreset = '1') else to_integer(unsigned(IMprogcounter) + 2);
	SigIMprogcounter24 <= 0 when ((unsigned(IMprogcounter) > 471) OR IMreset = '1') else to_integer(unsigned(IMprogcounter) + 3);

	IMout(7 downto 0)   <= SigIMmemory(SigIMprogcounter00) when (SigIMprogcounter00 >= 0) else x"00";
	IMout(15 downto 8)  <= SigIMmemory(SigIMprogcounter08) when (SigIMprogcounter08 >= 0) else x"00";
	IMout(23 downto 16) <= SigIMmemory(SigIMprogcounter16) when (SigIMprogcounter16 >= 0) else x"00";
	IMout(31 downto 24) <= SigIMmemory(SigIMprogcounter24) when (SigIMprogcounter24 >= 0) else x"00";	

end archi;
-- END FILE