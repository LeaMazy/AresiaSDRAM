
package GenericComm;
 
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.Hashtable;

import com.fazecast.jSerialComm.*;


// http://fazecast.github.io/jSerialComm/
//08/01/2014 : Attention modif SimpleSerialRxTx ajout serial.setDTR(false); sinon Adeunis868 ne fonctionne pas
 
public class JSerialRxTx implements Comm {
	
	final static int DEBUG=		0;

	public SerialPort serialPort;
	public String name;
	
	InputStream is;
	OutputStream os;
	
	int 				comSpeed;
	boolean 			rts_cts;
	String				comName;
	

	public String[] getComList() {
		return portList();
	}

	public String[] getComListDevDesc() {
		try {
			int index=0;
			SerialPort 	portList[]=SerialPort.getCommPorts();
			String name;

			String[] tabName=new String[portList.length];


			for(index=0; index<portList.length; index++) {
				tabName[index]=portList[index].getPortDescription();
				System.out.println("SystemPortName="+portList[index].getSystemPortName()+" description="+portList[index].getPortDescription()+" descriptive="+portList[index].getDescriptivePortName());
			}
			
			return tabName;

		} catch(Exception e) {
			System.out.println("Exception caught in SimpleSerialRxTx e="+e);
			e.printStackTrace();
		}
		return null;
	}

	
	public int setRTS(boolean b) {
		try {
			if(b==false) {
				serialPort.setFlowControl(SerialPort.FLOW_CONTROL_DISABLED);
				serialPort.clearRTS();
			} else {
				serialPort.setFlowControl(SerialPort.FLOW_CONTROL_RTS_ENABLED | SerialPort.FLOW_CONTROL_CTS_ENABLED);
				serialPort.setRTS();
			}
			return 1;
		} catch(Exception e) {
			System.out.println("Exception e="+e);
			e.printStackTrace();
			return -1;
		}
	}
	
	static public String[] portList() {
		try {
			int index=0;
			SerialPort 	portList[]=SerialPort.getCommPorts();
			String name;

			String[] tabName=new String[portList.length];


			for(index=0; index<portList.length; index++) {
				tabName[index]=portList[index].getSystemPortName();
				if(DEBUG!=0) System.out.println("SystemPortName="+portList[index].getSystemPortName()+" description="+portList[index].getPortDescription()+" descriptive="+portList[index].getDescriptivePortName());
			}
			
			return tabName;

		} catch(Exception e) {
			System.out.println("Exception caught in SimpleSerialRxTx e="+e);
			e.printStackTrace();
		}
		return null;
	}
	
	public JSerialRxTx() {};

	
	//***************************************************************
	// find the first port of type PORT_SERIAL and connect to it
	//***************************************************************
	public int init(int pcomSpeed, boolean prts_cts, String pcomName) {
		try {
			int i;
			comSpeed=pcomSpeed;
			rts_cts=prts_cts;
			
			SerialPort portList[] = SerialPort.getCommPorts();
			serialPort=null;
			
			for(i=0; i<portList.length; i++) {
				name=portList[i].getSystemPortName();
				if((pcomName!=null && name.compareToIgnoreCase(pcomName)==0) || pcomName==null) {
					if(DEBUG!=0) System.out.println("opening "+name);
					comName=name;
					serialPort=portList[i];
					serialPort.setComPortParameters(pcomSpeed, 8, SerialPort.ONE_STOP_BIT, SerialPort.NO_PARITY);
//					serialPort.setComPortParameters(pcomSpeed, 8, SerialPort.TWO_STOP_BITS, SerialPort.NO_PARITY);
					if(prts_cts==true) serialPort.setFlowControl(SerialPort.FLOW_CONTROL_RTS_ENABLED | SerialPort.FLOW_CONTROL_CTS_ENABLED);
					serialPort.openPort();
					serialPort.setComPortTimeouts(serialPort.TIMEOUT_READ_BLOCKING, 1000000, 1000000);

					// AH 08/01/2014 : ajout pour ADEUNIS868
					serialPort.clearDTR();
					
					is=serialPort.getInputStream();
					os=serialPort.getOutputStream();
					return 1;
				}
			}			
		} catch(Exception e) {
			System.out.println("Exception caught in SimpleSerialRxTx e="+e);
			e.printStackTrace();
			if(serialPort != null) serialPort.closePort();
			serialPort=null;
		}
		return 0;
	}

