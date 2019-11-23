int check[] = {0, 0, 0, 0, 0, 0, 0};

void gameSetup(int & diff, int & first)//ui to determine difficulty and who plays first
{
	string text[6] =    {"  Difficulty:", "  1", "  Who Starts:", "   Robot", "|START |", "|CANCEL|"};

	int selection = 0, difficulty = 1;
	string player = "Robot";
	bool mainMenu = true;

	while(mainMenu)
	{
		// Refreshing Display
		// Difficulty
		displayBigTextLine(1, text[0]);
		// Difficulty Number
		if(selection == 0)
			displayBigTextLine(3, "    <- %d ->", difficulty);
		else
			displayBigTextLine(3, "       %d", difficulty);

		// Who Starts
		displayBigTextLine(5, text[2]);
		// Robot/Player
		if(selection == 1)
			displayBigTextLine(7, "  <- %s ->", player);
		else
			displayBigTextLine(7, "     %s", player);

		// START
		if(selection == 2)
			displayBigTextLine(11, " <- %s ->", text[4]);
		else
			displayBigTextLine(11, "    %s", text[4]);

		// CANCEL
		if(selection == 3)
			displayBigTextLine(13, " <- %s ->", text[5]);
		else
			displayBigTextLine(13, "    %s", text[5]);


		// Detecting button presses, deciding what to do
		while(!getButtonPress(buttonAny))
		{}
		// If left button is pressed
		if(getButtonPress(buttonLeft))
		{
			while(getButtonPress(buttonLeft))
			{}
			// If difficulty is selected
			if(selection == 0)
				if(difficulty > 1)
				difficulty--;
			// If starter is selected
			if(selection == 1)
			{
				if(player == "Robot")
					player = "Player";
				else
					player = "Robot";
			}
		}
		// If right button is pressed
		if(getButtonPress(buttonRight))
		{
			while(getButtonPress(buttonRight))
			{}
			// If difficulty is selected
			if(selection == 0)
				if(difficulty < 10)
				difficulty++;
			// If starter is selected
			if(selection == 1)
			{
				if(player == "Robot")
					player = "Player";
				else
					player = "Robot";
			}
		}
		// If up button is pressed
		if(getButtonPress(buttonUp))
		{
			while(getButtonPress(buttonUp))
			{}
			if(selection > 0)
				selection--;
		}

		// If down button is pressed
		if(getButtonPress(buttonDown))
		{
			while(getButtonPress(buttonDown))
			{}
			if(selection < 3)
				selection++;
		}

		// If enter button is pressed
		if(getButtonPress(buttonEnter))
		{
			while(getButtonPress(buttonEnter))
			{}
			if(selection == 2)
			{
				diff = difficulty;
				if(player == "Robot")
					first = 1;
				else
					first = 0;
				mainMenu = false;
			}
			if(selection == 3)
				mainMenu = false;
		}
	}
	eraseDisplay();
}

void moveSensor(int row, int col)
{
	int horENC[7] = {-185, -350, -515, -680, -845, -1008, -1173};
	int verENC[6] = {0, -27, -38, -54, -69, -91};

	bool moveHor = true;
	bool moveVer = true;

	int diffVer = nMotorEncoder[motorA] - verENC[row];
	int diffHor = nMotorEncoder[motorB] - horENC[col];

	if(diffVer > 0) //up
		motor[motorA] = -30;
	else if (diffVer < 0) //down
		motor[motorA] = 30;
	else
		moveVer = false;
	if(diffHor > 0)
		motor[motorB] = -40;
	else if (diffHor < 0)
		motor[motorB] = 40;
	else
		moveHor = false;

	while(moveHor || moveVer)
	{
		if(diffVer > 0 && nMotorEncoder[motorA] <= verENC[row])
		{
			motor[motorA] = 0;
			moveVer = false;
		}
		else if(diffVer < 0 && nMotorEncoder[motorA] >= verENC[row])
		{
			motor[motorA] = 0;
			moveVer = false;
		}
		if(diffHor > 0 && nMotorEncoder[motorB] <= horENC[col])
		{
			motor[motorB] = 0;
			moveHor = false;
		}
		else if(diffHor < 0 && nMotorEncoder[motorB] >= horENC[col])
		{
			motor[motorB] = 0;
			moveHor = false;
		}
	}

	wait1Msec(500); //shouldnt need this in final phase
}

