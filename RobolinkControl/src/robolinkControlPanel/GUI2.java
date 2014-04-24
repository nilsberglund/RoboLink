package robolinkControlPanel;

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
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

public class GUI2 extends JFrame implements ChangeListener, ActionListener {


	private static final long serialVersionUID = 1L;

	//Communicator object
	Communicator2 communicator = null;
	//KeybindingController object
	KeybindingController2 keybindingController = null;

	//all the variables (buttons, labels etc.)
	public int speed;
	
	public JButton btnConnect;
	public JButton btnDisconnect;
	
	public JButton btnBodyStop;
	public JButton btnBodyForward;
	public JButton btnBodyBackward;
	public JButton btnBodyForwardLeft;
	public JButton btnBodyForwardRight;
	
	public JButton btnCalibration;
	
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
	
	public JButton btnJoint6LB;
	public JButton btnJoint6LS;
	public JButton btnJoint6RB;
	public JButton btnJoint6RS;
	
	public JButton btnDPP;
	
	public JComboBox cboxPorts;
	public JTextArea txtLog;
	public JTextArea txtTel;
	public JLabel lblDrive;
	public JLabel lblCalibration;
	public JLabel lblArm;
	public JSlider speedSlider;

	//layout for whole window and panel variables
	public GridLayout grid;
	public JPanel conPanel;
	public JPanel navPanel;
	public JPanel telPanel;


	/** Creates new form GUI */
	public GUI2() {
		initComponents();
		createObjects();
		communicator.searchForPorts();
		keybindingController.toggleControls();
		//keybindingController.bindKeys();
	}


	/**
	 * Creates instances of the communicator and keybindingcontroller
	 */
	private void createObjects() {
		communicator = new Communicator2(this);
		keybindingController = new KeybindingController2(this);


	}

	private void initComponents() {

		btnConnect = new JButton();
		btnDisconnect = new JButton();
		btnBodyBackward = new JButton("BW");
		btnBodyForward = new JButton("FW");
		btnBodyForwardLeft = new JButton("FL");
		btnBodyForwardRight = new JButton("FR");
		btnBodyStop = new JButton("STOP");
		btnCalibration = new JButton("CAL");
		
		btnJoint1LB = new JButton("J1 <<");
		btnJoint1LS = new JButton("J1 <");
		btnJoint1RB = new JButton("J1 >>");
		btnJoint1RS = new JButton("J1 >");
		
		btnJoint2LB = new JButton("J2 <<");
		btnJoint2LS = new JButton("J2 <");
		btnJoint2RB = new JButton("J2 >>");
		btnJoint2RS = new JButton("J2 >");
		
		btnJoint3LB = new JButton("J3 <<");
		btnJoint3LS = new JButton("J3 <");
		btnJoint3RB = new JButton("J3 >>");
		btnJoint3RS = new JButton("J3 >");
		
		btnJoint4LB = new JButton("J4 <<");
		btnJoint4LS = new JButton("J4 <");
		btnJoint4RB = new JButton("J4 >>");
		btnJoint4RS = new JButton("J4 >");
		
		btnJoint5LB = new JButton("J5 <<");
		btnJoint5LS = new JButton("J5 <");
		btnJoint5RB = new JButton("J5 >>");
		btnJoint5RS = new JButton("J5 >");
		
		btnJoint6LB = new JButton("J6 <<");
		btnJoint6LS = new JButton("J6 <");
		btnJoint6RB = new JButton("J6 >>");
		btnJoint6RS = new JButton("J6 >");
		
		
		//setting action commands
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
		
		btnJoint6LB.setActionCommand("J6LB");
		btnJoint6LS.setActionCommand("J6LS");
		btnJoint6RB.setActionCommand("J6RB");
		btnJoint6RS.setActionCommand("J6RS");
		
		
		btnDPP = new JButton("DPP");
		
		speedSlider = new JSlider(JSlider.VERTICAL, 1, 5, 1); //vertical layout, minlvl 1, maxlvl 5, start 1

		cboxPorts = new JComboBox();
		txtLog = new JTextArea();
		txtTel = new JTextArea();
		lblDrive = new JLabel();
		lblArm = new JLabel();
		lblCalibration = new JLabel();

		grid = new GridLayout(0, 3);
		conPanel = new JPanel();
		navPanel = new JPanel();
		telPanel = new JPanel();


		//CONPANEL
		btnConnect.setText("CONNECT");
		btnDisconnect.setText("DISCONNECT");
		txtLog.setEditable(false);
		txtLog.setLineWrap(true);
		txtLog.setFocusable(false);
		txtLog.setRows(20);
		conPanel.add(btnConnect);
		conPanel.add(btnDisconnect);
		conPanel.add(cboxPorts);
		conPanel.add(txtLog);

		//NAVPANEL

		//all the label texts
		lblArm.setText("ARM");
		lblCalibration.setText("CALIBRATION");
		lblDrive.setText("DRIVE");

		//Setting up the slider
		speedSlider.addChangeListener(this);
		speedSlider.setMajorTickSpacing(5);
		speedSlider.setMinorTickSpacing(1);
		speedSlider.setPaintLabels(true);
		speedSlider.setPaintTicks(true);

		//calibration button and label
		navPanel.add(lblCalibration);
		navPanel.add(btnCalibration);

		//drive buttons and label
		navPanel.add(lblDrive);
		navPanel.add(btnBodyStop);
		navPanel.add(btnBodyForward);
		navPanel.add(btnBodyBackward);
		navPanel.add(btnBodyForwardLeft);
		navPanel.add(btnBodyForwardRight);
		navPanel.add(speedSlider);

		//arm buttons and label
		navPanel.add(lblArm);
		navPanel.add(btnJoint1LB);
		navPanel.add(btnJoint1LS);
		navPanel.add(btnJoint1RS);
		navPanel.add(btnJoint1RB);
		
		navPanel.add(btnJoint2LB);
		navPanel.add(btnJoint2LS);
		navPanel.add(btnJoint2RS);
		navPanel.add(btnJoint2RB);
		
		navPanel.add(btnJoint3LB);
		navPanel.add(btnJoint3LS);
		navPanel.add(btnJoint3RS);
		navPanel.add(btnJoint3RB);
		
		navPanel.add(btnJoint4LB);
		navPanel.add(btnJoint4LS);
		navPanel.add(btnJoint4RS);
		navPanel.add(btnJoint4RB);
		
		navPanel.add(btnJoint5LB);
		navPanel.add(btnJoint5LS);
		navPanel.add(btnJoint5RS);
		navPanel.add(btnJoint5RB);
		
		navPanel.add(btnJoint6LB);
		navPanel.add(btnJoint6LS);
		navPanel.add(btnJoint6RS);
		navPanel.add(btnJoint6RB);
		navPanel.add(btnDPP);


		//TELPANEL
		txtTel.setEditable(false);
		txtTel.setLineWrap(true);
		txtTel.setFocusable(false);
		txtTel.setColumns(15);
		txtTel.setRows(20);
		telPanel.add(txtTel);


		//setting up the whole window
		this.setLayout(grid);
		this.add(conPanel);
		this.add(navPanel);
		this.add(telPanel);
		this.setTitle("Robolink Master Control Panel");


		this.setSize(800, 600);

		setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);