	public int init(int pcomSpeed, boolean prts_cts, String pcomName, int stopbit) {
		try {
			int i;
			comSpeed=pcomSpeed;
			rts_cts=prts_cts;
			
			SerialPort portList[] = SerialPort.getCommPorts();
			serialPort=null;
			
			for(i=0; i<portList.length; i++) {
				name=portList[i].getSystemPortName();
				if((pcomName!=null && name.compareToIgnoreCase(pcomName)==0) || pcomName==null) {
					if(DEBUG!=0) System.out.println("opening "+name);
					comName=name;
					serialPort=portList[i];
					if(stopbit==1) serialPort.setComPortParameters(pcomSpeed, 8, SerialPort.ONE_STOP_BIT, SerialPort.NO_PARITY);
					else serialPort.setComPortParameters(pcomSpeed, 8, SerialPort.TWO_STOP_BITS, SerialPort.NO_PARITY);
					if(prts_cts==true) serialPort.setFlowControl(SerialPort.FLOW_CONTROL_RTS_ENABLED | SerialPort.FLOW_CONTROL_CTS_ENABLED);
					serialPort.openPort();
					serialPort.setComPortTimeouts(serialPort.TIMEOUT_READ_BLOCKING, 1000000, 1000000);

					// AH 08/01/2014 : ajout pour ADEUNIS868
					serialPort.clearDTR();
					
					is=serialPort.getInputStream();
					os=serialPort.getOutputStream();
					return 1;
				}
			}			
		} catch(Exception e) {
			System.out.println("Exception caught in SimpleSerialRxTx e="+e);
			e.printStackTrace();
			if(serialPort != null) serialPort.closePort();
			serialPort=null;
		}
		return 0;
	}

	public void reset() throws Exception {
		if(serialPort==null) return;
		close();
		Thread.sleep(1000);
		serialPort.openPort();
		
		
		// AH 08/01/2014 : ajout pour ADEUNIS868
		serialPort.clearDTR();

		is=serialPort.getInputStream();
		os=serialPort.getOutputStream();
	}
	
	//***************************************************************
	// return the port input stream
	//***************************************************************
	public InputStream getInputStream() {
		try {
			return serialPort.getInputStream();
		} catch (Exception e) {
			System.out.println("Exception caught in getInputStream e="+e);
			e.printStackTrace();
			return null;
		}
	}

	//***************************************************************
	// return the port output stream
	//***************************************************************
	public OutputStream getOutputStream() {
		try {
			return serialPort.getOutputStream();
		} catch (Exception e) {
			System.out.println("Exception caught in getOutputStream e="+e);
			e.printStackTrace();
			return null;
		}
	}
	
	public int write(byte buf[], int off, int len) throws Exception{
		os.write(buf, off, len);
		return 1;
	}

	public int read(byte buf[], int off, int len) throws Exception{
		return is.read(buf, off, len);
	} 
	
	public int availableIn() throws Exception {
		return is.available();
	}
	
	public int availableOut() throws Exception {
		return 0;
	}
	
	public int open(String name) {
		return 1;
	}
	
	public void close() {
		try {
			serialPort.closePort();
			is.close();
			os.close();
		} catch(Exception e) {
			
		}
	}
	
	public int setSpeed(int newSpeed) {
		try {
			
			comSpeed=newSpeed;
			serialPort.setBaudRate(newSpeed);
		} catch(Exception e) {
			e.printStackTrace();
			return -1;
		}
		return 1;
	}
	

}