int scan() // returns number of pieces played, also indicates to C++ via file io
{
	SensorType[S2] = sensorEV3_Color;
	wait1Msec(50);
	SensorMode[S2] = modeEV3Color_Color;
	int totalFound = 0;
	bool cheating = false;
	int columnFound = -1;
	for(int col = 0; col < 7 && !cheating; col++)
	{
		displayString(1, "%d", check[col]);
		displayString(5, "I found %d tokens", totalFound);
		wait1Msec(100);
		if(check[col] <= 5)
		{
			moveSensor(check[col], col);
			int colour = SensorValue[S2];
			displayString(3, "Colour Bot: %d", colour);
			if(colour == colorBlack || colour == colorRed){
				totalFound++;
				playSound(soundFastUpwardTones);
				columnFound = col;
			}
			if(check[col] < 5 && totalFound == 1)
			{
				moveSensor(check[col] + 1, col);
				colour = SensorValue[S2];
				displayString(4, "Colour: %d", colour);
				displayString(5, "I found %d tokens", totalFound);
				if(colour == colorBlack || colour == colorRed)
				{
					totalFound++;
					playSound(soundDownwardTones);
					columnFound = col;
				}
			}
			if(totalFound > 1)
				cheating = true;
		}
	}
	displayString(5, "I found %d tokens", totalFound);
	if(cheating)
	{
		displayString(1, "Why must you cheat? :(");
		//TODO: find way to end program here
		return totalFound;
	}
	else if(columnFound == -1)
		return 0;
	else
	{
		check[columnFound] ++;
		//write columnFound to file;

		return 1;
	}
}

int inputCol() //test this
{
	string text[2] =    {"  Column:", "  1", };

	int col = 1;
	bool selecting = true;

	while(selecting)
	{

		// Refreshing Display

		// column
		displayBigTextLine(1, text[0]);
		// column Number
		displayBigTextLine(3, "    <- %d ->", col);

		// Detecting button presses, deciding what to do
		while(!getButtonPress(buttonAny))
		{}
		// If left button is pressed
		if(getButtonPress(buttonLeft))
		{
			while(getButtonPress(buttonLeft))
			{}
			if(col > -3)
				col--;
		}
		// If right button is pressed
		if(getButtonPress(buttonRight))
		{
			while(getButtonPress(buttonRight))
			{}
		if(col < 7)
				col++;
		}
		// If enter button is pressed
		if(getButtonPress(buttonEnter))
		{
			while(getButtonPress(buttonEnter))
			{}
			selecting = false;
		}
	}
	return col-1;
}

