-- Projet de fin d'études : RISC-V
-- ECE Paris / SECAPEM
-- ALIGNMENT VHDL

-- LIBRARIES
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

-- ENTITY
entity Alignment is
	port (
		IDfunct3 	: in std_logic_vector(2 downto 0);
		q_b  			: in std_logic_vector(31 downto 0);
		IDimm12I 	: in std_logic_vector(11 downto 0);
		IDimm12S 	: in std_logic_vector(11 downto 0);
		RF_Align_out: in std_logic_vector(31 downto 0);
		
		DQ 			: out std_logic_vector(3 downto 0);
		RF_Align_in	: out std_logic_vector(31 downto 0);
		PROCinputDM : out std_logic_vector(31 downto 0)
	);
end entity;

-- ARCHITECTURE
architecture archi of Alignment is
	signal Mask    :    std_logic_vector(31 downto 0);
	signal RF_Align : std_logic_vector(31 downto 0);
	signal storetype 		: std_logic_vector(3 downto 0);
	signal dq_0 : std_logic;
	signal dq_1 : std_logic;
	signal dq_2 : std_logic;
	signal dq_3 : std_logic;

begin
	-- Store
	storetype <= ("00" & IDimm12S(1 downto 0)) WHEN (IDfunct3 = "000" )	-- StoreByte (RAM3)
			else ("010" & IDimm12S(0)) WHEN (IDfunct3 = "001")	-- StoreHalf (RAM2 & RAM3)
			else ('1' & IDimm12S(2 downto 0)) WHEN (IDfunct3 = "010") -- StoreWord (RAMs)
			else ("0111"); -- (Ne prend jamais valeur 011X)
			
	dq_0 <= '1' WHEN ((storetype(3) ='1' or storetype(2 downto 0)="100" or storetype(3 downto 0)="0000"))
			else ('0');
	dq_1 <= '1' WHEN ((storetype(3) ='1' or storetype(2 downto 0)="100" or storetype(3 downto 0)="0001"))
			else ('0');	
	dq_2 <= '1' WHEN ((storetype(3) ='1' or storetype(2 downto 0)="101" or storetype(3 downto 0)="0010"))
			else ('0');
	dq_3 <= '1' WHEN ((storetype(3) ='1' or storetype(2 downto 0)="101" or storetype(3 downto 0)="0011"))
			else ('0');	
			
	DQ <= (dq_3 & dq_2 & dq_1 & dq_0);
	
	PROCinputDM <= std_logic_vector(shift_left(unsigned(RF_Align_out),0)) 
						when (IDfunct3= "010") 
				-- SHU
				 else std_logic_vector(shift_left(unsigned(RF_Align_out),16)) 						
						when (IDfunct3 = "001" and ((IDimm12S(0)='1') or (IDimm12S(1)='1'))) -- SHU (Imm%2!=0 || (Imm%2=0 & Imm%4!=0))
				 else std_logic_vector(shift_left(unsigned(RF_Align_out),0)) 
						when (IDfunct3 = "001" and (IDimm12S(0)='0')) -- SHU (Imm%4)
					-- SBU
				 else std_logic_vector(shift_left(unsigned(RF_Align_out),0)) 
						when (IDfunct3 = "000" and (IDimm12S(1 downto 0)="00")) -- SBU (Imm%4=0)
				 else std_logic_vector(shift_left(unsigned(RF_Align_out),(to_integer(unsigned(IDimm12S(1 downto 0))))*8)) 
						when (IDfunct3 = "000" and ((IDimm12S(1 downto 0)/="00"))) -- SBU (Imm%4!=0)
					-- Else
				 else (RF_Align_out);
			
	-- Load		
				-- LW
	RF_Align <= std_logic_vector(shift_right(unsigned(q_b),0)) 
					when (IDfunct3= "010") 
				-- LHU
			else std_logic_vector(shift_right(unsigned(q_b),16)) 						
					when (IDfunct3 = "101" and ((IDimm12I(0)='1') or (IDimm12I(1)='1'))) -- LHU (Imm%2!=0 || (Imm%2=0 & Imm%4!=0))
			else std_logic_vector(shift_right(unsigned(q_b),0)) 
					when (IDfunct3 = "101" and (IDimm12I(0)='0')) -- LHU (Imm%4)
				-- LBU
			else std_logic_vector(shift_right(unsigned(q_b),0)) 
					when (IDfunct3 = "100" and (IDimm12I(1 downto 0)="00")) -- LBU (Imm%4=0)
			else std_logic_vector(shift_right(unsigned(q_b),(to_integer(unsigned(IDimm12I(1 downto 0))))*8)) 
					when (IDfunct3 = "100" and ((IDimm12I(1 downto 0)/="00"))) -- LBU (Imm%4!=0)
				-- Else
			else (q_b);
	Mask <= "00000000000000001111111111111111" when (IDfunct3 = "101")
				else "00000000000000000000000011111111" when (IDfunct3 = "100")
				else (OTHERS => '1');
	RF_Align_in <= RF_Align and Mask;
	-- END
end archi;
-- END FILE