/*
 * TODO: Telemetri: Stationmode/Transport mode shower: 
 * RFID shower

 */


package robolinkControlPanel;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Font;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSlider;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

public class GUI2 extends JFrame implements ChangeListener, ActionListener {


	private static final long serialVersionUID = 1L;

	//Communicator object
	Communicator2 communicator = null;
	//KeybindingController object
	KeybindingController2 keybindingController = null;

	//all the variables (buttons, labels etc.)
	public int speed = 1;
	
	//setting up instruction bytes and data bytes
	final public byte DRIVEINSTR = 1; //instruction byte for drive data
	final public byte ARMINSTR = 2; //instruction byte for arm data
	final public byte CALINSTR = 3; //instruction byte for calibration data
	final public byte PCONINSTR = 4; //instruction byte for pickup control data
	final public byte KPINSTR = 5; //instruction byte for changing KP
	final public byte KDINSTR = 6; //instruction byte for changing KD
	final public byte MODEINSTR = 7; //instruction byte for changing auto/manual
	
	final public byte STOP = 0;
	final public byte BW1 = 1;
	final public byte BW2 = 9;
	final public byte FR1 = 2;
	final public byte FR2 = 10;
	final public byte FL1 = 3;
	final public byte FL2 = 11;
	final public byte FW1 = 4;
	final public byte FW2 = 12;
	final public byte RL = 6;
	final public byte RR = 5;
	
	final public byte SPICKUP = 1;
	final public byte EPICKUP = 2;
	final public byte APICKUP = 3;
	
	final public byte J1LB = 25;
	final public byte J1LS = 17;
	final public byte J1RB = 9;
	final public byte J1RS = 1;
	
	final public byte J2LB = 26;
	final public byte J2LS = 18;
	final public byte J2RB = 10;
	final public byte J2RS = 2;
	
	final public byte J3LB = 27;
	final public byte J3LS = 19;
	final public byte J3RB = 11;
	final public byte J3RS = 3;
	
	final public byte J4LB = 28;
	final public byte J4LS = 20;
	final public byte J4RB = 12;
	final public byte J4RS = 4;
	
	final public byte J5LB = 29;
	final public byte J5LS = 21;
	final public byte J5RB = 13;
	final public byte J5RS = 5;
	
	final public byte J6LS = 22;
	final public byte J6RS = 6;
	
	final public byte DPP = 32;
	final public byte DTP = 64;
	
	public JButton btnConnect;
	public JButton btnDisconnect;
	
	public JButton btnBodyStop;
	public JButton btnBodyForward;
	public JButton btnBodyBackward;
	public JButton btnBodyForwardLeft;
	public JButton btnBodyForwardRight;
	public JButton btnBodyRotateLeft;
	public JButton btnBodyRotateRight;
	
	public JButton btnCalibration;
	public JButton btnAutomaticMode;
	public JButton btnManualMode;
	public JButton btnEndPickup;
	public JButton btnStartPickup;
	public JButton btnAbortPickup;
	
	public JButton btnJoint1LB;
	public JButton btnJoint1LS;
	public JButton btnJoint1RB;
	public JButton btnJoint1RS;
	
	public JButton btnJoint2LB;
	public JButton btnJoint2LS;
	public JButton btnJoint2RB;
	public JButton btnJoint2RS;
	
	public JButton btnJoint3LB;
	public JButton btnJoint3LS;
	public JButton btnJoint3RB;
	public JButton btnJoint3RS;
	
	public JButton btnJoint4LB;
	public JButton btnJoint4LS;
	public JButton btnJoint4RB;
	public JButton btnJoint4RS;
	
	public JButton btnJoint5LB;
	public JButton btnJoint5LS;
	public JButton btnJoint5RB;
	public JButton btnJoint5RS;
	
	public JButton btnJoint6LS;
	public JButton btnJoint6RS;
	
	public JButton btnDPP;
	public JButton btnDTP;
	
	public JComboBox cboxPorts;
	public JTextArea txtLog;
	public JTextField txtKp;
	public JTextField txtKd;
	public JTextArea txtTel;
	
	public JLabel lblCalibration;
	public JLabel lblTel;
	public JLabel lblKp;
	public JLabel lblKd;
	
	public JLabel lblLED1;
	public JLabel lblLED2;
	public JLabel lblLED3;
	public JLabel lblLED4;
	public JLabel lblLED5;
	public JLabel lblLED6;
	public JLabel lblLED7;
	
	public JSlider speedSlider;

