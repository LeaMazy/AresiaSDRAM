-- Projet : RISC-V
-- ECE Paris / ARESIA
-- GPIO VHDL

-- LIBRARIES
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

-- ENTITY
entity GPIO is
	port (
		--INPUTS from TOP
		GPIOclock : in std_logic;
		GPIOreset : in std_logic;
		GPIOsw9,GPIOsw8,GPIOsw7,GPIOsw6,GPIOsw5	 : IN    STD_LOGIC; -- inputs for debuger (SW9 - SW5)
		GPIOsw4,GPIOsw3,GPIOsw2,GPIOsw1,GPIOsw0	 : IN    STD_LOGIC; -- inputs for debuger (SW4 - SW3), boot (SW2), hold (SW1) and reset (SW0)
		GPIOkey1, GPIOkey0								 : IN    STD_LOGIC; -- debug clock button (KEY0) & 2nd button (KEY1)
		--INPUTS from PROC
		GPIOcs 	 : in std_logic;
		GPIOaddr  : in std_logic_vector(31 downto 0);
		GPIOinput : in std_logic_vector(31 downto 0);
		GPIOstore : in std_logic;
		GPIOload : in std_logic;
		
		-- OUTPUTS to TOP
		GPIOleds 	 : out std_logic_vector(31 downto 0);
		GPIOdisplay1 : out std_logic_vector(31 downto 0);
		GPIOdisplay2 : out std_logic_vector(31 downto 0);
		-- OUTPUT to PROC
		GPIOoutput	 : out std_logic_vector(31 downto 0)
	);
end entity;

-- ARCHITECTURE
architecture archi of GPIO is
	
		signal combDisplay1, regDisplay1 : std_logic_vector(31 downto 0);
		signal combDisplay2, regDisplay2 : std_logic_vector(31 downto 0);
		signal combLed, regLed : std_logic_vector(31 downto 0);
		signal combGpio, regGpio	: std_logic_vector(31 DOWNTO 0) := (others => '0');
		signal GPIOloadP2 : std_logic;
begin
	-- BEGIN
	
	combDisplay1 <= GPIOinput when (GPIOcs='1' and GPIOstore='1' and GPIOaddr(3)='0' and GPIOaddr(2)='1') else regDisplay1; --0x80000004
	regDisplay1  <= (others => '1') when GPIOreset='1' else
						 combDisplay1 when rising_edge(GPIOclock);
					
	combDisplay2 <= GPIOinput when (GPIOcs='1' and GPIOstore='1' and GPIOaddr(3)='1' and GPIOaddr(2)='0') else regDisplay2; --0x80000008
	regDisplay2  <= (others => '1') when GPIOreset='1' else
						 combDisplay2 when rising_edge(GPIOclock);

	combLed 		 <= GPIOinput when (GPIOcs='1' and GPIOstore='1' and GPIOaddr(3)='1' and GPIOaddr(2)='1') else regLed;		--0x8000000C
	regLed 		 <= (others => '0') when GPIOreset='1' else
						 combLed when rising_edge(GPIOclock);
						 
	combGpio 	 <= x"00000" & GPIOkey1 & GPIOkey0 & GPIOsw9 & GPIOsw8 & GPIOsw7 & GPIOsw6 & 
						 GPIOsw5 & GPIOsw4 & GPIOsw3 & GPIOsw2 & GPIOsw1 & GPIOsw0 when (GPIOcs='1') else
						 regGpio;
	regGpio 		 <= combGpio when rising_edge(GPIOclock);
	
	GPIOloadP2 	 <= GPIOload when rising_edge(GPIOclock);
	
	GPIOdisplay1 <= regDisplay1;
	GPIOdisplay2 <= regDisplay2;
	GPIOleds 	 <= regLed;
	
	GPIOoutput 	 <= regGpio when (GPIOloadP2='1' and GPIOaddr(4)='1') else																			--0x80000010
						 regDisplay1 when (GPIOloadP2='1' and GPIOaddr(4)='0' and GPIOaddr(3)='0' and GPIOaddr(2)='1') else				--0x80000004
						 regDisplay2 when (GPIOloadP2='1' and GPIOaddr(4)='0' and GPIOaddr(3)='1' and GPIOaddr(2)='0') else				--0x80000008
						 regLed when (GPIOloadP2='1' and GPIOaddr(4)='0' and GPIOaddr(3)='1' and GPIOaddr(2)='1') else						--0x8000000C
--						 regGpio when (GPIOcs='1' and GPIOstore='0' and GPIOaddr(4)='1') else
						 (others => '1');
		
	-- END
end archi;
-- END FILE