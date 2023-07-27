import java.io.File;
import java.io.FileInputStream;

import javax.swing.JFrame;
import javax.swing.JOptionPane;

import GenericComm.GenericComm;

public class PC_RV_Upload {
	String fileName;
	GenericComm rxtx;
	String comName;
	byte fileBuf[];
	
	String VERSION="V1.19";
	
	static int DEBUG=0;
	static int BINARG=0;
	static int PACKETSIZE=4;
	static int BAUDRATE=115200;
	static int STOPBIT=1;
	static int VHDFILE=0;
	static int LEN=1024;
	
	

	void print(String s) {
		System.out.println(s);
	}
	
	void printByte(byte b) {
		if((b>='0' && b<='9') || (b>='A' && b<='Z') || (b>='a' && b<='z') ||  (b==' ')) {
			System.out.print((char)b);
			System.out.flush();
			return;
		}
		if(b==0x0d) { System.out.println(""); return; }
		System.out.print(" ("+Integer.toHexString((int)b & 0xff)+") ");
		System.out.flush();
	}

	static String BADARGS="Command Line= RV_Upload inputFile [-debug] [-bin] [-psize num(10)] [-baud num(115200)] [-2stop] [-vhdfile]";
	int getArgs(String args[]) {
		int i,j, nbargs;
		//i=args.length;
		//print("arglen="+i);
		//for(j=0; j<i; j++) print(""+args[j]);
		nbargs=args.length;
		if(nbargs<1) {
			print("bad arguments");
			return 0;
		}
		fileName=args[0];
		
		for(i=1; i<nbargs; i++) {
			if(args[i].indexOf("-debug")>=0) { DEBUG=1; continue;}
			if(args[i].indexOf("-bin")>=0) { BINARG=1; continue;}
			if(args[i].indexOf("-2stop")>=0) { STOPBIT=2; continue;}
			if(args[i].indexOf("-vhdfile")>=0) { VHDFILE=1; continue;}
			if(args[i].indexOf("-psize")>=0) { 
				i++;
				PACKETSIZE=Integer.parseInt(args[i]);
				continue;
			}
			if(args[i].indexOf("-baud")>=0) { 
				i++;
				BAUDRATE=Integer.parseInt(args[i]); 
				continue;
			}
			if(args[i].indexOf("-len")>=0) { 
				i++;
				LEN=Integer.parseInt(args[i]); 
				continue;
			}
		}		
		return 1;
	}
	
	void printArgs() {
		print("debug="+DEBUG+" bin="+BINARG+" Stopbits="+STOPBIT+" vhdfile="+VHDFILE+" psize="+PACKETSIZE+" baudrate="+BAUDRATE+" filename="+fileName+" len="+LEN);
	}
	

	
	String chooseCom( JFrame frame, Object[] comList) {
        
		String s = (String)JOptionPane.showInputDialog(
		                    frame,
		                    "Choose the Borneo serial COM:",
		                    "Serial COM Selector",
		                    JOptionPane.PLAIN_MESSAGE,
		                    null,
		                    comList,
		                    comList[0]);
		
		return s;
	}
	
	int openCom() {
		int i;
		rxtx= new GenericComm();

		int flag;
		String tmpStr=null;
		String portName[]=GenericComm.portList();
		for(i=0, flag=0; i< portName.length; i++) {
			if(portName[i]!=null) {
				flag++;
			}
		}

		if(flag==0) {
			print("Termine. Aucun port serie visible.");
			return 0;
		} else {
			if(flag==1) comName=portName[0];
			else {
				Object[] comList = new Object[flag];
				for(i=0, flag=0; i< portName.length; i++) {
					if(portName[i]!=null) {
						comList[flag]=portName[i];
						flag++;
					}
				}

				tmpStr=chooseCom(null, comList);
				comName=tmpStr;
			}
		}

		if(comName==null) {
			print("Termine. Aucun port selectionné.");
			return 0;
		} else {
			print("ouverture port "+comName);

			rxtx.init(BAUDRATE, false, comName, STOPBIT);
		}

		return 1;
	}
	
