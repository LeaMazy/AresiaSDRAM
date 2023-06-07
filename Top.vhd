-- Projet de fin d'études : RISC-V
-- ECE Paris / SECAPEM
-- Top entity VHDL = Processor + DataMemory + InstructionMemory

-- LIBRARIES
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.numeric_std.ALL;
USE work.simulPkg.ALL;

-- ENTITY
ENTITY Top IS
	PORT (
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
		TOPleds                              : OUT   STD_LOGIC_VECTOR(31 DOWNTO 0);					 --0x8000000c
		tx												 : OUT 	STD_LOGIC
	);
END ENTITY;

-- ARCHITECTURE
ARCHITECTURE archi OF Top IS

	-- COMPONENTS
	-- processor
	COMPONENT Processor IS
		PORT (
			-- INPUTS
			Hold            : IN  STD_LOGIC;
			PROCclock       : IN  STD_LOGIC;
			PROCreset       : IN  STD_LOGIC;
			PROCinstruction : IN  STD_LOGIC_VECTOR(31 DOWNTO 0);
			PROCoutputDM    : IN  STD_LOGIC_VECTOR(31 DOWNTO 0);
			
			-- OUTPUTS
			PROCprogcounter : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
			PROCPC			 : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
			PROCstore       : OUT STD_LOGIC;
			PROCload        : OUT STD_LOGIC;
			PROCfunct3      : OUT STD_LOGIC_VECTOR(2 DOWNTO 0);
			PROCaddrDM      : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
			PROCinputDM     : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
			PROCdq 			 : OUT STD_LOGIC_VECTOR(3 DOWNTO 0);
			PROCRFin			 : OUT STD_LOGIC_VECTOR(31 downto 0);
			PROCRFout1		 : OUT STD_LOGIC_VECTOR(31 downto 0);
			PROCRFout2		 : OUT STD_LOGIC_VECTOR(31 downto 0)
		);
	END COMPONENT;

	COMPONENT Counter IS
		PORT (
			-- INPUTS
			CPTclock   : IN  STD_LOGIC;
			CPTreset   : IN  STD_LOGIC;
			CPTwrite   : IN  STD_LOGIC;
			CPTaddr    : IN  STD_LOGIC_VECTOR(31 DOWNTO 0);
			CPTinput   : IN  STD_LOGIC_VECTOR(31 DOWNTO 0);

			-- OUTPUTS
			CPTcounter : OUT STD_LOGIC_VECTOR(31 DOWNTO 0)
		);
	END COMPONENT;

	COMPONENT Displays IS
		PORT (
			--INPUTS
			DISPcs 	 	 : in std_logic;
			DISPclock    : IN  STD_LOGIC;
			DISPreset    : IN  STD_LOGIC;
			DISPaddr     : IN  STD_LOGIC_VECTOR(31 DOWNTO 0);
			DISPinput    : IN  STD_LOGIC_VECTOR(31 DOWNTO 0);
			DISPwrite    : IN  STD_LOGIC;

			--OUTPUTS
			DISPleds     : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
			DISPdisplay1 : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
			DISPdisplay2 : OUT STD_LOGIC_VECTOR(31 DOWNTO 0)
		);
	END COMPONENT;
	COMPONENT clock1M IS
		PORT (
			areset : IN  STD_LOGIC := '0';
			inclk0 : IN  STD_LOGIC := '0';
			c0     : OUT STD_LOGIC;
			locked : OUT STD_LOGIC
		);
	END COMPONENT;

	COMPONENT RAM8x4 IS
		PORT (
			address_a : IN  STD_LOGIC_VECTOR (11 DOWNTO 0);
			address_b : IN  STD_LOGIC_VECTOR (11 DOWNTO 0);
			clock     : IN  STD_LOGIC := '1';
			data_a    : IN  STD_LOGIC_VECTOR (31 DOWNTO 0);
			data_b    : IN  STD_LOGIC_VECTOR (31 DOWNTO 0);
			enable    : IN  STD_LOGIC := '1';
			wren_a    : IN  STD_LOGIC := '0';
			wren_b    : IN  STD_LOGIC := '0';
			dq    	 : IN  STD_LOGIC_VECTOR (3 DOWNTO 0);
			q_a       : OUT STD_LOGIC_VECTOR (31 DOWNTO 0);
			q_b       : OUT STD_LOGIC_VECTOR (31 DOWNTO 0)
		);
	END COMPONENT;
	COMPONENT DEBUGER IS
		PORT (
			-- INPUTS
			enable                		  : IN  STD_LOGIC;
			SW8, SW7, SW6, SW5, SW4, SW3 : IN  STD_LOGIC;
			--reset    	: IN STD_LOGIC; --SW0
			PCregister            	: IN  STD_LOGIC_VECTOR(15 DOWNTO 0);
			Instruction           	: IN  STD_LOGIC_VECTOR(31 DOWNTO 0);
			RFin			: IN STD_LOGIC_VECTOR(31 downto 0);
			RFout1		: IN STD_LOGIC_VECTOR(31 downto 0);
			RFout2		: IN STD_LOGIC_VECTOR(31 downto 0);

			--OUTPUTS
			TOPdisplay2           	: OUT STD_LOGIC_VECTOR(31 DOWNTO 0) := (OTHERS => '1'); --0x80000008
			TOPdisplay1           	: OUT STD_LOGIC_VECTOR(31 DOWNTO 0) := (OTHERS => '1')  --0x80000004
		);
	END COMPONENT;
	
	component Bootloader is
	port (
		--INPUTS
		--INPUTS
		clk 					: in std_logic;
		CS 					: in std_logic; 							--chip select
		addrInstBoot		: in std_logic_vector(11 downto 0); --addr of boot instruction
		--OUTPUT
		instBoot				: out std_logic_vector(31 downto 0)    --output boot instruction
	);
	end component;
	
   component uartComm IS
	PORT(
		clk		:	IN	STD_LOGIC;
		reset	:	IN	STD_LOGIC;				--ascynchronous reset
		data_in  :  IN STD_LOGIC_VECTOR(31 DOWNTO 0);
		-- reading  :  IN STD_LOGIC;
		addOutMP	:	IN STD_LOGIC_VECTOR(31 DOWNTO 0);
		cs 		:	IN STD_LOGIC;
	   rx			:	IN STD_LOGIC;	
		uartload	:	IN STD_LOGIC;	
		uartstore:	IN STD_LOGIC;	
		data_out :  OUT 	STD_LOGIC_VECTOR(31 DOWNTO 0);
		tx			:	OUT	STD_LOGIC
	);
	END component;


	--------STATE MACHINES	
	TYPE BootMemMachine IS (idle, R1, R2, R3);	--state machine to force reset when boot mode is activated/desactivated
	SIGNAL currentState, nextState : BootMemMachine;
	--------SIGNALS
	--SIGNAL SIGoutputDMorREG : STD_LOGIC_VECTOR (31 DOWNTO 0);
	SIGNAL SIGcounter                                    : STD_LOGIC_VECTOR (31 DOWNTO 0); --0x80000000
	SIGNAL SIGPLLclock                                   : STD_LOGIC;
	SIGNAL SIGPLLclockinverted                           : STD_LOGIC;
	SIGNAL SIGclock                                      : STD_LOGIC; --either from pll or simulation
	--SIGNAL SIGclockInverted : STD_LOGIC; --either from pll or simulation
	SIGNAL SIGsimulOn                                    : STD_LOGIC; --either from pll or simulation
	SIGNAL TOPreset                                      : STD_LOGIC; --main reset
	SIGNAL SIGreset												  : STD_LOGIC; --state machine reset
	SIGNAL PLLlock                                       : STD_LOGIC;

	--SIGNAL debuger
	SIGNAL debugDisplay1, debugDisplay2			           : STD_LOGIC_VECTOR(31 DOWNTO 0);
	SIGNAL procDisplay1, procDisplay2, procLed           : STD_LOGIC_VECTOR(31 DOWNTO 0);
	SIGNAL RegcsDMProc, MuxcsDMProc                      : STD_LOGIC;
	
	SIGNAL SIGPROCinstruction 			: STD_LOGIC_VECTOR(31 DOWNTO 0);
	SIGNAL SIGPROCoutputDM 				: STD_LOGIC_VECTOR(31 DOWNTO 0);
	SIGNAL SIGPROChold 					: STD_LOGIC;
	SIGNAL SIGPROCprogcounter			: STD_LOGIC_VECTOR(31 DOWNTO 0);
	SIGNAL SIGPROCPC						: STD_LOGIC_VECTOR(31 DOWNTO 0);
	SIGNAL SIGPROCstore, SIGPROCload : STD_LOGIC;
	SIGNAL SIGPROCfunct3 				: STD_LOGIC_VECTOR(2 DOWNTO 0);
	SIGNAL SIGPROCaddrDM 				: STD_LOGIC_VECTOR(31 DOWNTO 0);
	SIGNAL SIGPROCinputDM 				: STD_LOGIC_VECTOR(31 DOWNTO 0);
	SIGNAL SIGPROCRFin 					: STD_LOGIC_VECTOR(31 DOWNTO 0);			 
	SIGNAL SIGPROCRFout1	 				: STD_LOGIC_VECTOR(31 DOWNTO 0);	 
	SIGNAL SIGPROCRFout2	 				: STD_LOGIC_VECTOR(31 DOWNTO 0);
	
	SIGNAL SIGfunct3 						: STD_LOGIC_VECTOR(2 DOWNTO 0);
	SIGNAL SIGcsDM, SIGwriteSelect   : STD_LOGIC;
	SIGNAL SIGinputDM, SIGAddressDM  : STD_LOGIC_VECTOR(31 DOWNTO 0);
	SIGNAL SIGReady_32b, SIGData_Ready_32b : STD_LOGIC;
	SIGNAL SIGDataOut_32b 				: STD_LOGIC_VECTOR(31 DOWNTO 0);
	
	--Store/Load SRAM
	SIGNAL SIGMEMcs	 	 	 : std_logic;
	SIGNAL MuxPROCstore_b  : STD_LOGIC;
	SIGNAL SIGPROCdq		: STD_LOGIC_VECTOR(3 DOWNTO 0);
	SIGNAL SIGMEMdq		: STD_LOGIC_VECTOR(3 DOWNTO 0);
	--BootLoader
	SIGNAL SIGbootReg1, SIGbootReg2 : std_logic;
	SIGNAL SIGbootChg			 : std_logic;
	SIGNAL SIGboot			 	 : std_logic; --state machine boot
	SIGNAL SIGbootReg		 	 : std_logic := '0'; 
	SIGNAL SIGbootMux		 	 : std_logic;  
	SIGNAL SIGinstBoot	 	 : std_logic_vector(31 downto 0);
	SIGNAL SIGinstMux 	 	 : std_logic_vector(31 downto 0);
	--UART
	SIGNAL SIGuartCS	 	 	 : std_logic;
	SIGNAL SIGSelectDataOut  : std_logic_vector(4 downto 0);
	SIGNAL SIGUARTOut			 : std_logic_vector(31 downto 0);
	SIGNAL SIGMuxDataOut		 : std_logic_vector(31 downto 0);
	--Displayer
	SIGNAL SIGdispCS	 	 	 : std_logic;
	


	
	
