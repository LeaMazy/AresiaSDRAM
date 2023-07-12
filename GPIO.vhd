--
-- Projet de fin d'études : RISC-V
-- ECE Paris / ARESIA
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
--		debug			 : out std_logic_vector(31 DOWNTO 0)
	);
end entity;

-- ARCHITECTURE
architecture archi of GPIO is
	
		signal combDisplay1, regDisplay1 : std_logic_vector(31 downto 0);
		signal combDisplay2, regDisplay2 : std_logic_vector(31 downto 0);
		signal combLed, regLed : std_logic_vector(31 downto 0);
		signal SIGgpio, TOPGPIO, GPIOoutMux, GPIOoutReg : std_logic_vector(31 downto 0) := (others=> '0');
--		signal SIGtestdebug, SIGtestdeb, SIGtestdebug2, SIGtestdeb2 : std_logic :='0';
		
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
		
	GPIOoutMux	 <= TOPGPIO when (GPIOcs='1' and GPIOLoad='1' and GPIOaddr(4)='1') else
						 regDisplay1 when (GPIOcs='1' and GPIOLoad='1' and GPIOaddr(3)='0' and GPIOaddr(2)='1') else
						 regDisplay2 when (GPIOcs='1' and GPIOLoad='1' and GPIOaddr(3)='1' and GPIOaddr(2)='0') else
						 regLed when (GPIOcs='1' and GPIOLoad='1' and GPIOaddr(3)='1' and GPIOaddr(2)='1') else
						 GPIOoutReg;
	GPIOoutReg 	 <= GPIOoutMux when rising_edge(GPIOclock);
	GPIOoutput 	 <= GPIOoutReg;
	
	
	-------------------debug------------------------					 
--	SIGtestdebug <= '1' WHEN (GPIOinput=x"0000000D") else
--					    SIGtestdeb;
--	SIGtestdeb <= SIGtestdebug WHEN rising_edge(GPIOclock);
--	
--	SIGtestdebug2 <= '1' WHEN (GPIOinput=x"00000012") else
--						 SIGtestdeb2;
--	SIGtestdeb2 <= SIGtestdebug2 WHEN rising_edge(GPIOclock);
--	
--	debug <= "00000000000000000000000" & SIGtestdeb2 & "0000000" & SIGtestdeb;
	-- END
end archi;
-- END FILE