	int getByte(byte buf[], int index) {
		int c, v;
		
		c=(int)buf[index]; index++;
		if(c>='a' && c<='f') c=c-'a'+10;
		else if(c>='0' && c<='9') c=c-'0';			
		else return -1;
		v=c<<4;
		
		c=(int)buf[index]; index++;
		
		if(c>='a' && c<='f') c=c-'a'+10;
		else if(c>='0' && c<='9') c=c-'0';			
		else return -1;
		v|=c;
		
		return v;
	}
	
	int getByte2(byte buf[], int index) {
		int v, tmp;
		
		v=getByte(buf, index); index+=2;
		if(v<0) return -1;
		v=v<<8;
		
		tmp=getByte(buf, index); index+=2;
		if(tmp<0) return -1;
		v|=tmp;
		
		return v;
	}
	
	
	byte[] parseHexFileBuf(byte buf[]) {
		int i, rIndex, wIndex, inBufLen=buf.length, lineNum, nbByte, address, opcode,  v;
		byte retBuf[], tmpBuf[]=new byte[inBufLen];
		char c;
		
		for(rIndex=0, wIndex=0, lineNum=0; rIndex<inBufLen; lineNum++) {
			// start new line
			// get ':'

			c=(char)buf[rIndex]; rIndex++;
			if(c!=':') {
				print("HexFile doesn't start with ':' at line "+lineNum);
				return null;
			}
			
			// read number of bytes in data section (8 bits)
			nbByte=getByte(buf, rIndex); rIndex+=2;
			if(nbByte<0) {
				print("bad char (nbByte) at line "+lineNum);
				return null;
			}
			//if(DEBUG!=0) print("nbByte="+nbByte);
			
			// read address (16 bits)
			address=getByte2(buf, rIndex); rIndex+=4;
			if(address<0) {
				print("bad char (address) at line "+lineNum);
				return null;
			}
			
			
			rIndex+=2; //filler 00 before instruction
			//if(DEBUG!=0) print("address="+address);
			
			// read opcode
//			opcode=getByte(buf, rIndex); rIndex+=2;
//			if(opcode<0) {
//				print("bad char (opcode) at line "+lineNum);
//				return null;
//			}
			//if(DEBUG!=0) print("opcode="+opcode);
			
			// skip line if opcode is not equal to 0
//			if(opcode!=0) {
//				print("Warning : Unknown opcode at line "+lineNum);
//				do {
//					rIndex++;
//					if(rIndex>=inBufLen) break;
//					c=(char)buf[rIndex];
//				} while(c!=':');
//				continue;
//			}
			
			
			// read data
			if((nbByte & 3)!=0) {
				print("number of data is not modulo 4 at line "+lineNum);
				return null;
			}
			
			for(i=0; i<nbByte; i+=4) {
				v=getByte(buf, rIndex); rIndex+=2;
				if(v<0) {
					print("bad char1 (data) at line "+lineNum);
					return null;
				}
				//if(DEBUG!=0) print("data="+v);
				tmpBuf[wIndex]=(byte)v; wIndex++;
				
				v=getByte(buf, rIndex); rIndex+=2;
				if(v<0) {
					print("bad char2 (data) at line "+lineNum);
					return null;
				}
				//if(DEBUG!=0) print("data="+v);
				tmpBuf[wIndex]=(byte)v; wIndex++;
				
				v=getByte(buf, rIndex); rIndex+=2;
				if(v<0) {
					print("bad char3 (data) at line "+lineNum);
					return null;
				}
				//if(DEBUG!=0) print("data="+v);
				tmpBuf[wIndex]=(byte)v; wIndex++;
								
				v=getByte(buf, rIndex); rIndex+=2;
				if(v<0) {
					print("bad char4 (data) at line "+lineNum);
					return null;
				}
				//if(DEBUG!=0) print("data="+v);
				tmpBuf[wIndex]=(byte)v; wIndex++;
			}
			
			// skip end of line (crc)
			c=(char)buf[rIndex];
			while (c!=':'){
				rIndex++;
				if(rIndex>=inBufLen) break;
				c=(char)buf[rIndex];
			} 
		}
		
		retBuf=new byte[wIndex];
		System.arraycopy(tmpBuf, 0, retBuf, 0, wIndex);
		
		print("HexToBin : number of bytes in bin="+wIndex);
		
		// debug
		if(DEBUG!=0) {
			for(i=0; i<wIndex; ) {
				print("0x"+Integer.toHexString((int)retBuf[i]&0xff)+" 0x"+Integer.toHexString((int)retBuf[i+1]&0xff)+" 0x"+Integer.toHexString((int)retBuf[i+2]&0xff)+" 0x"+Integer.toHexString((int)retBuf[i+3]&0xff));
				i+=4;
			}
		}
		return retBuf;
	}
	
