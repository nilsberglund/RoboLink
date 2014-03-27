package robolinkControlPanel;

public class GUI2 extends javax.swing.JFrame {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	//Communicator object
    Communicator2 communicator = null;
    //KeybindingController object
    KeybindingController2 keybindingController = null;

    /** Creates new form GUI */
    public GUI2() {
        initComponents();
        createObjects();
        communicator.searchForPorts();
        keybindingController.toggleControls();
        keybindingController.bindKeys();
    }

	private void createObjects() {
		communicator = new Communicator2(this);
        keybindingController = new KeybindingController2(this);
		
		
	}

	private void initComponents() {
		
		
	}

	public static void main(String[] args) {
		// TODO Auto-generated method stub

	}

}