	//layout for whole window and panel variables
	public GridLayout grid;
	public JPanel conPanel;
	public JPanel navPanel;
	public JPanel telPanel;
	public JPanel ledPanel;
	public JPanel armPanel;
	public JPanel calPanel;
	public JPanel stationPanel;


	/** Creates new form GUI */
	public GUI2() {
		initComponents();
		createObjects();
		communicator.searchForPorts();
		keybindingController.toggleControls();
		//keybindingController.bindKeys();
	}


	/**
	 * Creates instances of the Communicator and Keybindingcontroller
	 */
	private void createObjects() {
		communicator = new Communicator2(this);
		keybindingController = new KeybindingController2(this);


	}

	private void initComponents() {
		
		//Instantiating buttons and adding text
		btnConnect = new JButton("Connect");
		btnDisconnect = new JButton("Disconnect");
		btnBodyBackward = new JButton("Backward");
		btnBodyForward = new JButton("Forward");
		btnBodyForwardLeft = new JButton("Forward Left");
		btnBodyForwardRight = new JButton("Forward Right");
		btnBodyRotateLeft = new JButton("Rotate Left");
		btnBodyRotateRight = new JButton("Rotate Right");
		btnBodyStop = new JButton("Stop");
		btnCalibration = new JButton("Calibrate");
		btnAutomaticMode = new JButton("Automatic Mode");
		btnManualMode = new JButton("Manual Mode");
		btnEndPickup = new JButton("End Pickup");
		btnStartPickup = new JButton("Start Pickup");
		btnAbortPickup = new JButton("Leave without initiation");
		
		btnJoint1LB = new JButton("Base <<");
		btnJoint1LS = new JButton("Base <");
		btnJoint1RB = new JButton("Base >>");
		btnJoint1RS = new JButton("Base >");
		
		btnJoint2LB = new JButton("Shoulder <<");
		btnJoint2LS = new JButton("Shoulder <");
		btnJoint2RB = new JButton("Shoulder >>");
		btnJoint2RS = new JButton("Shoulder >");
		
		btnJoint3LB = new JButton("Elbow <<");
		btnJoint3LS = new JButton("Elbow <");
		btnJoint3RB = new JButton("Elbow >>");
		btnJoint3RS = new JButton("Elbow >");
		
		btnJoint4LB = new JButton("Wrist Tilt <<");
		btnJoint4LS = new JButton("Wrist Tilt <");
		btnJoint4RB = new JButton("Wrist Tilt >>");
		btnJoint4RS = new JButton("Wrist Tilt >");
		
		btnJoint5LB = new JButton("Wrist Rotate <<");
		btnJoint5LS = new JButton("Wrist Rotate <");
		btnJoint5RB = new JButton("Wrist Rotate >>");
		btnJoint5RS = new JButton("Wrist Rotate >");
		
		btnJoint6LS = new JButton("Release");
		btnJoint6RS = new JButton("Grip");
		
		btnDPP = new JButton("DPP");
		btnDTP = new JButton("DTP");
		
		txtKp = new JTextField();
		txtKd = new JTextField();
		
		
		//setting action commands
		btnConnect.setActionCommand("connect");
		btnDisconnect.setActionCommand("disconnect");
		btnBodyBackward.setActionCommand("BW");
		btnBodyForward.setActionCommand("FW");
		btnBodyForwardLeft.setActionCommand("FL");
		btnBodyForwardRight.setActionCommand("FR");
		btnBodyRotateLeft.setActionCommand("RL");
		btnBodyRotateRight.setActionCommand("RR");
		btnBodyStop.setActionCommand("stop");
		btnCalibration.setActionCommand("calibration");
		btnAutomaticMode.setActionCommand("automaticmode");
		btnManualMode.setActionCommand("manualmode");
		btnEndPickup.setActionCommand("endpickup");
		btnStartPickup.setActionCommand("startpickup");
		btnAbortPickup.setActionCommand("abortpickup");
		
		btnJoint1LB.setActionCommand("J1LB");
		btnJoint1LS.setActionCommand("J1LS");
		btnJoint1RB.setActionCommand("J1RB");
		btnJoint1RS.setActionCommand("J1RS");
		
		btnJoint2LB.setActionCommand("J2LB");
		btnJoint2LS.setActionCommand("J2LS");
		btnJoint2RB.setActionCommand("J2RB");
		btnJoint2RS.setActionCommand("J2RS");
		
		btnJoint3LB.setActionCommand("J3LB");
		btnJoint3LS.setActionCommand("J3LS");
		btnJoint3RB.setActionCommand("J3RB");
		btnJoint3RS.setActionCommand("J3RS");
		
		btnJoint4LB.setActionCommand("J4LB");
		btnJoint4LS.setActionCommand("J4LS");
		btnJoint4RB.setActionCommand("J4RB");
		btnJoint4RS.setActionCommand("J4RS");
		
		btnJoint5LB.setActionCommand("J5LB");
		btnJoint5LS.setActionCommand("J5LS");
		btnJoint5RB.setActionCommand("J5RB");
		btnJoint5RS.setActionCommand("J5RS");
		
		btnJoint6LS.setActionCommand("J6LS");
		btnJoint6RS.setActionCommand("J6RS");
		
		btnDPP.setActionCommand("DPP");
		btnDTP.setActionCommand("DTP");
		
		txtKp.setActionCommand("KP");
		txtKd.setActionCommand("KD");
		
		
		//Instantiating the sliders, text areas, labels etc
		speedSlider = new JSlider(JSlider.HORIZONTAL, 1, 2, 1); //vertical layout, minlvl 1, maxlvl 2, start 1

		cboxPorts = new JComboBox();
		txtLog = new JTextArea();
		txtTel = new JTextArea();
		lblTel = new JLabel();
		lblKp = new JLabel("Kp:");
		lblKd = new JLabel("Kd:");
		lblCalibration = new JLabel();
	
				
		lblLED1 = new JLabel("O");
		lblLED2 = new JLabel("O");
		lblLED3 = new JLabel("O");
		lblLED4 = new JLabel("O");
		lblLED5 = new JLabel("O");
		lblLED6 = new JLabel("O");
		lblLED7 = new JLabel("O");

		
		Font dotFont = new Font("SansSerif", Font.BOLD, 28);
		lblLED1.setFont(dotFont);
		lblLED2.setFont(dotFont);
		lblLED3.setFont(dotFont);
		lblLED4.setFont(dotFont);
		lblLED5.setFont(dotFont);
		lblLED6.setFont(dotFont);
		lblLED7.setFont(dotFont);
		
		grid = new GridLayout(2, 3);
		conPanel = new JPanel();
		navPanel = new JPanel();
		telPanel = new JPanel();
		ledPanel = new JPanel();
		armPanel = new JPanel();
		calPanel = new JPanel();
		stationPanel = new JPanel();
		
		//Adding action listeners
		btnConnect.addActionListener(this);
		btnDisconnect.addActionListener(this);
		btnBodyStop.addActionListener(this);
		btnBodyForward.addActionListener(this);
		btnBodyBackward.addActionListener(this);
		btnBodyForwardLeft.addActionListener(this);
		btnBodyForwardRight.addActionListener(this);
		btnBodyRotateLeft.addActionListener(this);
		btnBodyRotateRight.addActionListener(this);
		btnCalibration.addActionListener(this);
		btnAutomaticMode.addActionListener(this);
		btnManualMode.addActionListener(this);
		btnEndPickup.addActionListener(this);
		btnStartPickup.addActionListener(this);
		btnAbortPickup.addActionListener(this);
		btnJoint1LB.addActionListener(this);
		btnJoint1LS.addActionListener(this);
		btnJoint1RB.addActionListener(this);
		btnJoint1RS.addActionListener(this);
		btnJoint2LB.addActionListener(this);
		btnJoint2LS.addActionListener(this);
		btnJoint2RB.addActionListener(this);
		btnJoint2RS.addActionListener(this);
		btnJoint3LB.addActionListener(this);
		btnJoint3LS.addActionListener(this);
		btnJoint3RB.addActionListener(this);
		btnJoint3RS.addActionListener(this);
		btnJoint4LB.addActionListener(this);
		btnJoint4LS.addActionListener(this);
		btnJoint4RB.addActionListener(this);
		btnJoint4RS.addActionListener(this);
		btnJoint5LB.addActionListener(this);
		btnJoint5LS.addActionListener(this);
		btnJoint5RB.addActionListener(this);
		btnJoint5RS.addActionListener(this);
		btnJoint6LS.addActionListener(this);
		btnJoint6RS.addActionListener(this);
		btnDPP.addActionListener(this);
		btnDTP.addActionListener(this);
		txtKp.addActionListener(this);
		txtKd.addActionListener(this);


		//CONPANEL
		
		txtLog.setEditable(false);
		txtLog.setLineWrap(true);
		txtLog.setFocusable(false);
		txtLog.setRows(10);
		txtLog.setColumns(20);
		conPanel.add(btnConnect);
		conPanel.add(btnDisconnect);
		conPanel.add(cboxPorts);
		conPanel.add(txtLog);

		//NAVPANEL

		//Setting up the slider
		speedSlider.addChangeListener(this);
		speedSlider.setMajorTickSpacing(1);
		speedSlider.setMinorTickSpacing(1);
		speedSlider.setPaintLabels(true);
		speedSlider.setPaintTicks(true);


		//drive buttons and labels
		navPanel.add(btnAutomaticMode);
		navPanel.add(btnManualMode);
		navPanel.add(btnBodyForward);
		navPanel.add(btnBodyBackward);
		navPanel.add(btnBodyStop);
		navPanel.add(btnBodyForwardLeft);
		navPanel.add(btnBodyForwardRight);
		navPanel.add(btnBodyRotateLeft);
		navPanel.add(btnBodyRotateRight);
		navPanel.add(speedSlider);

		//STATIONPANEL
		
		//pickup control buttons and label
		stationPanel.add(btnStartPickup);
		stationPanel.add(btnEndPickup);
		stationPanel.add(btnAbortPickup);

		//CALPANEL
		
		lblCalibration.setText("First floor, then line:");
		calPanel.add(lblCalibration);
		calPanel.add(btnCalibration);
		
		txtKp.setColumns(5);
		txtKd.setColumns(5);
		calPanel.add(lblKp);
		calPanel.add(txtKp);
		calPanel.add(lblKd);
		calPanel.add(txtKd);
		
		
		//ARMPANEL
		
		//arm buttons and label
		armPanel.add(btnDPP);
		armPanel.add(btnDTP);
		armPanel.add(btnJoint1LB);
		armPanel.add(btnJoint1LS);
		armPanel.add(btnJoint1RS);
		armPanel.add(btnJoint1RB);
		
		armPanel.add(btnJoint2LB);
		armPanel.add(btnJoint2LS);
		armPanel.add(btnJoint2RS);
		armPanel.add(btnJoint2RB);
		
		armPanel.add(btnJoint3LB);
		armPanel.add(btnJoint3LS);
		armPanel.add(btnJoint3RS);
		armPanel.add(btnJoint3RB);
		
		armPanel.add(btnJoint4LB);
		armPanel.add(btnJoint4LS);
		armPanel.add(btnJoint4RS);
		armPanel.add(btnJoint4RB);
		
		armPanel.add(btnJoint5LB);
		armPanel.add(btnJoint5LS);
		armPanel.add(btnJoint5RS);
		armPanel.add(btnJoint5RB);
		
		armPanel.add(btnJoint6LS);
		armPanel.add(btnJoint6RS);
		
		
		//TELPANEL
		
		lblTel.setText("TELEMETRY");
		lblLED1.setForeground(Color.WHITE);
		lblLED2.setForeground(Color.WHITE);
		lblLED3.setForeground(Color.WHITE);
		lblLED4.setForeground(Color.WHITE);
		lblLED5.setForeground(Color.WHITE);
		lblLED6.setForeground(Color.WHITE);
		lblLED7.setForeground(Color.WHITE);
		ledPanel.add(lblLED1);
		ledPanel.add(lblLED2);
		ledPanel.add(lblLED3);
		ledPanel.add(lblLED4);
		ledPanel.add(lblLED5);
		ledPanel.add(lblLED6);
		ledPanel.add(lblLED7);
		
		txtTel.setEditable(false);
		txtTel.setLineWrap(true);
		txtTel.setFocusable(false);
		txtTel.setRows(10);
		txtTel.setColumns(20);
	
		
		ledPanel.setBackground(Color.BLACK);
		telPanel.setLayout(new BorderLayout());
		telPanel.add(lblTel, BorderLayout.PAGE_START);
		telPanel.add(ledPanel, BorderLayout.PAGE_END);
		telPanel.add(txtTel, BorderLayout.CENTER);		


		//setting up the whole window
		this.setLayout(grid);
		this.add(conPanel);
		this.add(navPanel);
		this.add(armPanel);
		this.add(telPanel);
		this.add(calPanel);
		this.add(stationPanel);
	
		this.setTitle("Robolink Master Control Panel");


		this.setSize(1200,700);

		setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);

	}

	//slider function
	@Override
	public void stateChanged(ChangeEvent e) {
		JSlider source = (JSlider)e.getSource();
		if (!source.getValueIsAdjusting()) {
			speed = (int)source.getValue();
		}
	}
	
	//functions for the buttons
	@Override
	public void actionPerformed(ActionEvent e) {
		if("connect".equals(e.getActionCommand())){
			communicator.connect();
			if (communicator.getConnected() == true) {
				if (communicator.initIOStream() == true) {
					communicator.initListener();
				}
			}
		}
		else if ("disconnect".equals(e.getActionCommand())) {
			communicator.disconnect();
		}
		else if ("stop".equals(e.getActionCommand())) {
			communicator.writeData(DRIVEINSTR);
			communicator.writeData(STOP);
		}
		else if ("FW".equals(e.getActionCommand())) {
			communicator.writeData(DRIVEINSTR);
			switch (speed) {
			case 1:
				communicator.writeData(FW1);
				break;
			case 2:
				communicator.writeData(FW2);
				break;
			default:
				break;
			}
		}
		else if ("BW".equals(e.getActionCommand())) {
			communicator.writeData(DRIVEINSTR);
			switch (speed) {
			case 1:
				communicator.writeData(BW1);
				break;
			case 2:
				communicator.writeData(BW2);
				break;
			default:
				break;
			}
		}
		else if ("FL".equals(e.getActionCommand())) {
			communicator.writeData(DRIVEINSTR);
			switch (speed) {
			case 1:
				communicator.writeData(FL1);
				break;
			case 2:
				communicator.writeData(FL2);
				break;
			default:
				break;
			}
		}
		else if ("FR".equals(e.getActionCommand())) {
			communicator.writeData(DRIVEINSTR);
			switch (speed) {
			case 1:
				communicator.writeData(FR1);
				break;
			case 2:
				communicator.writeData(FR2);
				break;
			default:
				break;
			}
		}
		else if ("RL".equals(e.getActionCommand())) {
			communicator.writeData(DRIVEINSTR);
			communicator.writeData(RL);
		}
		else if ("RR".equals(e.getActionCommand())) {
			communicator.writeData(DRIVEINSTR);
			communicator.writeData(RR);
		}
		else if ("automaticmode".equals(e.getActionCommand())) {
			communicator.writeData(MODEINSTR);
			btnAutomaticMode.setEnabled(false);
			btnManualMode.setEnabled(true);
		}
		else if ("manualmode".equals(e.getActionCommand())) {
			communicator.writeData(MODEINSTR);
			btnAutomaticMode.setEnabled(true);
			btnManualMode.setEnabled(false);
		}
		else if ("calibration".equals(e.getActionCommand())) {
			communicator.writeData(CALINSTR);
		}
		else if ("endpickup".equals(e.getActionCommand())) {
			communicator.writeData(PCONINSTR);
			communicator.writeData(EPICKUP);
		}
		else if ("startpickup".equals(e.getActionCommand())) {
			communicator.writeData(PCONINSTR);
			communicator.writeData(SPICKUP);
		}
		else if ("abortpickup".equals(e.getActionCommand())) {
			communicator.writeData(PCONINSTR);
			communicator.writeData(APICKUP);
		}
		else if ("J1LB".equals(e.getActionCommand())) {
			communicator.writeData(ARMINSTR);
			communicator.writeData(J1LB);
		}
		else if ("J1LS".equals(e.getActionCommand())) {
			communicator.writeData(ARMINSTR);
			communicator.writeData(J1LS);
		}
		else if ("J1RB".equals(e.getActionCommand())) {
			communicator.writeData(ARMINSTR);
			communicator.writeData(J1RB);
		}
		else if ("J1RS".equals(e.getActionCommand())) {
			communicator.writeData(ARMINSTR);
			communicator.writeData(J1RS);
		}
		else if ("J2LB".equals(e.getActionCommand())) {
			communicator.writeData(ARMINSTR);
			communicator.writeData(J2LB);
		}
		else if ("J2LS".equals(e.getActionCommand())) {
			communicator.writeData(ARMINSTR);
			communicator.writeData(J2LS);
		}
		else if ("J2RB".equals(e.getActionCommand())) {
			communicator.writeData(ARMINSTR);
			communicator.writeData(J2RB);
		}
		else if ("J2RS".equals(e.getActionCommand())) {
			communicator.writeData(ARMINSTR);
			communicator.writeData(J2RS);
		}
		else if ("J3LB".equals(e.getActionCommand())) {
			communicator.writeData(ARMINSTR);
			communicator.writeData(J3LB);
		}
		else if ("J3LS".equals(e.getActionCommand())) {
			communicator.writeData(ARMINSTR);
			communicator.writeData(J3LS);
		}
		else if ("J3RB".equals(e.getActionCommand())) {
			communicator.writeData(ARMINSTR);
			communicator.writeData(J3RB);
		}
		else if ("J3RS".equals(e.getActionCommand())) {
			communicator.writeData(ARMINSTR);
			communicator.writeData(J3RS);
		}
		else if ("J4LB".equals(e.getActionCommand())) {
			communicator.writeData(ARMINSTR);
			communicator.writeData(J4LB);
		}
		else if ("J4LS".equals(e.getActionCommand())) {
			communicator.writeData(ARMINSTR);
			communicator.writeData(J4LS);
		}
		else if ("J4RB".equals(e.getActionCommand())) {
			communicator.writeData(ARMINSTR);
			communicator.writeData(J4RB);
		}
		else if ("J4RS".equals(e.getActionCommand())) {
			communicator.writeData(ARMINSTR);
			communicator.writeData(J4RS);
		}
		else if ("J5LB".equals(e.getActionCommand())) {
			communicator.writeData(ARMINSTR);
			communicator.writeData(J5LB);
		}
		else if ("J5LS".equals(e.getActionCommand())) {
			communicator.writeData(ARMINSTR);
			communicator.writeData(J5LS);
		}
		else if ("J5RB".equals(e.getActionCommand())) {
			communicator.writeData(ARMINSTR);
			communicator.writeData(J5RB);
		}
		else if ("J5RS".equals(e.getActionCommand())) {
			communicator.writeData(ARMINSTR);
			communicator.writeData(J5RS);
		}
		else if ("J6LS".equals(e.getActionCommand())) {
			communicator.writeData(ARMINSTR);
			communicator.writeData(J6LS);
		}
		else if ("J6RS".equals(e.getActionCommand())) {
			communicator.writeData(ARMINSTR);
			communicator.writeData(J6RS);
		}
		else if ("DPP".equals(e.getActionCommand())) {
			communicator.writeData(ARMINSTR);
			communicator.writeData(DPP);
		}
		else if ("DTP".equals(e.getActionCommand())) {
			communicator.writeData(ARMINSTR);
			communicator.writeData(DTP);
		}
		else if ("KP".equals(e.getActionCommand())) {
			byte tmpKp = Byte.valueOf(txtKp.getText());
			communicator.writeData(KPINSTR);
			communicator.writeData(tmpKp);
		}
		else if ("KD".equals(e.getActionCommand())) {
			byte tmpKd = Byte.valueOf(txtKd.getText());
			communicator.writeData(KDINSTR);
			communicator.writeData(tmpKd);
		}
	}
	
	public void paintLED(byte ledByte){
		
		int led7 = (ledByte & 0x01);
		if (led7 == 1) {
			lblLED7.setForeground(Color.RED);
		}
		else
		{
			lblLED7.setForeground(Color.WHITE);
		}
		
		int led6 = ((ledByte >> 1) & 0x01);
		if (led6 == 1) {
			lblLED6.setForeground(Color.RED);
		}
		
		else
		{
			lblLED6.setForeground(Color.WHITE);
		}
		
		int led5 = ((ledByte >> 2) & 0x01);
		if (led5 == 1) {
			lblLED5.setForeground(Color.RED);
		}
		else
		{
			lblLED5.setForeground(Color.WHITE);
		}
		
		int led4 = ((ledByte >> 3) & 0x01);
		if (led4 == 1) {
			lblLED4.setForeground(Color.RED);
		}
		else
		{
			lblLED4.setForeground(Color.WHITE);
		}
		
		int led3 = ((ledByte >> 4) & 0x01);
		if (led3 == 1) {
			lblLED3.setForeground(Color.RED);
		}
		else
		{
			lblLED3.setForeground(Color.WHITE);
		}
		
		int led2 = ((ledByte >> 5) & 0x01);
		if (led2 == 1) {
			lblLED2.setForeground(Color.RED);
		}
		else
		{
			lblLED2.setForeground(Color.WHITE);
		}
		
		int led1 = ((ledByte >> 6) & 0x01);
		if (led1 == 1) {
			lblLED1.setForeground(Color.RED);
		}
		else
		{
			lblLED1.setForeground(Color.WHITE);
		}
		
		this.repaint();
		
	}


	public static void main(String[] args) {
		java.awt.EventQueue.invokeLater(new Runnable() {
			public void run() {
				new GUI2().setVisible(true);
			}
		});

	}


	

}
