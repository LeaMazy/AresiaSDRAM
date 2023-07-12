LIBRARY ieee;
USE ieee.std_logic_1164.all;


ENTITY uartComm IS
	PORT(
		clk		:	IN	STD_LOGIC;
		reset		:	IN	STD_LOGIC;				--ascynchronous reset
		data_in  :  IN STD_LOGIC_VECTOR(31 DOWNTO 0);
		-- reading  :  IN STD_LOGIC;
		addOutMP	:	IN STD_LOGIC_VECTOR(31 DOWNTO 0);
		cs 		:	IN STD_LOGIC;
	   rx			:	IN STD_LOGIC;
		uartload	:	IN STD_LOGIC;	
		uartstore:	IN STD_LOGIC;		

		data_out :  OUT 	STD_LOGIC_VECTOR(31 DOWNTO 0);
--		debug		:  OUT	STD_LOGIC_VECTOR(31 DOWNTO 0);
		tx			:	OUT	STD_LOGIC
	);
END uartComm;

ARCHITECTURE vhdl OF uartComm IS
	COMPONENT uart IS
	PORT(
		debug    :	OUT	STD_LOGIC_VECTOR(7 DOWNTO 0); 
		clk		:	IN	 	STD_LOGIC;				--system clock
		reset_n	:	IN	 	STD_LOGIC;				--ascynchronous reset
		tx_ena	:	IN		STD_LOGIC;				--initiate transmission
		tx_data	:	IN		STD_LOGIC_VECTOR(7 DOWNTO 0);  --data to transmit
		rx			:	IN		STD_LOGIC;				--receive pin
		rx_read	:	IN		STD_LOGIC;				--receive pin
		rx_full	:	OUT	STD_LOGIC;				--data reception in progress
		rx_error :	OUT	STD_LOGIC;				--start, parity, or stop bit error detected
		rx_data	:	OUT	STD_LOGIC_VECTOR(7 DOWNTO 0);	--data received
		tx_busy	:	OUT	STD_LOGIC;  				--transmission in progress
		tx			:	OUT	STD_LOGIC);				--transmit pin
	END COMPONENT;

	SIGNAL SIGRESET_N		 : std_logic;
	SIGNAL SIGSEL_STATUS  : std_logic;
	SIGNAL SIGSEL_RX		 : std_logic;
	SIGNAL SIGSEL_TX		 : std_logic;
	SIGNAL SIGRX_FULL		 : std_logic;
	SIGNAL SIGRX_ERROR	 : std_logic;
	SIGNAL SIGTX_BUSY		 : std_logic;
	SIGNAL SIGRX_DATA 	 : std_logic_vector(7 downto 0);
	SIGNAL SIGUART_STATUS : std_logic_vector(7 downto 0);
	SIGNAL SIGMUXOUT		 : std_logic_vector(7 downto 0);
	SIGNAL SIGREG_OUT	 	 : std_logic_vector(7 downto 0);
--	SIGNAL SIGdebug	 	 : std_logic_vector(7 downto 0);
--	SIGNAL SIGtestdebug   : std_logic;
--	SIGNAL SIGtestdeb     : std_logic :='0';
	
BEGIN
	SIGRESET_N <= not reset;
	SIGSEL_STATUS <= '1' when (cs='1' and addOutMP(2)='0' and uartload='1' and uartstore='0') else '0';
	SIGSEL_RX <= '1' when (cs='1' and addOutMP(2)='1' and uartload='1' and uartstore='0') else '0';
	SIGSEL_TX <= '1' when (cs='1' and addOutMP(2)='1' and uartload='0' and uartstore='1') else '0';	--uartload='0'
	SIGUART_STATUS <= ("00000" & SIGRX_FULL & SIGRX_ERROR & SIGTX_BUSY);
	SIGMUXOUT <= SIGUART_STATUS when (SIGSEL_STATUS='1') else 
				   (SIGRX_DATA) when (SIGSEL_RX='1') else
					SIGREG_OUT;--(others => '0');
	
	SIGREG_OUT <= SIGMUXOUT when rising_edge(clk);
	data_out <= "000000000000000000000000" & SIGREG_OUT;
	
	-------------------debug------------------------
--	debug <= "000000000000000000000000" & SIGRX_DATA;
--	debug <= "000000000000000000000000" & SIGdebug;
	
--	SIGtestdebug <= '1' WHEN ((SIGSEL_TX and SIGTX_BUSY) = '1') else
--					    SIGtestdeb;
--	SIGtestdeb <= SIGtestdebug WHEN rising_edge(clk);
	
--	debug <= "0000000000000000000000000000000" & SIGtestdeb;
	
	
	instUART : uart
	PORT MAP(
--		debug 	=> SIGdebug,
		clk	   => clk,
		reset_n	=> SIGRESET_N,
		tx_ena	=> SIGSEL_TX,
		tx_data	=> data_in(7 downto 0),
		rx			=> rx,
		rx_read	=> SIGSEL_RX,
		rx_full	=> SIGRX_FULL,
		rx_error => SIGRX_ERROR,
		rx_data	=> SIGRX_DATA,
		tx_busy	=> SIGTX_BUSY,
		tx	 		=> tx
	);
END vhdl;
