#include <iostream>
#include <cmath>
//#include <unordered_map>
#include <map>
#include <fstream>
#include "gamestate.h"

using namespace std;

//unordered_map<long long, int> storedScores;
map<long long, int> storedScore;

void printBoard(int board[7][7]){
    for(int i = 1; i < 7; i++)
    {
       for(int j = 0; j < 7; j++)
       {
           cout << board[i][j] << " ";
       }
       cout << endl;
    }
}

void placePiece(int & currentCol, int board[7][7], int col, GameState & currentGame)
{
    bool placed = false;
    for(int i = 6; i > 0 && !placed; i--) {
        if(board[i][col] == 0) {
            board[i][col] = currentCol;
			currentCol = (currentCol % 2) + 1;
			currentGame = currentGame.move(col);
			cout << currentGame.getCurBM() << endl;
			cout << currentGame.getOppBM() << endl;
			cout << currentGame.getMaskBM() << endl;
            placed = true;
        }
    }
}

int negaMax(GameState & game, int depth, int alpha, int beta)
{
    //cout << depth;
    depth--;
    if(depth == 0) 
        return game.score();
    if(game.getTotalMoves() == 42)
        return game.score();
    for(int col = 0; col < 7; col++)
    {
        if(game.canMove(col) && game.move(col).loser())
            return 1000000;
    }
    int max = 1010000;
    
    
    if(storedScore.find(game.key()) != storedScore.end())
    {
        int ret = storedScore[game.key()];
        return ret;
    }
    
    if(beta > max)
	{
		beta = max;
		if(alpha >= beta) 
            return beta;
	}
	
	for(int i = 0; i < 7; i++) {
		int col = 7/2 + (1-2*(i%2))*(i+1)/2;
		if(game.canMove(col)) {
			GameState nextMove = game.move(col);
			int score = -negaMax(nextMove, depth, -beta, -alpha);
			if(score >= beta) 
                return score;
			if(score > alpha) 
                alpha = score;    			
		}
	}
	storedScore[game.key()] = alpha;
    return alpha;
    
}

int chooseCol(GameState & currentGame, int & lvl)
{
    
    int minVal = -10000000;
    int scores[7] = {minVal, minVal, minVal, minVal, minVal, minVal, minVal}; 
    int bestScore = minVal;
    int bestColumn = -1;
    bool foundWin = false;
    
    for(int i = 0; i < 7 && !foundWin; i++)
    {
        int col = 7/2 + (1-2*(i%2))*(i+1)/2;
        if(currentGame.canMove(col))
        {
            GameState nextState = currentGame.move(col);
            if(nextState.loser())
            {
                cout << "Column " << col << " wins." << endl;
                scores[col] = 100000;
                bestColumn = col;
                foundWin = true;
            }
            if(!foundWin)
            {
                scores[col] = -negaMax(nextState, lvl, INT_MIN, INT_MAX);
                cout << scores[col] << endl;
                if(bestScore < scores[col])
                {
                    bestScore = scores[col];
                    bestColumn = col;
                }
            }
        }
    }
    return bestColumn;
}

int main()
{
    const int WIDTH = 7;
    const int HEIGHT = 7;
    int board[HEIGHT][WIDTH] = {0};
    int first = 0;
    int lvl = 0;
    int turn = 0;
    
    ifstream in_indic("indicator.txt");
    ifstream in_instr("instruction.txt");
    ofstream ou_indic("indicating.txt");
    ofstream ou_instr("instructor.txt");
    
    int totalPlayed = 0;
    
    do {
		cout << "Enter level between 1 and 15: ";
		cin >> lvl;
	}while(lvl <= 0 || lvl > 15); //10 works well
	do {
		cout << "First player: \n1) User \n2) Computer" << endl;
		cin >> first;
	}while(first <= 0 || first > 2);
	
	turn = first;
    
    GameState currentGame = GameState();
    
    while(currentGame.getTotalMoves() <= 42)
    {
        printBoard(board);
        if(turn == 1)
        {
            int column = 0;
            /*
            //manual input start
            do {
        		cout << "Enter column between 0 and 6: ";
        		cin >> column;
        	}while(column < 0 || column > 6 || !currentGame.canMove(column));
            placePiece(turn, board, column, currentGame); 
            //manual input end
            */
            
            //file input start
			int info = -1;
			while(info != totalPlayed)
			{
				ifstream in_indic("indicator.txt");
				in_indic >> info;
				//cout << info << endl;
				in_indic.close();
			}
            
    		ifstream in_instr("instruction.txt");
            int playerMove = -1;
            in_instr >> playerMove; //robot should change this file before changing the waitfile            
            placePiece(turn, board, playerMove, currentGame);
            //file input end
            
            totalPlayed++;
            in_indic.close();
            in_instr.close();
            remove("indicator.txt");
        }
        else{
            cout << "AI turn" << endl;
        	int column = chooseCol(currentGame, lvl);
        	
        	placePiece(turn, board, column, currentGame);
        	
        	ou_indic << totalPlayed;
        	ou_instr << column;
        	
        	totalPlayed++;
        }
        cout << totalPlayed << endl;
    }
    return 0;
    
}
