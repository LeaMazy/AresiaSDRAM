-- Projet de fin d'études : RISC-V
-- ECE Paris / SECAPEM

-- LIBRARIES
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.simulPkg.all;

-- ENTITY
entity TestBench is
end entity;

architecture VHDL of TestBench is
	component Top is
    port (
		-- INPUTS
		enableDebug									 : IN    STD_LOGIC; -- debugger mode
		SW8, SW7, SW6, SW5, SW4, SW3 			 : IN    STD_LOGIC; -- inputs for debuger
		switchBoot									 : IN 	STD_LOGIC; -- input for bootloader
		TOPclock                             : IN    STD_LOGIC; -- must go through pll
		buttonClock                          : IN    STD_LOGIC;
		reset                                : IN    STD_LOGIC;
		rx												 : IN 	STD_LOGIC;
		--SW0

		-- OUTPUTS
		TOPdisplay1                          : OUT   STD_LOGIC_VECTOR(31 DOWNTO 0);                --0x80000004
		TOPdisplay2                          : OUT   STD_LOGIC_VECTOR(31 DOWNTO 0);                --0x80000008
		TOPleds                              : OUT   STD_LOGIC_VECTOR(31 DOWNTO 0);			  		 --0x8000000c
		tx												 : OUT 	STD_LOGIC
	);
	end component;

	signal reset, ck : std_logic;
	signal sigBoot : std_logic;
	signal counter, progcounter, instr : std_logic_vector(31 downto 0);
    
	signal dataAddr: std_logic_vector(31 downto 0);
	signal load, store : std_logic;
	signal dataLength : std_logic_vector(2 downto 0);
	signal inputData, outputData, outputInstr: std_logic_vector(31 downto 0);
   
	signal reg00, reg01, reg02, reg03, reg04, reg05, reg06, reg07, reg08, reg09, reg0A, reg0B, reg0C, reg0D, reg0E, reg0F, reg10, reg11, reg12, reg13, reg14, reg15, reg16, reg17, reg18, reg19, reg1A, reg1B, reg1C, reg1D, reg1E, reg1F : std_logic_vector(31 downto 0);
	signal SigTOPdisplay1, SigTOPdisplay2 : std_logic_vector (31 downto 0);

	type mem is array(0 to 2047) of std_logic_vector(15 downto 0);
	signal TabMemory : mem :=(others => (others => '0'));
	
	signal sigtx : std_logic;
	signal sigrx : std_logic;
															 

	BEGIN

	--instanciation de l'entité PROC
	iTop : Top port map (
	
		TOPclock        => ck,
		reset        	 => reset,
		TOPdisplay1     => SigTOPdisplay1,
		TOPdisplay2     => SigTOPdisplay2,
		rx					 => sigrx,
		
		enableDebug 	 => '0',
		SW8		 		 => '0',
		SW7		 		 => '0',
		SW6		 		 => '0',
		SW5		 		 => '0',
		SW4		 		 => '0',
		SW3		 		 => '0',
		switchBoot 		 => sigBoot,
		buttonClock		 => '0',
		tx					 => sigtx
		
		
	);
    
    counter       <= PKG_counter;
    store         <= PKG_store;      
    load          <= PKG_load;       
    dataLength    <= PKG_funct3;     
    dataAddr      <= PKG_addrDM;     
    inputData     <= PKG_inputDM;    
    outputData    <= PKG_outputDM; 
	 outputData    <= PKG_outputDM;
	 outputInstr   <= PKG_outputInstr;
	 
    progcounter <= PKG_progcounter;
    instr       <= PKG_instruction;
    reg00       <= PKG_reg00;
    reg01       <= PKG_reg01;
    reg02       <= PKG_reg02;
    reg03       <= PKG_reg03;
    reg04       <= PKG_reg04;
    reg05       <= PKG_reg05;
    reg06       <= PKG_reg06;
    reg07       <= PKG_reg07;
    reg08       <= PKG_reg08;
    reg09       <= PKG_reg09;
    reg0A       <= PKG_reg0A;
    reg0B       <= PKG_reg0B;
    reg0C       <= PKG_reg0C;
    reg0D       <= PKG_reg0D;
    reg0E       <= PKG_reg0E;
    reg0F       <= PKG_reg0F;
    reg10       <= PKG_reg10;
    reg11       <= PKG_reg11;
    reg12       <= PKG_reg12;
    reg13       <= PKG_reg13;
    reg14       <= PKG_reg14;
    reg15       <= PKG_reg15;
    reg16       <= PKG_reg16;
    reg17       <= PKG_reg17;
    reg18       <= PKG_reg18;
    reg19       <= PKG_reg19;
    reg1A       <= PKG_reg1A;
    reg1B       <= PKG_reg1B;
    reg1C       <= PKG_reg1C;
    reg1D       <= PKG_reg1D;
    reg1E       <= PKG_reg1E;
    reg1F       <= PKG_reg1F;
	 

		clocktestbench: process
		begin
		-- init  simulation
			ck <= '1';
			wait for 10 ns;
			ck <= '0';
			wait for 10 ns;
		end process;
		
		resetTestbench : process
		begin
		-- init  simulation
			reset <= '1';
			wait for 1000 ns;
			reset <= '0';
--			wait for 10100 ns;
--			reset <= '1';
--			wait for 410 ns;
--			reset <= '0';
			wait;
		end process;
		
		bootTestbench: process
		begin
		--init  simulation
--   		sigBoot <= '1';
--   		wait for 2100 ns;
			sigBoot <= '1';
			wait for 10100 ns;
   		sigBoot <= '0';
--			wait for 12100 ns;
--			sigBoot <= '1';
--			wait for 10100 ns;
--   		sigBoot <= '0';
--			wait for 12100 ns;
--			sigBoot <= '1';
--			wait for 10100 ns;
--   		sigBoot <= '0';
			wait;
		end process;
		
		uarttestbench: process
		begin
		-- init  simulation
			wait for 1000 ns;
			sigrx <= '1';
			wait for 100 ns;
			sigrx <= '0';
--			wait for 100 ns;
--			sigrx <= '0';
--			wait for 100 ns;
--			sigrx <= '0';
--			wait for 100 ns;
--			sigrx <= '0';
--			wait for 100 ns;
--			sigrx <= '0';
--			wait for 100 ns;
--			sigrx <= '1';
--			wait for 100 ns;
--			sigrx <= '0';
--			wait for 100 ns;
--			sigrx <= '0';
			wait;
		end process;
		
END vhdl;