BEGIN

	TOPreset <= '1' WHEN reset = '1' ELSE
				   SIGreset WHEN rising_edge(SIGclock);
	-- BEGIN
	-- ALL
	-- TEST BENCH ONLY ---

	PKG_instruction   <= SIGinstMux;
	PKG_store         <= SIGPROCstore;
	PKG_load          <= SIGPROCload;
	PKG_funct3        <= SIGPROCfunct3;
	PKG_addrDM        <= SIGPROCaddrDM;
	PKG_inputDM       <= SIGPROCinputDM;
	PKG_outputInstr	<= SIGinstMux;
	PKG_outputDM      <= SIGPROCoutputDM;
	PKG_progcounter   <= SIGPROCPC;
	PKG_counter       <= SIGcounter;
	SIGsimulOn			<= PKG_simulON;
	-----------------------
	
	-- Chip Select for sram, displayer and uart
	SIGmemCS <= '0' when (SIGPROCaddrDM(31)='1' and (SIGPROCload='1' or SIGPROCstore='1')) else
					 '1'; --when (SIGPROCload='0') or (SIGPROCstore='0');
	SIGdispCS <= '1' when (SIGPROCload='1' or SIGPROCstore='1') and (SIGPROCaddrDM(31)='1' and SIGPROCaddrDM(30)='0') else '0';
	SIGuartCS <= '1' when (SIGPROCload='1' or SIGPROCstore='1') and (SIGPROCaddrDM(31)='1' and SIGPROCaddrDM(30)='1') else '0';

	-- Multiplexor for instruction between Boot and Sram
	SIGinstMux <= SIGinstBoot when SIGboot = '1' else
					  SIGPROCinstruction;
	
	-- Sram specific signal
	-- avoid writing in memory when the proc wants to write on its outputs
	MuxPROCstore_b <= '0' WHEN SIGPROCaddrDM(31)='1' ELSE
							SIGPROCstore;
	SIGMEMdq <= (others => '0') WHEN SIGMEMcs='0' else SIGPROCdq;
	--

	SIGclock    <= TOPclock WHEN SIGsimulOn = '1' ELSE
								buttonClock WHEN enableDebug = '1'  ELSE
								SIGPLLclock;

	TOPdisplay1 <= procDisplay1 WHEN enableDebug = '0' ELSE
		            debugDisplay1;

	TOPdisplay2 <= procDisplay2 WHEN enableDebug = '0' ELSE
		            debugDisplay2;

	TOPLeds <= procLed WHEN enableDebug = '0' ELSE procLed;

	
	SIGSelectDataOut <= SIGmemCS & SIGdispCS & SIGuartCS & SIGPROCaddrDM(3) & SIGPROCaddrDM(2) when rising_edge(SIGclock);
	SIGMuxDataOut <=  SIGPROCoutputDM when (SIGSelectDataOut(4 downto 2)="100") else
							procDisplay1    when (SIGSelectDataOut="01001") else --0x80000004
							procDisplay2    when (SIGSelectDataOut="01010") else --0x80000008
							SIGUARTOut 		 when (SIGSelectDataOut(4 downto 2)="001") else 
							(others => '0');
	
	SIGbootReg1 <= switchBoot when rising_edge(SIGclock);
	SIGbootReg2 <= SIGbootReg1 when rising_edge(SIGclock);
	SIGbootChg 	<= SIGbootReg1 xor SIGbootReg2;
	
	

	-- INSTANCES

	debug : debUGER
	PORT MAP(
		--TOPclock =>
		enable      => enableDebug,
		SW8			=> SW8, 
		SW7			=> SW7, 
		SW6			=> SW6, 
		SW5			=> SW5, 
		SW4			=> SW4,
		SW3			=> SW3,
		PCregister  => SIGPROCPC(15 DOWNTO 0),
		Instruction => SIGinstMux,
		RFin			=> SIGPROCRFin,
		RFout1		=> SIGPROCRFout1,
	   RFout2		=> SIGPROCRFout2,
		--OUTPUTS
		TOPdisplay2 => debugDisplay2,
		TOPdisplay1 => debugDisplay1
	);

	instPROC : Processor
	PORT MAP(
		Hold            => '0',
		PROCclock       => SIGclock,
		PROCreset       => TOPreset,
		PROCinstruction => SIGinstMux,
		PROCoutputDM    => SIGMuxDataOut,
--		PROCoutputDM    => SIGPROCoutputDM,
		-- OUTPUTS
		PROCprogcounter => SIGPROCprogcounter,
		PROCPC 			 => SIGPROCPC,
		PROCstore       => SIGPROCstore,
		PROCload        => SIGPROCload,
		PROCfunct3      => SIGPROCfunct3,
		PROCaddrDM      => SIGPROCaddrDM,
		PROCinputDM     => SIGPROCinputDM,
		PROCdq 			 => SIGPROCdq,
		PROCRFin			 => SIGPROCRFin,
		PROCRFout1		 => SIGPROCRFout1,
	   PROCRFout2		 => SIGPROCRFout2
	);

	instCPT : Counter
	PORT MAP(
		CPTclock   => SIGclock,
		CPTreset   => TOPreset,
		CPTwrite   => SIGPROCstore,
		CPTaddr    => SIGPROCaddrDM,
		CPTinput   => SIGPROCoutputDM,
		CPTcounter => SIGcounter
	);

	instDISP : Displays
	PORT MAP(
		--INPUTS
		DISPcs 		 => SIGdispCS,
		DISPclock    => SIGclock,
		DISPreset    => TOPreset,
		DISPaddr     => SIGPROCaddrDM,
		DISPinput    => SIGPROCinputDM,
		DISPWrite    => SIGPROCstore,
		--OUTPUTS
		DISPleds     => procLed,
		DISPdisplay1 => procDisplay1,
		DISPdisplay2 => procDisplay2
	);

	instPLL : clock1M
	PORT MAP(
		areset => '0',
		inclk0 => TOPclock,
		c0     => SIGPLLclock,
		locked => PLLlock
	);
	
	
	Memory : RAM8x4
	PORT MAP(
		address_a => SIGPROCprogcounter(13 downto 2),  --  Addr instruction (divided by 4 because we use 32 bits memory)
		address_b => SIGPROCaddrDM(13 downto 2),       --  Addr memory (divided by 4 because we use 32 bits memory)
		clock     => SIGclock,
		data_a    => (OTHERS => '0'), 		-- Instruction in
		data_b    => SIGPROCinputDM,  	-- Data in
		enable    => '1',						-- ChipSelect for SRAM
		wren_a    => '0',       -- Write Instruction Select
		wren_b    => MuxPROCstore_b,       -- Write Data Select
		dq			 => SIGMEMdq,
		q_a       => SIGPROCinstruction, -- DataOut Instruction
		q_b       => SIGPROCoutputDM		-- DataOut Data
	);
	
	instBoot : Bootloader
	port map(
		--INPUTS
		clk 			 => SIGclock,
		CS 			 => SIGboot, 							 --chip select
		addrInstBoot => SIGPROCprogcounter(13 downto 2), --addr of boot instruction
		--OUTPUT
		instBoot		 => SIGinstBoot							 --output boot instruction
	);
	
	instUARTComm : UARTComm
	port map(
		clk		=> SIGclock,
		reset	=> TOPreset,
		data_in  => SIGPROCinputDM,
		uartload	=> SIGPROCload,
		uartstore => SIGPROCstore,
		addOutMP	=> SIGPROCaddrDM,
		cs 		=> SIGuartCS,
	   rx			=>	rx,
		data_out => SIGUARTOut,
		tx			=> tx
	);

	SIGbootMux <= switchBoot when currentState=R1 else 
					  SIGbootReg;
	SIGbootReg <= SIGbootMux when rising_edge(SIGclock);
	
	-- State machine process
	iBootMemMachine : PROCESS(SIGclock, reset, switchBoot, SIGbootChg, currentState, SIGbootReg)
	BEGIN
		--init 
		nextState <= currentState;
		SIGreset <= '0';
		SIGboot  <= SIGbootReg;
		
		--cases
		case currentState is 
			when idle =>
				SIGreset <= '0';
				if(SIGbootChg='1') then nextState <= R1;
				end if;
			when R1 => 
				SIGreset <= '1';
				SIGboot <= SIGbootReg;
				nextState <= R2;
			when R2 => 
				nextState <= R3;
			when R3 => 
				SIGreset <= '0';
				nextState <= idle;
		end case;
	END PROCESS;
		
	iProcessSynchro : PROCESS(reset, SIGclock)
	BEGIN 
		if (reset = '1') then currentState <= idle;
		elsif (rising_edge(SIGclock)) then currentState <= nextState;
		end if;
	END PROCESS;
END archi;
-- END FILE