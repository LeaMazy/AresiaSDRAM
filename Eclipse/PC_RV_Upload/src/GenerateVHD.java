import java.io.File;
import java.io.FileOutputStream;

public class GenerateVHD {
	String vhdStart="LIBRARY ieee; USE ieee.std_logic_1164.ALL;\n"+ 
	"USE ieee.numeric_std.ALL;\n\n"+ 

	"ENTITY SRAM IS\n"+ 
	"PORT(\n\n"+ 
		
		"-- Inputs\n"+
		"CLK : IN std_logic;\n"+ 
		"W_DATA : IN std_logic_vector(31 DOWNTO 0);\n"+ 
		"W_ADDR : IN std_logic_vector(15 DOWNTO 0);\n"+
		"R_ADDR : IN std_logic_vector(15 DOWNTO 0);\n"+ 
		"WE : IN std_logic;\n\n"+ 
		
		"-- Outputs\n"+
		"R_DATA : OUT std_logic_vector(31 DOWNTO 0));\n\n"+ 

	"END SRAM;\n\n"+ 

	"ARCHITECTURE rtl OF SRAM IS\n"+ 
	"TYPE RAM IS ARRAY(0 TO 65535)\n"+ 
	"OF std_logic_vector(31 DOWNTO 0);\n"+ 

	"SIGNAL ram_block : RAM:= (\";";
	
	
	static int generateVHD(byte buf[], int addressSize) {
		FileOutputStream os;
		File file=new File("SRAM.vhd");
		int totalLen, bufIndex=0, v, lineIndex, wordIndex;
		
		totalLen=(int)Math.pow(2, addressSize);
		
		try {
			os=new FileOutputStream(file);
			
			String s="LIBRARY ieee; USE ieee.std_logic_1164.ALL;\n"+ 
					"USE ieee.numeric_std.ALL;\n\n"+ 

					"ENTITY SRAM IS\n"+ 
					"PORT(\n\n"+ 
						
						"-- Inputs\n"+
						"CLK : IN std_logic;\n"+ 
						"W_DATA : IN std_logic_vector(31 DOWNTO 0);\n"+ 
						"W_ADDR : IN std_logic_vector("+(addressSize-1)+" DOWNTO 0);\n"+
						"R_ADDR : IN std_logic_vector("+(addressSize-1)+" DOWNTO 0);\n"+ 
						"WE : IN std_logic;\n\n"+ 
						
						"-- Outputs\n"+
						"R_DATA : OUT std_logic_vector(31 DOWNTO 0));\n\n"+ 

					"END SRAM;\n\n"+ 

					"ARCHITECTURE rtl OF SRAM IS\n"+ 
					"TYPE RAM IS ARRAY(0 TO "+(totalLen-1)+")\n"+ 
					"OF std_logic_vector(31 DOWNTO 0);\n"+ 

					"SIGNAL ram_block : RAM:= (\n";
			
			
			os.write(s.getBytes());
			
			String line="", charWord;
			wordIndex=0;
			for(bufIndex=0, lineIndex=0; bufIndex<buf.length; ) {
				String charByte;
				
				v=(int)buf[bufIndex+3] & 0xff;
				charByte=String.format("%02X", v);
				charWord=""+charByte;
				
				v=(int)buf[bufIndex+2] & 0xff;
				charByte=String.format("%02X", v);
				charWord+=""+charByte;

				v=(int)buf[bufIndex+1] & 0xff;
				charByte=String.format("%02X", v);
				charWord+=""+charByte;

				v=(int)buf[bufIndex+0] & 0xff;
				charByte=String.format("%02X", v);
				charWord+=""+charByte;

				line+="x\""+charWord+"\"";
				
				bufIndex+=4;
				wordIndex++;
				if(wordIndex<totalLen) line+=", ";
				else break;
				
				lineIndex++;
				if(lineIndex>=10) {
					lineIndex=0;
					line+="\n";
					os.write(line.getBytes());
					line="";
				}
			}
			
			charWord="x\"00000000\"";
			for(; wordIndex<totalLen; ) {
				line+=charWord;

				wordIndex++;
				if(wordIndex<totalLen) line+=", ";
				else break;

				lineIndex++;
				if(lineIndex>=10) {
					lineIndex=0;
					line+="\n";
					os.write(line.getBytes());
					line="";
				}
			}
			line+="\n";
			os.write(line.getBytes());
			
			s=");\r\n" + 
					" \r\n" + 
					"BEGIN \r\n" + 
					"\r\n" + 
					"PROCESS (CLK) \r\n" + 
					"BEGIN \r\n" + 
					"	IF (CLK'event AND CLK = '1') \r\n" + 
					"	THEN \r\n" + 
					"		IF (WE = '1') \r\n" + 
					"		THEN \r\n" + 
					"			ram_block(to_integer(unsigned(W_ADDR))) <= W_DATA; \r\n" + 
					"		END IF; \r\n" + 
					"	END IF; \r\n" + 
					"	IF (CLK'event AND CLK = '1') \r\n" + 
					"	THEN \r\n" + 
					"		R_DATA <= ram_block(to_integer(unsigned(R_ADDR))); \r\n" + 
					"	END IF;\r\n" + 
					"END PROCESS; \r\n" + 
					"END rtl;";
			
			os.write(s.getBytes());
			
			
			os.close();
			
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		return 0;
		
		
	}
}