	int openAndReadFile(String fileName) {
    	File desc;
    	FileInputStream fi;
    	int size;
	
    	try {
	    	desc=new File(fileName);
	    	if(desc!=null) {
	    		if((LEN+1)*21-2 <= desc.length()) {
					size=(LEN+1)*21-2;
	    		}else {
					size=(int)desc.length();
	    		}
//				print("len: "+ desc.length());
//				print("size: "+ size);
				fileBuf=new byte[size];
				fi=new FileInputStream(desc);
				fi.read(fileBuf);
				fi.close();
				return 1;
	    	} else {
	    		print("Unable to open file : "+fileName);
	    		return 0;
	    	}
		} catch(Exception e) {
			print("Exception while reading file : "+fileName);
			return 0;
		}
	}	
	
	int sendBuf(byte buf[], GenericComm com) {
		int i=0, len=0, packetSize=0, n=0;
		byte locBuf[]=new byte[1024];
		long startTime;
		
		try {
			// send buf size
			len=buf.length;
			
			locBuf[0]=(byte)((len>>24) & 0xff);
			com.write(locBuf, 0, 1);
			com.read(locBuf, 1, 1);
			if(locBuf[0]!=locBuf[1]) {
				print("Error while sending size(0)");
				return 0;
			}

			locBuf[0]=(byte)((len>>16) & 0xff);
			com.write(locBuf, 0, 1);
			com.read(locBuf, 1, 1);
			if(locBuf[0]!=locBuf[1]) {
				print("Error while sending size(1)");
				return 0;
			}

			locBuf[0]=(byte)((len>>8) & 0xff);
			com.write(locBuf, 0, 1);
			com.read(locBuf, 1, 1);
			if(locBuf[0]!=locBuf[1]) {
				print("Error while sending size(2)");
				return 0;
			}

			locBuf[0]=(byte)((len) & 0xff);
			com.write(locBuf, 0, 1);
			com.read(locBuf, 1, 1);
			if(locBuf[0]!=locBuf[1]) {
				print("Error while sending size(3)");
				return 0;
			}

			/*
			// send buf core : interlaced
			int packetSize=1;
			print("Interlaced");
			startTime=System.currentTimeMillis();
			i=0;
			com.write(buf, i, packetSize); i++;
			for( ; i<len; ) {
				if(System.currentTimeMillis()-startTime>1000) {
					print("sending "+i+"/"+len);
					startTime=System.currentTimeMillis();
				}
				if((i+packetSize) > len) packetSize=len-i;
				com.write(buf, i, packetSize); i++;

				// check
				com.read(locBuf, 0, 1);
				if(locBuf[0]!=buf[i-2]) {
					print("Error while sending core pos="+i);
					print("emitted=0x"+Integer.toHexString((int)buf[i-2] & 0xff)+" received=0x"+Integer.toHexString((int)locBuf[0] & 0xff));
					return 0;
				}
			}
			*/
			

			/*
			// send buf core : by packet
			packetSize=PACKETSIZE;
			print("PacketSize="+packetSize);
			startTime=System.currentTimeMillis();
			for(i=0; i<len; ) {
				if(System.currentTimeMillis()-startTime>1000) {
					print("sending "+i+"/"+len);
					startTime=System.currentTimeMillis();
				}
				if((i+packetSize) >= len) packetSize=len-i;
				com.write(buf, i, packetSize);
				for(int j=0, startI=i; j<packetSize; ) {
					int av=com.availableIn();
					if(av>0) {
						com.read(locBuf, 0, av);
						for(n=0; n<av ;n++) {
							if(locBuf[n]!=buf[i]) {
								print("Error while sending core pos="+i+" packetPos="+j);
								print("emitted=0x"+Integer.toHexString((int)buf[i] & 0xff)+" received=0x"+Integer.toHexString((int)locBuf[n] & 0xff));
								String s="";
								for(i=startI,j=0; j<packetSize; i++, j++) s+="0x"+Integer.toHexString((int)buf[i] & 0xff)+" ";
								print("last packet sent: "+s);
								return 0;
							}
							i++; j++;
						}
					}
				}
			}
			*/

			// send buf core : by packet
			packetSize=PACKETSIZE;
			print("PacketSize="+packetSize);
			startTime=System.currentTimeMillis();
			for(i=0; i<len; ) {
				if(System.currentTimeMillis()-startTime>1000) {
					print("sending "+i+"/"+len);
					startTime=System.currentTimeMillis();
				}
				if((i+packetSize) >= len) packetSize=len-i;
				com.write(buf, i, packetSize);
				for(int j=i; j<i+packetSize; j++ ) {
					print(""+Integer.toHexString((int)buf[j] & 0xff));
				}
				for(int j=0, startI=i; j<packetSize; ) {
					com.read(locBuf, 0, 1);
					printByte(locBuf[0]);
					if(locBuf[0]!=buf[i]) {
							print("Error while sending core pos="+i+" packetPos="+j);
							print("emitted=0x"+Integer.toHexString((int)buf[i] & 0xff)+" received=0x"+Integer.toHexString((int)locBuf[0] & 0xff));
							String s="";
							for(i=startI,j=0; j<packetSize; i++, j++) s+="0x"+Integer.toHexString((int)buf[i] & 0xff)+" ";
							print("last packet sent: "+s);
							return 0;
					}
					i++; j++;
				}
			}

			
			
			// 
/*			startTime=System.currentTimeMillis();
			for(i=0;i<len;i++) {
				if(System.currentTimeMillis()-startTime>1000) {
					print("sending "+i+"/"+len);
					startTime=System.currentTimeMillis();
				}
				com.write(buf, i, 1);
				com.read(locBuf, 0, 1);
				if(locBuf[0]!=buf[i]) {
					print("Error while sending core pos="+i);
					return 0;
				}
			}
*/
			return 1;
		} catch(Exception e) {
			print("Error while sending buf on serial port exception="+e);
			print("packetsize="+packetSize+" n="+n+" i="+i+" len="+len);

			e.printStackTrace();
			return 0;
		}
		
	}
	
