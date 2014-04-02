package robolinkControlPanel;

import java.awt.event.ActionEvent;
import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.JButton;
import javax.swing.KeyStroke;

public class KeybindingController2 {
	GUI2 window = null;


	//    private static char leftAccel = 'q';
	//    private static char leftDecel = 'a';
	//    private static char rightAccel = 'e';
	//    private static char rightDecel = 'd';
	//    private static char bothAccel = 'w';
	//    private static char bothDecel = 's';

	public KeybindingController2(GUI2 window)
	{
		this.window = window;
	}

	//    public void bindKeys()
	//    {
	//        //set input maps so that the program can read key bindings
	//        //putting something in the input map means to assign a key to an action name
	//        //action name is associated with a method in the action map
	//        window.btnLeftAccel.getInputMap(JButton.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke(leftAccel), "accelerateLeft");
	//        window.btnLeftAccel.getInputMap(JButton.WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke(Character.toUpperCase(leftAccel)), "accelerateLeft");
	//        window.btnLeftAccel.getActionMap().put("accelerateLeft", accelerateLeft);
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
	//    }

	public void toggleControls() {
		if (window.communicator.getConnected() == true) {
			window.btnArmBackward.setEnabled(true);
			window.btnArmDown.setEnabled(true);
			window.btnArmForward.setEnabled(true);
			window.btnArmLeft.setEnabled(true);
			window.btnArmRight.setEnabled(true);
			window.btnArmUp.setEnabled(true);
			window.btnBodyBackward.setEnabled(true);
			window.btnBodyForward.setEnabled(true);
			window.btnBodyLeft.setEnabled(true);
			window.btnBodyRight.setEnabled(true);
			window.btnBodyStop.setEnabled(true);
			window.btnDisconnect.setEnabled(true);
			window.btnCalibration.setEnabled(true);
			window.btnConnect.setEnabled(false);
			window.cboxPorts.setEnabled(false);
		}
		else {
			window.btnArmBackward.setEnabled(false);
			window.btnArmDown.setEnabled(false);
			window.btnArmForward.setEnabled(false);
			window.btnArmLeft.setEnabled(false);
			window.btnArmRight.setEnabled(false);
			window.btnArmUp.setEnabled(false);
			window.btnBodyBackward.setEnabled(false);
			window.btnBodyForward.setEnabled(false);
			window.btnBodyLeft.setEnabled(false);
			window.btnBodyRight.setEnabled(false);
			window.btnBodyStop.setEnabled(false);
			window.btnDisconnect.setEnabled(false);
			window.btnCalibration.setEnabled(false);
			window.btnConnect.setEnabled(true);
			window.cboxPorts.setEnabled(true);
		}
	}

}