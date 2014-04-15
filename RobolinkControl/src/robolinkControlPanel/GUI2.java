package robolinkControlPanel;

import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.Toolkit;
import java.awt.Window;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextArea;

public class GUI2 extends javax.swing.JFrame {
	
	
	private static final long serialVersionUID = 1L;
	
	//Communicator object
	Communicator2 communicator = null;
	//KeybindingController object
	KeybindingController2 keybindingController = null;

	//all the variables (buttons, labels etc.)
	public JButton btnConnect;
	public JButton btnDisconnect;
	public JButton btnBodyStop;
	public JButton btnBodyForward;
	public JButton btnBodyBackward;
	public JButton btnBodyLeft;
	public JButton btnBodyRight;
	public JButton btnCalibration;
	public JButton btnArmUp;
	public JButton btnArmDown;
	public JButton btnArmForward;
	public JButton btnArmBackward;
	public JButton btnArmRight;
	public JButton btnArmLeft;
	public JComboBox cboxPorts;
	public JTextArea txtLog;
	public JTextArea txtTel;
	public JLabel lblDrive;
	public JLabel lblCalibration;
	public JLabel lblArm;
	
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
		btnBodyBackward = new JButton();
		btnBodyForward = new JButton();
		btnBodyLeft = new JButton();
		btnBodyRight = new JButton();
		btnBodyStop = new JButton();
		btnCalibration = new JButton();
		btnArmBackward = new JButton();
		btnArmDown = new JButton();
		btnArmForward = new JButton();
		btnArmLeft = new JButton();
		btnArmRight = new JButton();
		btnArmUp = new JButton();
		
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
		
		//all the button texts
		lblArm.setText("ARM");
		lblCalibration.setText("CALIBRATION");
		lblDrive.setText("DRIVE");
		btnCalibration.setText("CAL");
		btnBodyStop.setText("STOP");
		btnBodyForward.setText("FW");
		btnBodyBackward.setText("BW");
		btnBodyLeft.setText("LEFT");
		btnBodyRight.setText("RIGHT");
		btnArmBackward.setText("BW");
		btnArmDown.setText("DOWN");
		btnArmForward.setText("FW");
		btnArmLeft.setText("LEFT");
		btnArmRight.setText("RIGHT");
		btnArmUp.setText("UP");
		
		//calibration button and label
		navPanel.add(lblCalibration);
		navPanel.add(btnCalibration);
		
		//drive buttons and label
		navPanel.add(lblDrive);
		navPanel.add(btnBodyStop);
		navPanel.add(btnBodyForward);
		navPanel.add(btnBodyBackward);
		navPanel.add(btnBodyLeft);
		navPanel.add(btnBodyRight);
		
		//arm buttons and label
		navPanel.add(lblArm);
		navPanel.add(btnArmUp);
		navPanel.add(btnArmForward);
		navPanel.add(btnArmDown);
		navPanel.add(btnArmLeft);
		navPanel.add(btnArmBackward);
		navPanel.add(btnArmRight);
		
		
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
		
		btnConnect.addActionListener(new java.awt.event.ActionListener() {
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

		
		btnDisconnect.addActionListener(new java.awt.event.ActionListener() {
			public void actionPerformed(ActionEvent evt) {
				btnDisconnectActionPerformed(evt);

			}

			private void btnDisconnectActionPerformed(ActionEvent evt) {
				communicator.disconnect();
			}
		});
		
		btnCalibration.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent evt) {
				btnCalibrationActionPerformed(evt);
				
			}

			private void btnCalibrationActionPerformed(ActionEvent evt) {
				byte temp = 1;
				communicator.writeData(temp);
				
			}
		});
		
		btnBodyForward.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent evt) {
				btnBodyForwardActionPerformed(evt);
				
			}

			private void btnBodyForwardActionPerformed(ActionEvent evt) {
				byte temp = 2;
				communicator.writeData(temp);
				
			}
		});
		


	}

	public static void main(String[] args) {
		java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                new GUI2().setVisible(true);
            }
        });

	}

}
