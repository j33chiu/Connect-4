#include <cmath>
#include <cstdlib>

using namespace std;

#ifndef GAMESTATE_H
#define GAMESTATE_H

class GameState
{
    private:
        long long curBitMap;
        long long oppBitMap;
        long long maskBitMap;
        static const int WIDTH = 7;
        static const int HEIGHT = 6;
        int totalMoves;
    
    public:        
        GameState();
        GameState(long long curBitMap, long long oppBitMap, long long mask);
        bool canMove(int column);
        long long key();
        GameState move(int column);
        bool winner();
        bool loser();
        int centerPieces(long long map);
        int checkThrees(long long map);
        int checkTwos(long long map);
        int score();
        long long sentinelRow();
        long long outsideBoard();
        int bitCount(long long map);  
        int getTotalMoves();
        long long getCurBM();
        long long getOppBM();
        long long getMaskBM();
};
#endif