void dispense(int col, int & counter)
{
	check[col]++;
	int horENC[7] = {0, -185, -350, -515, -680, -845, -1008};
	int difference = nMotorEncoder[motorB] - horENC[col];

	if(difference > 0) //away
	{
		motor[motorB] = -25;
		while(nMotorEncoder[motorB] > horENC[col])
		{}
	}
	else if(difference < 0)//towards
	{
		motor[motorB] = 25;
		while(nMotorEncoder[motorB] < horENC[col])
		{}
	}
	motor[motorB] = 0;
	motor[motorC] = 15;
	while(nMotorEncoder[motorC] < 90 * counter)
	{}
	motor[motorC] = 0;
	wait1Msec(1000);
}
/*
void moveRow(int row)
{
	int verENC[6] = {0, -26, -39, -51, -65, -84};
	float difference = nMotorEncoder[motorA] - verENC[row];

	if(difference > 0)//up
	{
		motor[motorA] = -7;
		while(nMotorEncoder[motorA] > verENC[row])
		{}
	}
	else if(difference < 0)//down
	{
		motor[motorA] = 7;
		while(nMotorEncoder[motorA] < verENC[row])
		{}
	}
	motor[motorA] = 0;
	wait1Msec(500);
}

void moveCol(int col)
{
	int horENC[7] = {0, 167, 334, 501, 668, 835, 1000}; //1737 for position that the user plays
	float difference = nMotorEncoder[motorB] - horENC[col];

	if(difference > 0) //away
	{
		motor[motorB] = -25;
		while(nMotorEncoder[motorB] > horENC[col])
		{}
	}
	else if(difference < 0)//towards
	{
		motor[motorB] = 25;
		while(nMotorEncoder[motorB] < horENC[col])
		{}
	}
	motor[motorB] = 0;
	displayString(1, "%d", nMotorEncoder[motorB]);
	wait1Msec(500);
}*/

void resetGame() //just makes sure all parts are back to the starting state (generally only functional if player wants to play again)
{
	/*
	SensorType[S1] = sensorEV3_Touch;
	motor[motorB] = 40;
	while(SensorValue[S1] != 1)
	{}
	motor[motorB] = 0;*/

	float hdiff = nmotorEncoder[motorB];
	if(hdiff> 0){
		motor[motorB] = -40;
		while (nMotorEncoder[motorB] > 0)
		{}
	}
	else if (hdiff < 0){
		motor[motorB] = 40;
		while(nMotorEncoder[motorB] < 0)
		{}
	}
	motor[motorB] = 0;

	float difference = nMotorEncoder[motorA] - 0;

	if(difference > 0)//up
	{
		motor[motorA] = -30;
		while(nMotorEncoder[motorA] > 0)
		{}
	}
	else if(difference < 0)//down
	{
		motor[motorA] = 30;
		while(nMotorEncoder[motorA] < 0)
		{}
	}
	motor[motorA] = 0;
	wait1Msec(200);
}

void playerPosition() //sends board to position where the user can properly see and play
{
	//move motorB to ~1700 encoder
	float difference = nMotorEncoder[motorB] - (-1500);
  if(difference > 0) //away
	{
		motor[motorB] = -40;
		while(nMotorEncoder[motorB] > (-1500))
		{}
	}
	else if(difference < 0)//towards
	{
		motor[motorB] = 40;
		while(nMotorEncoder[motorB] < (-1500))
		{}
	}
	motor[motorB] = 0;
	//displayString(1, "%d", nMotorEncoder[motorB]);
	wait1Msec(500);
}

int userTurn(int & totalPlaced);
int computerTurn(int moves, int & totalPlaced);

int userTurn(int & totalPlaced)
{
	playerPosition();
	SensorType[S3] = sensorEV3_Touch;
	time1[T1] = 0;
	bool hasPressed = false;
	while(time1[T1] < 20000 && !hasPressed)
	{
		if(SensorValue[S3] == 1)
			hasPressed = true;
	}
	if(!hasPressed) // user failed to go in the 20s time frame
	{
		//file io: tell C++ to give 2 moves
		playSound(soundException);
		return -computerTurn(2, totalPlaced);
	}
	resetGame();
	int userPlaced = scan();
	eraseDisplay();
	if(userPlaced == 1)
		return -computerTurn(1, totalPlaced);
	else if(userPlaced == 0)
		return -computerTurn(2, totalPlaced);
	else
		return 100; //cheating
}

