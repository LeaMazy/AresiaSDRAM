-- Projet de stage ING4 : RISC-V
-- ECE Paris / ARESIA
-- ALIGNMENT VHDL

-- LIBRARIES
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

-- ENTITY
entity Alignment is
	port (
		IDfunct3 	: in std_logic_vector(2 downto 0);  --data type (byte, half or word)
		q_b  			: in std_logic_vector(31 downto 0); --datamem output for load
		IDimm12I 	: in std_logic_vector(11 downto 0); --offset for load 
		IDimm12S 	: in std_logic_vector(11 downto 0); --offset for store
		RF_Align_out: in std_logic_vector(31 downto 0); --regfile output for store
		PROCaddrDM  : in std_logic_vector(31 downto 0); --datamem addr
		
		DQ 			: out std_logic_vector(3 downto 0);  --write enable for each ram (3 to 0)
		RF_Align_in	: out std_logic_vector(31 downto 0); --regfile input for load (shifted)
		PROCinputDM : out std_logic_vector(31 downto 0)  --datamem input for store (shifted)
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
	signal shiftL : integer;
	signal shiftS : std_logic_vector(2 downto 0);

begin
	-- Store
	shiftS 	 <= (IDimm12S(2 downto 0)) when PROCaddrDM(1 downto 0)="00" else
				    ('0' & PROCaddrDM(1 downto 0)); 
	storetype <= ("00" & shiftS(1 downto 0)) WHEN (IDfunct3 = "000" )	-- StoreByte (RAMs)
			else ("01" & shiftS(1 downto 0)) WHEN (IDfunct3 = "001")	-- StoreHalf (RAM3&2 ou RAM1&0)
			else ('1' & shiftS(2 downto 0)) WHEN (IDfunct3 = "010") -- StoreWord (RAM0)
			else ("0111"); -- (Ne prend jamais valeur 0111)
			
	dq_0 <= '1' WHEN ((storetype(3) ='1' or storetype(2 downto 0)="100" or storetype(3 downto 0)="0000"))
			else ('0');
	dq_1 <= '1' WHEN ((storetype(3) ='1' or storetype(2 downto 0)="100" or storetype(3 downto 0)="0001"))
			else ('0');	
	dq_2 <= '1' WHEN ((storetype(3) ='1' or storetype(2 downto 1)="11" or storetype(3 downto 0)="0010"))
			else ('0');
	dq_3 <= '1' WHEN ((storetype(3) ='1' or storetype(2 downto 1)="11" or storetype(3 downto 0)="0011"))
			else ('0');	
			
	DQ <= (dq_3 & dq_2 & dq_1 & dq_0);
				-- SHU
	PROCinputDM <= std_logic_vector(shift_left(unsigned(RF_Align_out),16)) 						
						when (IDfunct3 = "001" and (shiftS(1)='1')) -- SHU (Imm%2!=0 || (Imm%2=0 & Imm%4!=0))
				 else RF_Align_out 
						when (IDfunct3 = "001" and (shiftS(1 downto 0)="00")) -- SHU (Imm%4)
					-- SBU
				 else RF_Align_out
						when (IDfunct3 = "000" and (shiftS(1 downto 0)="00")) -- SBU (Imm%4=0)
				 else std_logic_vector(shift_left(unsigned(RF_Align_out),(to_integer(unsigned(shiftS(1 downto 0))))*8)) 
						when (IDfunct3 = "000" and ((shiftS(1 downto 0)/="00"))) -- SBU (Imm%4!=0)
					-- Else (SW or no shift)
				 else (RF_Align_out);
			
	-- Load		
				-- LHU
				
	shiftL <= 0 when (IDimm12I="111111111111" and (PROCaddrDM(1 downto 0)="00"))  else
				(to_integer(unsigned(IDimm12I(1 downto 0)))) when PROCaddrDM(1 downto 0)="00" else
				(to_integer(unsigned(PROCaddrDM(1 downto 0))));
	RF_Align <= std_logic_vector(shift_right(unsigned(q_b),16) ) 						
					when (IDfunct3 = "101" and ((IDimm12I(1)='1') 
													or	((IDimm12I(1)='0') and PROCaddrDM(1 downto 0)/="00"))) -- LHU (Imm%2=0 & Imm%4!=0)
			 else q_b 
					when (IDfunct3 = "101" and (IDimm12I(1 downto 0)="00") and PROCaddrDM(1 downto 0)="00") -- LHU (Imm%4)
				-- LBU
			 else q_b
					when (IDfunct3 = "100" and (IDimm12I(1 downto 0)="00") and PROCaddrDM(1 downto 0)="00") -- LBU (Imm%4=0)
			 else std_logic_vector(shift_right(unsigned(q_b),shiftL*8)) 
					when (IDfunct3 = "100" and ((IDimm12I(1 downto 0)/="00")
												  or ((IDimm12I(1 downto 0)="00") and PROCaddrDM(1 downto 0)/="00"))) -- LBU (Imm%4!=0)
				-- Else (LW or no shift)
			else (q_b);
	Mask <= "00000000000000001111111111111111" when (IDfunct3 = "101")
				else "00000000000000000000000011111111" when (IDfunct3 = "100")
				else (OTHERS => '1');
	RF_Align_in <= RF_Align and Mask;
	-- END
end archi;
-- END FILE