package robolinkControlPanel;

import java.awt.event.ActionEvent;
import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.JButton;
import javax.swing.KeyStroke;

public class KeybindingController2 {
	GUI2 window = null;


	//    private static char stop = 'q';
	//    private static char rotateLeft = 'a';
	//    private static char rotateRight = 'd';
	   // private static char forward = 'w';
	//    private static char backward = 's';

	public KeybindingController2(GUI2 window)
	{
		this.window = window;
	}

	    public void bindKeys()
	    {
	//        //set input maps so that the program can read key bindings
	//        //putting something in the input map means to assign a key to an action name
	//        //action name is associated with a method in the action map
//		    	window.btnBodyForward.getInputMap(JButton.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke(forward), "Forward");
//		       window.btnBodyForward.getInputMap(JButton.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke(Character.toUpperCase(forward)), "Forward");
//	        window.btnBodyForward.getActionMap().put("forward", forward);
	//
	//        window.btnLeftDecel.getInputMap(JButton.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke(leftDecel), "decelerateLeft");
	//        window.btnLeftDecel.getInputMap(JButton.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke(Character.toUpperCase(leftDecel)), "decelerateLeft");
	//        window.btnLeftDecel.getActionMap().put("decelerateLeft", decelerateLeft);
	//
	//        window.btnRightAccel.getInputMap(JButton.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke(rightAccel), "accelerateRight");
	//        window.btnRightAccel.getInputMap(JButton.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke(Character.toUpperCase(rightAccel)), "accelerateRight");
	//        window.btnRightAccel.getActionMap().put("accelerateRight", accelerateRight);
	//
	//        window.btnRightDecel.getInputMap(JButton.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke(rightDecel), "decelerateRight");
	//        window.btnRightDecel.getInputMap(JButton.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke(Character.toUpperCase(rightDecel)), "decelerateRight");
	//        window.btnRightDecel.getActionMap().put("decelerateRight", decelerateRight);
	//
	//        //just attach to the left button, since theres no button for both
	//        window.btnLeftAccel.getInputMap(JButton.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke(bothAccel), "accelerateBoth");
	//        window.btnLeftAccel.getInputMap(JButton.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke(Character.toUpperCase(bothAccel)), "accelerateBoth");
	//        window.btnLeftAccel.getActionMap().put("accelerateBoth", accelerateBoth);
	//
	//        //just attach to the left button, since theres no button for both
	//        window.btnLeftDecel.getInputMap(JButton.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke(bothDecel), "decelerateBoth");
	//        window.btnLeftDecel.getInputMap(JButton.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke(Character.toUpperCase(bothDecel)), "decelerateBoth");
	//        window.btnLeftDecel.getActionMap().put("decelerateBoth", decelerateBoth);
	    }

	public void toggleControls() {
		if (window.communicator.getConnected() == true) {
			window.btnJoint1LB.setEnabled(true);
			window.btnJoint1LS.setEnabled(true);
			window.btnJoint1RB.setEnabled(true);
			window.btnJoint1RS.setEnabled(true);
			window.btnJoint2LB.setEnabled(true);
			window.btnJoint2LS.setEnabled(true);
			window.btnJoint2RB.setEnabled(true);
			window.btnJoint2RS.setEnabled(true);
			window.btnJoint3LB.setEnabled(true);
			window.btnJoint3LS.setEnabled(true);
			window.btnJoint3RB.setEnabled(true);
			window.btnJoint3RS.setEnabled(true);
			window.btnJoint4LB.setEnabled(true);
			window.btnJoint4LS.setEnabled(true);
			window.btnJoint4RB.setEnabled(true);
			window.btnJoint4RS.setEnabled(true);
			window.btnJoint5LB.setEnabled(true);
			window.btnJoint5LS.setEnabled(true);
			window.btnJoint5RB.setEnabled(true);
			window.btnJoint5RS.setEnabled(true);
			window.btnJoint6LS.setEnabled(true);
			window.btnJoint6RS.setEnabled(true);
			window.btnDPP.setEnabled(true);
			window.btnBodyBackward.setEnabled(true);
			window.btnBodyForward.setEnabled(true);
			window.btnBodyForwardLeft.setEnabled(true);
			window.btnBodyForwardRight.setEnabled(true);
			window.btnBodyStop.setEnabled(true);
			window.btnDisconnect.setEnabled(true);
			window.btnCalibration.setEnabled(true);
			window.btnConnect.setEnabled(false);
			window.btnStartPickup.setEnabled(true);
			window.btnEndPickup.setEnabled(true);
			window.btnAbortPickup.setEnabled(true);
			window.btnBodyRotateLeft.setEnabled(true);
			window.btnBodyRotateRight.setEnabled(true);
			window.btnAutomaticMode.setEnabled(true);
			window.btnDTP.setEnabled(true);
			window.btnJoint7LS.setEnabled(true);
			window.btnJoint7RS.setEnabled(true);
			window.txtKp.setEnabled(true);
			window.txtKd.setEnabled(true);
			window.cboxPorts.setEnabled(false);
		}
		else {
			window.btnJoint1LB.setEnabled(false);
			window.btnJoint1LS.setEnabled(false);
			window.btnJoint1RB.setEnabled(false);
			window.btnJoint1RS.setEnabled(false);
			window.btnJoint2LB.setEnabled(false);
			window.btnJoint2LS.setEnabled(false);
			window.btnJoint2RB.setEnabled(false);
			window.btnJoint2RS.setEnabled(false);
			window.btnJoint3LB.setEnabled(false);
			window.btnJoint3LS.setEnabled(false);
			window.btnJoint3RB.setEnabled(false);
			window.btnJoint3RS.setEnabled(false);
			window.btnJoint4LB.setEnabled(false);
			window.btnJoint4LS.setEnabled(false);
			window.btnJoint4RB.setEnabled(false);
			window.btnJoint4RS.setEnabled(false);
			window.btnJoint5LB.setEnabled(false);
			window.btnJoint5LS.setEnabled(false);
			window.btnJoint5RB.setEnabled(false);
			window.btnJoint5RS.setEnabled(false);
			window.btnJoint6LS.setEnabled(false);
			window.btnJoint6RS.setEnabled(false);
			window.btnDPP.setEnabled(false);
			window.btnBodyBackward.setEnabled(false);
			window.btnBodyForward.setEnabled(false);
			window.btnBodyForwardLeft.setEnabled(false);
			window.btnBodyForwardRight.setEnabled(false);
			window.btnBodyStop.setEnabled(false);
			window.btnDisconnect.setEnabled(false);
			window.btnCalibration.setEnabled(false);
			window.btnConnect.setEnabled(true);
			window.btnStartPickup.setEnabled(false);
			window.btnEndPickup.setEnabled(false);
			window.btnAbortPickup.setEnabled(false);
			window.btnBodyRotateLeft.setEnabled(false);
			window.btnBodyRotateRight.setEnabled(false);
			window.btnAutomaticMode.setEnabled(false);
			window.btnManualMode.setEnabled(false);
			window.btnDTP.setEnabled(false);
			window.btnJoint7LS.setEnabled(false);
			window.btnJoint7RS.setEnabled(false);
			window.txtKp.setEnabled(false);
			window.txtKd.setEnabled(false);
			window.cboxPorts.setEnabled(true);
		}
	}

}
