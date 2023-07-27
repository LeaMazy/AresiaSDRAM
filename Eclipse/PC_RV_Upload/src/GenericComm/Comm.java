
package GenericComm;


/**
 * The <code>Comm</code> Interface is a generic class of all communication peripherals
 *
 */
public interface Comm  {
	
	public int init(int pcomSpeed, boolean prts_cts, String pcomName);

	public int init(int pcomSpeed, boolean prts_cts, String pcomName, int StopBit);

    		/**
		* Write in comm device.
     		* Send a packet of bytes. This function will block the thread until all the bytes are sent (or buffered for transmission).
     		*
     		* @param buf  buffer containing bytes to send
     		* @param off  first byte index to send in buf (offset)
     		* @param len  number of bytes to send
     		* @return  the number of bytes sent
     		* @since   BORNEO1.0
     		*/
		public int write(byte buf[], int off, int len) throws Exception;

    		/**
		* read from comm device.
     		* Receive a packet of bytes. This function will block the thread until all the bytes are received.
     		*
     		* @param buf  buffer where received bytes are stored
     		* @param off  first byte index where byte are stored (offset)
     		* @param len  number of bytes to receive
     		* @return  the number of bytes received
     		* @since   BORNEO1.0
     		*/
		public int read(byte buf[], int off, int len) throws Exception;

		// Available (busy) bytes in input buffer

    		/**
		* Read the number of bytes ready in the input comm device.
     		* This function is used to know the number of available byte received in the input comm device without blocking the thread.
     		*
     		* @return  the number of available bytes received
     		* @since   BORNEO1.0
     		*/
		public int availableIn() throws Exception;

    		/**
		* Read the number of byte ready to be sent in the output comm device without blocking the thread.
     		*
     		* @return  the number of bytes 
     		* @since   BORNEO1.0
     		*/
		public int availableOut() throws Exception;

    		/**
		* Close the comm device
     		*
     		* @return  reserved 
     		* @since   BORNEO1.0
     		*/
		public void close();

    		/**
		* Open the comm device
     		*
     		* @param name  virtual name of the connection
     		* @return  0 if the connection is not opened 
     		* @since   BORNEO1.0
     		*/
		public int open(String name);
		
		/**
	* Set speed transaction
 		*
 		* @param speed  speed in baud rate
 		* @return  0 if the connection is not opened 
 		* @since   BORNEO1.0
 		*/
	public int setSpeed(int speed);
	
	public String[] getComList();
	
	public String[] getComListDevDesc();
	
	public void reset() throws Exception;
	
}
