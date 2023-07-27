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
	type mem is array(0 to 117) of std_logic_vector(31 downto 0);
	signal SigIMmemory : mem :=(
		x"37110000",
		x"ef00c000",
		x"73001000",
		x"6f000000",
		x"130101ff",
		x"23260100",
		x"23240100",
		x"23220100",
		x"37070080",
		x"b7070100",
		x"938737f8",
		x"2324f700",
		x"b787c0c0",
		x"9387f77f",
		x"2322f700",
		x"93068001",
		x"13050000",
		x"370700c0",
		x"930580ff",
		x"83270700",
		x"93f74700",
		x"e38c07fe",
		x"03264700",
		x"83270700",
		x"93f71700",
		x"e39c07fe",
		x"2322c700",
		x"3316d600",
		x"3365c500",
		x"938686ff",
		x"e39ab6fc",
		x"6304050e",
		x"37e3ffff",
		x"1303c3ff",
		x"33036500",
		x"93080500",
		x"b70700c0",
		x"37060080",
		x"930580ff",
		x"6f00800b",
		x"03a70700",
		x"13774700",
		x"e30c07fe",
		x"03a74700",
		x"2326e100",
		x"0327c100",
		x"13170701",
		x"2322e600",
		x"0327c100",
		x"03288100",
		x"3317d700",
		x"33670701",
		x"2324e100",
		x"938686ff",
		x"6380b602",
		x"03a70700",
		x"13774700",
		x"e31e07fa",
		x"03a70700",
		x"13772700",
		x"e30607fe",
		x"6ff0dffa",
		x"9388c8ff",
		x"03278100",
		x"2320ee00",
		x"23240100",
		x"2324e100",
		x"93068001",
		x"03278100",
		x"3357d700",
		x"1377f70f",
		x"2322e100",
		x"03a70700",
		x"13771700",
		x"e31c07fe",
		x"03274100",
		x"23a2e700",
		x"03274100",
		x"03284600",
		x"33670701",
		x"2322e600",
		x"938686ff",
		x"e394b6fc",
		x"638c0800",
		x"630a1301",
		x"330e1541",
		x"23240100",
		x"93068001",
		x"6ff0dff7",
		x"13080000",
		x"370700c0",
		x"37060080",
		x"130580ff",
		x"b7280000",
		x"6f00c000",
		x"13084800",
		x"630a1805",
		x"23240100",
		x"83270800",
		x"2324f100",
		x"93068001",
		x"83278100",
		x"b3d7d700",
		x"93f7f70f",
		x"2322f100",
		x"83270700",
		x"93f71700",
		x"e39c07fe",
		x"83274100",
		x"2322f700",
		x"83274100",
		x"83254600",
		x"b3e7b700",
		x"2322f600",
		x"938686ff",
		x"e394a6fc",
		x"6ff0dffa",
		x"6f000000"
	);
	
	signal sigad : integer;
	signal sigpc : std_logic_vector(2 downto 0);
	
begin

	sigpc <= IMprogcounter(4 downto 2);
	IMout <= SigIMmemory(sigad) when rising_edge(IMclock);
	Sigad <= 0 when ((unsigned(sigpc) > 117)) else to_integer(unsigned(sigpc));

end archi;
-- END FILE