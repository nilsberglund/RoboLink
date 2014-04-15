package robolinkControlPanel;

import jssc.SerialPort;
import jssc.SerialPortList;
import jssc.SerialPortException;

public class JSSCexample {

	GUI2 window = null;

	public JSSCexample(GUI2 window)
	{
		this.window = window;
	}

	public void searchForPorts() {
		String[] portNames = SerialPortList.getPortNames();
		for(int i = 0; i < portNames.length; i++){
			window.cboxPorts.addItem(portNames[i]);
		}
	}

	public void connect(){
		SerialPort serialPort = new SerialPort("COM5");
		try {
			serialPort.openPort();//Open serial port
			serialPort.setParams(SerialPort.BAUDRATE_9600, 
					SerialPort.DATABITS_8,
					SerialPort.STOPBITS_1,
					SerialPort.PARITY_NONE);
		}
		catch (SerialPortException ex) {
			System.out.println(ex);
		}
	}
	
	public void disconnect(){
		serialPort.closePort();
	}
}