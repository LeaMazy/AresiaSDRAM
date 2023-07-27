package GenericComm;

import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.Hashtable;

import gnu.io.CommPortIdentifier;
import gnu.io.PortInUseException;
import gnu.io.SerialPort;

 
public class SimpleSerialRxTx {
	
	final static int DEBUG=		1;

	SerialPort serialPort;
	String name;

	//***************************************************************
	// find the first port of type PORT_SERIAL and connect to it
	//***************************************************************
	public SimpleSerialRxTx(int comSpeed, boolean rts_cts, String comName) {
		try {
			CommPortIdentifier portId;
			Enumeration portList = CommPortIdentifier.getPortIdentifiers();
			serialPort=null;
			while (portList.hasMoreElements()) {// open all available serial Ports and add to list
				portId = (CommPortIdentifier) portList.nextElement();
				if (portId.getPortType() == CommPortIdentifier.PORT_SERIAL) {
						name=portId.getName();
						if((comName!=null && name.compareToIgnoreCase(comName)==0) || comName==null) {
							if(DEBUG!=0) System.out.println("no comName defined, taking the first one");
							serialPort = (SerialPort) portId.open("RSInterface:" + name, 2000);
							serialPort.setSerialPortParams(comSpeed, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);
							if(rts_cts==true) serialPort.setFlowControlMode(SerialPort.FLOWCONTROL_RTSCTS_IN | SerialPort.FLOWCONTROL_RTSCTS_OUT);
							return;
						}
				}
				if(DEBUG!=0) System.out.println("No port found");
			}
		} catch(Exception e) {
			System.out.println("Exception caught in SimpleSerialRxTx e="+e);
			e.printStackTrace();
			if(serialPort != null) serialPort.close();
			serialPort=null;
		}
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

}
