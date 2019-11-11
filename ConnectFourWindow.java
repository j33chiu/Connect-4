//package perfect_connect_four;

import java.awt.Frame;

import javax.swing.JFrame;

public class ConnectFourWindow extends Frame{

	private static final long serialVersionUID = 1L;

	public ConnectFourWindow(int lvl, int first, String startState) {
		String ss = "";
		if(!startState.equals("-1")){
			ss = startState;
		}
		ConnectFourPanel gamePanel = new ConnectFourPanel(lvl, first, ss);
		setTitle("Connect 4");
		setSize(827, 771);
		//setExtendedState(JFrame.MAXIMIZED_BOTH);
		setVisible(true);
		setAlwaysOnTop(true);
		add(gamePanel);
	}
}