int computerTurn(int moves, int & totalPlaced)
{
	/*while() //wait for C++ program to respond with move
	{}*/
	resetGame();
	int data = inputCol(); //ai win: press 0, draw: -1, user winL -2
	//check cases:
	//data = -1; //ai has won
	//data = -2; //draw
	//data = -3; //user has won
	if(data == -1) //ai has won
	{
		return -1;
	}
	else if(data == -3) //user has won
	{
		return 1;
	}
	else if(data == -2) //draw
	{
		return 0;
	}

	if(moves == 1)
	{
		dispense(data, totalPlaced);
		totalPlaced++;
		playerPosition();
		return -userTurn(totalPlaced);
	}
	else
	{
		dispense(data, totalPlaced);
		totalPlaced++;
		data = inputCol();
		eraseDisplay();
		if(data == -1) //ai has won
		{
			return -1;
		}
		else if(data == -3) //user has won
		{
			return 1;
		}
		else if(data == -2) //draw
		{
			return 0;
		}
		dispense(data, totalPlaced);
		totalPlaced++;

		return -userTurn(totalPlaced);
	}
}

void playerPosition() //sends board to position where the user can properly see and play
{
	//move motorB to ~1700 encoder
	float difference = nMotorEncoder[motorB] - (-1500);
  if(difference > 0) //away
	{
		motor[motorB] = -40;
		while(nMotorEncoder[motorB] > (-1500))
		{}
	}
	else if(difference < 0)//towards
	{
		motor[motorB] = 40;
		while(nMotorEncoder[motorB] < (-1500))
		{}
	}
	motor[motorB] = 0;
	//displayString(1, "%d", nMotorEncoder[motorB]);
	wait1Msec(500);
}

bool playAgain()
{
	resetGame();
	string text[2] = {"|YES |", "| NO |"};
	int selection = 0
	bool endMenu = true;

	eraseDisplay();
	//displayBigTextLine(3, "%s won!", winner);
	displayBigTextLine(3, "Play again?");

	while(endMenu)
	{
		// YES
		if(selection == 0)
			displayBigTextLine(7, " <- %s ->", text[0]);
		else
			displayBigTextLine(7, "    %s", text[0]);

		// NO
		if(selection == 1)
			displayBigTextLine(9, " <- %s ->", text[1]);
		else
			displayBigTextLine(9, "    %s", text[1]);
		// Detecting button presses, deciding what to do
		while(!getButtonPress(buttonAny))
		{}

		// If up button is pressed
		if(getButtonPress(buttonUp))
		{
			while(getButtonPress(buttonUp))
			{}
			if(selection > 0)
				selection--;
		}

		// If down button is pressed
		if(getButtonPress(buttonDown))
		{
			while(getButtonPress(buttonDown))
			{}
			if(selection < 1)
				selection++;
		}
		// If enter button is pressed
		if(getButtonPress(buttonEnter))
		{
			endMenu = false;
			eraseDisplay();
			while(getButtonPress(buttonEnter))
			{}
			if(selection == 0)
				return true;
			else
				return false;
		}
	}
	return false;
}

task main()
{

	nMotorEncoder[motorA] = 0;
	nMotorEncoder[motorB] = 0;
	bool playGame = true;
	while(playGame)
	{
		resetGame();
		int difficulty = 1;
		int first = 0; //0 for user, 1 for ai

		gameSetup(difficulty, first);

		displayTextLine(1, "%d", difficulty);
		displayTextLine(2, "%d", first);

		int totalPlaced = 1;

		if(first == 0){
			int winner = userTurn(totalPlaced);
			eraseDisplay();
			if(winner == 1)
			{
				displayBigTextLine(1, "AI win");
			} //ai win
			else if(winner == -1){displayBigTextLine(1, "user win");} //user win
			else{displayBigTextLine(1, "draw");} //draw
		}
		else{
			int winner = computerTurn(1, totalPlaced);
			eraseDisplay();
			if(winner == -1){displayBigTextLine(1, "AI win");} //ai win
			else if(winner == 1){displayBigTextLine(1, "user win");} //user win
			else {displayBigTextLine(1, "draw");}//draw
		}
		wait1Msec(5000);
		playGame = playAgain();
	}
	//ending message
	//wait 3 seconds
}
