//package perfect_connect_four;

import java.util.Scanner;
import java.awt.event.*;

public class MainConnectFour {
	public static void main(String[] args) {
		int level = 0;
		int first = 0;
		Scanner in = new Scanner(System.in);
		do {
			System.out.println("Enter level between 1 and 10: ");
			level = in.nextInt();
		}while(level <= 0 || level > 10);
		do {
			System.out.println("First player: \n1) User \n2) Computer");
			first = in.nextInt();
		}while(first <= 0 || first > 2);
		System.out.println("Starting State? (-1 for normal)");
		String startState = in.next();
		in.close();
		System.out.println(0^1);
		ConnectFourWindow gameWindow = new ConnectFourWindow(level, first, startState);
		gameWindow.addWindowListener(new WindowAdapter() {
			public void windowClosing(WindowEvent e) {
				System.exit(0);
			}
		});
	}
}