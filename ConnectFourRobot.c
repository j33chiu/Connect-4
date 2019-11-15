void resetGame(); //just makes sure all parts are back to the starting state (generally only functional if player wants to play again)
void gameSetup(int & diff, int & first); //ui to determine difficulty and who plays first

void playerPosition(); //sends board to position where the user can properly see and play
int userTurn(); 
int computerTurn(int moves); 

int scan(int (&check)[]); //returns number of pieces played 
void moveSensor(int row, int col);
int getSensorColour ();

void dispense(int column);

bool playAgain();

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
}


task main()
{
	nMotorEncoder[motorA] = 0;
	nMotorEncoder[motorB] = 0;
	bool playGame = true;
	while(playGame)
	{
		resetGame();
		int difficulty = -1;
		int userFirst = true; //0 for user, 1 for ai
		
		gameSetup(difficulty, userFirst);
		
		if(userFirst){
			int winner = userTurn();
			if(winner == 1) //ai win
			else if(winner == -1) //user win
			else //draw
		}
			
		else{
			int winner = computerTurn();
			if(winner == -1) //ai win
			else if(winner == 1) //user win
			else //draw
		}
		playGame = playAgain();
	}
	//ending message
	//wait 3 seconds
}

void resetGame()
{
}

void gameSetup(int & diff, bool & playerFirst)
{
}

void playerPosition()
{
	//move motorB to ~1700 encoder
}

int userTurn()
{
	time1[T1] = 0;
	bool hasPressed = false;
	while(time1[T1] < 20000 && !hasPressed)
	{
		//if touch sensor pressed (value = 1) hasPressed = true
	}
	if(!hasPressed) // user failed to go in the 20s time frame
	{
		//file io: tell C++ to give 2 moves
		return -computerTurn(2);
	}
	int userPlaced = scan(); //TODO: implement array to check
	if(userPlaced == 1) 
		return -computerTurn(1);
	else if(userPlaced == 0) 
		return -computerTurn(2);
	else 
		return 100; //cheating
}

int computerTurn(int moves)
{
	while() //wait for C++ program to respond with move
	{
	}
	int data = 0; //read from file
	if() //ai has won
	{
		return -1;
	}
	else if() //user has won
	{
		return 1;
	}
	else if() //draw
	{
		return 0
	}
	
	if(moves == 1)
	{
		dispense(column);
		playerPosition();
		return -userTurn();
	}
	else
	{
		dispense(column1);
		dispense(column2);
		playerPosition();
		return -userTurn();
	}
}


int scan(int (&check)[]) //also indicates to C++ via file io
{
	int totalFound = 0;
	bool cheating = false;
	int columnFound = -1;
	for(int col = 0; col < 7 && !cheating; col++)
	{
		moveSensor(check[col], col);
		int colour = getSensorColour();
		if(colour != white/yellow){
			totalFound++;
			columnFound = col;
		}
		if(check[col] < 5)
		{
			moveSensor(check[col], col);
			colour = getSensorColour();
			if(colour != white/yellow)
			{
				totalFound++;
				columnFound = col;
			}	
		}
		if(totalFound > 1)
			cheating = true;
	}
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

void moveSensor(int row, int col)
{
	int horENC[7] = {0, 360, 720, 1080, 1440, 1800, 2160};
	int verENC[6] = {0, -26, -39, -51, -65, -84};

	float rowDiff = nMotorEncoder[motorA] - verENC[row];
	float colDiff = nMotorEncoder[motorB] - horENC[col];

	bool horMove = true;
	bool verMove = true;
	
	if(rowDiff > 0 && colDiff < 0)
	{
		motor[motorA] = -7;
		motor[motorB] = 25;
		while(horMove || verMove)
		{
			if(	nMotorEncoder[motorB] >= horENC[col])
			{	
				motor[motorB] = 0;
				horMove = false;
			}
			
			if(	nMotorEncoder[motorA] <= verENC[row])
			{	
				motor[motorA] = 0;
				verMove = false;
			}
		}
	}
	else if(rowDiff > 0 && colDiff > 0)
	{
		motor[motorA] = -7;
		motor[motorB] = -25;
		while(horMove || verMove)
		{
			if(	nMotorEncoder[motorB] <= horENC[col])
			{	
				motor[motorB] = 0;
				horMove = false;
			}
			if(	nMotorEncoder[motorA] <= verENC[row])
			{	
				motor[motorA] = 0;
				verMove = false;
			}
		}
	}
	else if(rowDiff < 0 && colDiff < 0)
	{
		motor[motorA] = 7;
		motor[motorB] = 25;
		while(horMove || verMove)
		{
			if(	nMotorEncoder[motorB] >= horENC[col])
			{	
				motor[motorB] = 0;
				horMove = false;
			}
			if(	nMotorEncoder[motorA] >= verENC[row])
			{	
				motor[motorA] = 0;
				verMove = false;
			}
		}
	}
	else if(rowDiff < 0 && colDiff > 0)
	{
		motor[motorA] = 7;
		motor[motorB] = -25;
		while(horMove || verMove)
		{
			if(	nMotorEncoder[motorB] <= horENC[col])
			{	
				motor[motorB] = 0;
				horMove = false;
			}
			if(	nMotorEncoder[motorA] >= verENC[row])
			{	
				motor[motorA] = 0;
				verMove = false;
			}
		}
	}
	wait1Msec(500);
}

int getSensorColour ()
{
}

void dispense(int column)
{
	moveSensor(0, column); //TODO check if mechanical system works for column. otherwise another function might be needed
	//move motor driving dispensing system
}

bool playAgain()
{
}

