package GenericComm;

import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;

public class SocketComm implements Comm {
	public Socket 			sock;
	public int 		port;
	public InputStream 	is;
	public OutputStream 	os;
	
	public int setSpeed(int speed) {return 1;}
	
	public String[] getComList() {return null;}
	
	public String[] getComListDevDesc() {return null;}
	
	public void reset() throws Exception {};
	
	public int init(int pcomSpeed, boolean prts_cts, String pcomName) {return 1;}
	public int init(int pcomSpeed, boolean prts_cts, String pcomName, int stopbit) {return 1;}
	
	public int open(String name) {
		try {
			sock=new Socket(name, port);
			is=sock.getInputStream();
			os=sock.getOutputStream();
			return 1;
		} catch(Exception e) {
			System.out.println("Exception caught in SocketComm:open");
			e.printStackTrace();
			return 0;
		}
	}
	
	public int read(byte buf[], int off, int size) {
		if(sock.isConnected()==false) return -1;

		try {
			return is.read(buf, off, size);
		} catch(Exception e) {
			System.out.println("Exception caught in SocketComm:read");
			e.printStackTrace();
			return -1;			
		}
	}

	public int write(byte buf[], int off, int size) {
		try {
			os.write(buf, off, size);
			return 1;
		} catch(Exception e) {
			System.out.println("Exception caught in SocketComm:write");
			e.printStackTrace();
			return 0;
		}
	}

	public int availableIn() {
		if(sock.isConnected()==false) return -1;
		try {
			return is.available();
		} catch (Exception e) {
			System.out.println("Exception caught in SocketComm:availableIn");
			e.printStackTrace();
			return -1;			
		}
	}

	public int availableOut() {
		return 1;
	}
	
	public void close() {
		try {
			sock.close();
			is=null;
			os=null;
			sock=null;
		} catch(Exception e) {
			System.out.println("Exception caught in SocketComm:close");
			e.printStackTrace();			
		}
	}

}