		//functions for all the buttons

		//connect button
		btnConnect.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent evt) {
				btnConnectActionPerformed(evt);
			}

			private void btnConnectActionPerformed(ActionEvent evt) {
				communicator.connect();
				if (communicator.getConnected() == true) {
					if (communicator.initIOStream() == true) {
						communicator.initListener();
					}
				}
			}
		});

		//disconnect button
		btnDisconnect.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent evt) {
				btnDisconnectActionPerformed(evt);
			}

			private void btnDisconnectActionPerformed(ActionEvent evt) {
				communicator.disconnect();
			}
		});


		//Calibration button
		btnCalibration.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent evt) {
				btnCalibrationActionPerformed(evt);
			}

			private void btnCalibrationActionPerformed(ActionEvent evt) {
				byte temp = 1;
				communicator.writeData(temp);
			}
		});


		//bodyforward button
		btnBodyForward.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent evt) {
				btnBodyForwardActionPerformed(evt);
			}

			private void btnBodyForwardActionPerformed(ActionEvent evt) {
				byte temp = 2;
				communicator.writeData(temp);

			}
		});
		
		//bodybackward button
		btnBodyBackward.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent evt) {
				btnBodyBackwardActionPerformed(evt);
			}

			private void btnBodyBackwardActionPerformed(ActionEvent evt) {
				// TODO Auto-generated method stub
				
			}
		});
		
		//btnJoint1LB.addActionListener(arg0);

	}

	//slider function
	@Override
	public void stateChanged(ChangeEvent e) {
		JSlider source = (JSlider)e.getSource();
		if (!source.getValueIsAdjusting()) {
			int speed = (int)source.getValue();
			System.out.println(speed);
		}
	}
	
	@Override
	public void actionPerformed(ActionEvent arg0) {
		// TODO Auto-generated method stub
		
	}


	public static void main(String[] args) {
		java.awt.EventQueue.invokeLater(new Runnable() {
			public void run() {
				new GUI2().setVisible(true);
			}
		});

	}


	

}