	public PC_RV_Upload(String args[]) {
		
		print("----- RV_Uploading tool : "+VERSION);
		print(BADARGS);
		
		// get command line arguments
		if(getArgs(args)<=0) {
			print("Bad Args");	
			return;
		}
		
		printArgs();
		
		// open file
		if(openAndReadFile(fileName)<=0) {
			return;
		}
		print("file="+fileName+" size(bytes)="+fileBuf.length);
		
		// convert Hex file into bin
		if(BINARG==0) {
			print("-- Convertint Hex to Bin");
			fileBuf=parseHexFileBuf(fileBuf);
		}
		
		if(VHDFILE==1) {
			print("-- Generating VHD file");
			GenerateVHD.generateVHD(fileBuf, 16);
			return;
		}
		
		// open Com
		print("-- Opening Serial Port at "+BAUDRATE+" bauds");
		if(openCom()<=0) {
			return;
		}
		
		// send Buf
		print("-- Sending File");
		if(sendBuf(fileBuf, rxtx)<=0) {
			return;
		}
		print("file sent OK");

		print("--- Console Mode ---");
		try {
			byte buf[]=new byte[4];
			while(true) {
				if(rxtx.availableIn()>0) {
					rxtx.read(buf, 0, 1);
					printByte(buf[0]);
					//print(""+Integer.toHexString((int)(buf[0]) & 0xff));
//					print((char)buf[0]+"("+((int)buf[0] & 0xff)+")");
				} else Thread.sleep(100);
			}
		} catch (Exception e) {
			print("Exception caught in console mode. Exiting.");
		}
	}
	
	public static void main(String args[]) {
		new PC_RV_Upload(args);
	}

}
