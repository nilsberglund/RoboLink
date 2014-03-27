package robolinkControlPanel;

import java.awt.event.ActionEvent;

import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JTextArea;

public class GUI2 extends javax.swing.JFrame {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	//Communicator object
	Communicator2 communicator = null;
	//KeybindingController object
	KeybindingController2 keybindingController = null;

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
	public JLabel lblDrive;
	public JLabel lblCalibration;
	public JLabel lblArm;





	/** Creates new form GUI */
	public GUI2() {
		initComponents();
		createObjects();
		communicator.searchForPorts();
		keybindingController.toggleControls();
		//keybindingController.bindKeys();


	}

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
		lblDrive = new JLabel();
		lblArm = new JLabel();
		lblCalibration = new JLabel();

		setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);

		setTitle("Robolink Master Control Panel");

		lblArm.setText("ARM");
		lblCalibration.setText("CALIBRATION");
		lblDrive.setText("DRIVE");

		btnConnect.setText("CONNECT");
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

		btnDisconnect.setText("DISCONNECT");
		btnDisconnect.addActionListener(new java.awt.event.ActionListener() {
			public void actionPerformed(ActionEvent evt) {
				btnDisconnectActionPerformed(evt);

			}

			private void btnDisconnectActionPerformed(ActionEvent evt) {
				communicator.disconnect();
			}
		});
		
		


	}

	public static void main(String[] args) {
		java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                new GUI().setVisible(true);
            }
        });

	}

}
