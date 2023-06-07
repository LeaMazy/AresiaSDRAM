LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY RAM8x4 IS
	PORT
	(
		address_a		: IN STD_LOGIC_VECTOR (11 DOWNTO 0);
		address_b		: IN STD_LOGIC_VECTOR (11 DOWNTO 0);
		clock			: IN STD_LOGIC  := '1';
		data_a		: IN STD_LOGIC_VECTOR (31 DOWNTO 0);
		data_b		: IN STD_LOGIC_VECTOR (31 DOWNTO 0);
		enable		: IN STD_LOGIC  := '1';
		wren_a		: IN STD_LOGIC  := '0';
		wren_b		: IN STD_LOGIC  := '0';
		dq				: IN STD_LOGIC_VECTOR (3 DOWNTO 0);
		q_a		: OUT STD_LOGIC_VECTOR (31 DOWNTO 0);
		q_b		: OUT STD_LOGIC_VECTOR (31 DOWNTO 0)
	);
END RAM8x4;

ARCHITECTURE SYN OF RAM8x4 IS

	component RAM8_0 is
    port (
		address_a		: IN STD_LOGIC_VECTOR (11 DOWNTO 0);
		address_b		: IN STD_LOGIC_VECTOR (11 DOWNTO 0);
		clock			: IN STD_LOGIC  := '1';
		data_a		: IN STD_LOGIC_VECTOR (7 DOWNTO 0);
		data_b		: IN STD_LOGIC_VECTOR (7 DOWNTO 0);
		enable		: IN STD_LOGIC  := '1';
		wren_a		: IN STD_LOGIC  := '0';
		wren_b		: IN STD_LOGIC  := '0';
		q_a		: OUT STD_LOGIC_VECTOR (7 DOWNTO 0);
		q_b		: OUT STD_LOGIC_VECTOR (7 DOWNTO 0)
	);
	end component;
	
	component RAM8_1 is
    port (
		address_a		: IN STD_LOGIC_VECTOR (11 DOWNTO 0);
		address_b		: IN STD_LOGIC_VECTOR (11 DOWNTO 0);
		clock			: IN STD_LOGIC  := '1';
		data_a		: IN STD_LOGIC_VECTOR (7 DOWNTO 0);
		data_b		: IN STD_LOGIC_VECTOR (7 DOWNTO 0);
		enable		: IN STD_LOGIC  := '1';
		wren_a		: IN STD_LOGIC  := '0';
		wren_b		: IN STD_LOGIC  := '0';
		q_a		: OUT STD_LOGIC_VECTOR (7 DOWNTO 0);
		q_b		: OUT STD_LOGIC_VECTOR (7 DOWNTO 0)
	);
	end component;
	
	component RAM8_2 is
    port (
		address_a		: IN STD_LOGIC_VECTOR (11 DOWNTO 0);
		address_b		: IN STD_LOGIC_VECTOR (11 DOWNTO 0);
		clock			: IN STD_LOGIC  := '1';
		data_a		: IN STD_LOGIC_VECTOR (7 DOWNTO 0);
		data_b		: IN STD_LOGIC_VECTOR (7 DOWNTO 0);
		enable		: IN STD_LOGIC  := '1';
		wren_a		: IN STD_LOGIC  := '0';
		wren_b		: IN STD_LOGIC  := '0';
		q_a		: OUT STD_LOGIC_VECTOR (7 DOWNTO 0);
		q_b		: OUT STD_LOGIC_VECTOR (7 DOWNTO 0)
	);
	end component;
	
	component RAM8_3 is
    port (
		address_a		: IN STD_LOGIC_VECTOR (11 DOWNTO 0);
		address_b		: IN STD_LOGIC_VECTOR (11 DOWNTO 0);
		clock			: IN STD_LOGIC  := '1';
		data_a		: IN STD_LOGIC_VECTOR (7 DOWNTO 0);
		data_b		: IN STD_LOGIC_VECTOR (7 DOWNTO 0);
		enable		: IN STD_LOGIC  := '1';
		wren_a		: IN STD_LOGIC  := '0';
		wren_b		: IN STD_LOGIC  := '0';
		q_a		: OUT STD_LOGIC_VECTOR (7 DOWNTO 0);
		q_b		: OUT STD_LOGIC_VECTOR (7 DOWNTO 0)
	);
	end component;

BEGIN		
	--instanciation des entitées ram_8
	instRAM0 : RAM8_0 port map (
		address_a => 	address_a,
		address_b => address_b,
		clock => clock,
		data_a => data_a(7 downto 0),
		data_b => data_b(7 downto 0),
		enable => enable,
		wren_a => wren_a,
		wren_b => dq(0),
		q_a => q_a(7 downto 0),
		q_b => q_b(7 downto 0)
	);
	
	instRAM1 : RAM8_1 port map (
		address_a => 	address_a,
		address_b => address_b,
		clock => clock,
		data_a => data_a(15 downto 8),
		data_b => data_b(15 downto 8),
		enable => enable,
		wren_a => wren_a,
		wren_b => dq(1),
		q_a => q_a(15 downto 8),
		q_b => q_b(15 downto 8)
	);
	
	instRAM2 : RAM8_2 port map (
		address_a => 	address_a,
		address_b => address_b,
		clock => clock,
		data_a => data_a(23 downto 16),
		data_b => data_b(23 downto 16),
		enable => enable,
		wren_a => wren_a,
		wren_b => dq(2),
		q_a => q_a(23 downto 16),
		q_b => q_b(23 downto 16)
	);
	
	instRAM3 : RAM8_3 port map (
		address_a => 	address_a,
		address_b => address_b,
		clock => clock,
		data_a => data_a(31 downto 24),
		data_b => data_b(31 downto 24),
		enable => enable,
		wren_a => wren_a,
		wren_b => dq(3),
		q_a => q_a(31 downto 24),
		q_b => q_b(31 downto 24)
	);
	
	END SYN;