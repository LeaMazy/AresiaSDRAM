--
-- Projet de fin d'études : RISC-V
-- ECE Paris / SECAPEM
-- Displays VHDL

-- LIBRARIES
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

-- ENTITY
entity GPIO is
	port (
		--INPUTS
		-- From TOP
		GPIOclock 															: in std_logic;
		GPIOreset 															: in std_logic;
		GPIOSW8, GPIOSW7, GPIOSW6, GPIOSW5, GPIOSW4, GPIOSW3 	: in std_logic;  --  inputs for debuger
		GPIOhold																: in std_logic;
		
		-- From PROC
		GPIOcs 	 : in std_logic;
		GPIOaddr  : in std_logic_vector(31 downto 0);
		GPIOinput : in std_logic_vector(31 downto 0);
		GPIOwrite : in std_logic;
		GPIOload  : in std_logic;
		
		
		enableDebug									 : IN    STD_LOGIC;  --  debugger mode
		switchBoot									 : IN 	STD_LOGIC;  --  input for bootloader
		buttonClock                          : IN    STD_LOGIC;  --  Debug clock Butto
		
		
		--OUTPUTS
		DISPleds 	 : out std_logic_vector(31 downto 0);
		DISPdisplay1 : out std_logic_vector(31 downto 0);
		DISPdisplay2 : out std_logic_vector(31 downto 0);
		GPIOoutput	 : out std_logic_vector(31 DOWNTO 0)
	);
end entity;

-- ARCHITECTURE
architecture archi of GPIO is
	
		signal combDisplay1, regDisplay1 : std_logic_vector(31 downto 0);
		signal combDisplay2, regDisplay2 : std_logic_vector(31 downto 0);
		signal combLed, regLed : std_logic_vector(31 downto 0);
		signal SIGgpio, TOPGPIO : std_logic_vector(31 downto 0);
		signal GPIOLoadP2 : std_logic;
begin
	-- BEGIN
	
	combDisplay1 <= GPIOinput when (GPIOcs='1' and GPIOwrite='1' and GPIOaddr(3)='0' and GPIOaddr(2)='1') else regDisplay1; --0x80000004
	regDisplay1  <= (others => '1') when GPIOreset='1' else
						 combDisplay1 when rising_edge(GPIOclock);
					
	combDisplay2 <= GPIOinput when (GPIOcs='1' and GPIOwrite='1' and GPIOaddr(3)='1' and GPIOaddr(2)='0') else regDisplay2; --0x80000008
	regDisplay2  <= (others => '1') when GPIOreset='1' else
						 combDisplay2 when rising_edge(GPIOclock);

	combLed 		 <= GPIOinput when (GPIOcs='1' and GPIOwrite='1' and GPIOaddr(3)='1' and GPIOaddr(2)='1') else regLed;		 --0x8000000C
	regLed 		 <= (others => '0') when GPIOreset='1' else
						 combLed when rising_edge(GPIOclock);
					
	SIGgpio <= x"00000" & '0' & buttonClock & enableDebug & GPIOSW8 & GPIOSW7 & GPIOSW6 & GPIOSW5 & GPIOSW4 & GPIOSW3 & switchBoot & GPIOhold & GPIOreset;
	TOPGPIO <= SIGgpio when rising_edge(GPIOclock);
	
	DISPdisplay1 <= regDisplay1;
	DISPdisplay2 <= regDisplay2;
	DISPleds 	 <= regLed;
	
	GPIOLoadP2	<= GPIOLoad when rising_edge(GPIOclock);
	
	GPIOoutput	 <= TOPGPIO when (GPIOLoadP2='1' and GPIOaddr(4)='1') else
						 regDisplay1 when (GPIOLoadP2='1' and GPIOaddr(3)='0' and GPIOaddr(2)='1') else
						 regDisplay2 when (GPIOLoadP2='1' and GPIOaddr(3)='1' and GPIOaddr(2)='0') else
						 regLed when (GPIOLoadP2='1' and GPIOaddr(3)='1' and GPIOaddr(2)='1') else
						 (others => '0');
	-- END
end archi;
-- END FILE