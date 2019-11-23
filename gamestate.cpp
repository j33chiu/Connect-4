#include <bitset>
#include "gamestate.h"

GameState::GameState() 
{
    curBitMap = 0;
    oppBitMap = 0;
    maskBitMap = 0;
    totalMoves = 0;   
}
GameState::GameState(long long curBitMap, long long oppBitMap, long long mask)
{
    (*this).curBitMap = curBitMap;
    (*this).oppBitMap = oppBitMap;
    (*this).maskBitMap = mask;
    totalMoves = 0;
}

long long GameState::sentinelRow()
{
    long long topRow = 283691315109952LL; //bitmap of entire sentinel row of 1s
    return topRow;
}

long long GameState::outsideBoard()
{
    long long outside = 9222809086901354496LL;
    return outside;
}

bool GameState::canMove(int col)
{
    long long place = (1LL << (HEIGHT - 1)) << col*(HEIGHT + 1);
    long long m = maskBitMap & place;
    if(m == 0) return true;
    else return false;
}
long long GameState::key()
{
    return curBitMap + maskBitMap;
}

GameState GameState::move(int col)
{
    long long nextMask = maskBitMap | (maskBitMap + (1LL << (long long)(col*7)));
    long long nextOpp = oppBitMap ^ nextMask;
    long long nextCur = nextOpp ^ nextMask;
    GameState newState(nextCur, nextOpp, nextMask);
    totalMoves++;
    return newState;
}

bool GameState::winner()
{
    long long map = 0;
    map = curBitMap & (curBitMap >> 7);
    if((map & (map >> 14)) > 0)
        return true;
    //diagonal \ check
    map = curBitMap & (curBitMap >> 6);
    if((map & (map >> 12)) > 0)
        return true;
    //diagonal / check
    map = curBitMap & (curBitMap >> 8);
    if((map & (map >> 16)) > 0)
        return true;
    //vertical check
    map = curBitMap & (curBitMap >> 1);
    if((map & (map >> 2)) > 0)
        return true;
    //no win pattern found
    return false;
}

bool GameState::loser()
{
    long long map = 0;
    map = oppBitMap & (oppBitMap >> 7);
    if((map & (map >> 14)) > 0)
        return true;
    //diagonal \ check
    map = oppBitMap & (oppBitMap >> 6);
    if((map & (map >> 12)) > 0)
        return true;
    //diagonal / check
    map = oppBitMap & (oppBitMap >> 8);
    if((map & (map >> 16)) > 0)
        return true;
    //vertical check
    map = oppBitMap & (oppBitMap >> 1);
    if((map & (map >> 2)) > 0)
        return true;
    //no win pattern found
    return false;
}

int GameState::bitCount(long long map)
{
    bitset<64> m(map);
    return m.count();
}

int GameState::centerPieces(long long map)
{
    long long centerColMask = ((1LL << HEIGHT) - 1LL) << (HEIGHT + 1)*3; //mask of the entire center column
    long long toCount = centerColMask & map;
    return bitCount(toCount);
}

int GameState::checkThrees(long long map)
{
    long long avail = ~maskBitMap;
    avail = avail - sentinelRow();
    int count = 0;
    long long m1 = 0;
    long long m2 = 0;
    //horizontal
    //shift tokens right
    m1 = map & (map << 7);
    m1 = ((m1 & (m1 << 7)) << 7) & avail;
    count += bitCount(m1);
    //shift tokens left
    m2 = map & (map >> 7);
    m2 = ((m2 & (m2 >> 7)) >> 7) & avail;
    count += bitCount(m2);

    //vertical
    //shift up
    m1 = map & (map << 1);
    m1 = ((m1 & (m1 << 1)) << 1) & avail;
    count += bitCount(m1);
    //shift down
    m2 = map & (map >> 1);
    m2 = ((m2 & (m2 >> 1)) >> 1) & avail;
    count += bitCount(m2);
    
    //diagonal \
    //shift tokens to the upper right
    m1 = map & (map << 6);
    m1 = ((m1 & (m1 << 6)) << 6) & avail;
    count += bitCount(m1);
    //shift tokens to the lower left
    m2 = map & (map >> 6);
    m2 = ((m2 & (m2 >> 6)) >> 6) & avail;
    count += bitCount(m2);
    
    //diagonal /
    //shift tokens to the lower right
    m1 = map & (map << 8);
    m1 = ((m1 & (m1 << 8)) << 8) & avail;
    count += bitCount(m1);
    //shift tokens to the upper left
    m2 = map & (map >> 8);
    m2 = ((m2 & (m2 >> 8)) >> 8) & avail;
    count += bitCount(m2);
    
    return count;
}

int GameState::checkTwos(long long map)
{
    int count = 0;
	long long avail = ~maskBitMap;
    avail = avail - sentinelRow() - outsideBoard();
	long long m1 = 0;
	long long m2 = 0;
	
	//horizontal
    //shift tokens right
    m1 = map & (map << 7);
    m1 = (((m1 << 7) & avail) << 7) & avail;
    count += bitCount(m1);
    //shift tokens left
    m2 = map & (map >> 7);
    m2 = (((m2 >> 7) & avail) >> 7) & avail;
    count += bitCount(m2);

    //vertical
    //shift up
    m1 = map & (map << 1);
    m1 = (((m1 << 1) & avail) << 1) & avail;
    count += bitCount(m1);
    //shift down
    m2 = map & (map >> 1);
    m2 = (((m2 >> 1) & avail) >> 1) & avail;
    count += bitCount(m2);
    
    //diagonal \
    //shift tokens to the upper right
    m1 = map & (map << 6);
    m1 = (((m1 << 6) & avail) << 6) & avail;
    count += bitCount(m1);
    //shift tokens to the lower left
    m2 = map & (map >> 6);
    m2 = (((m2 >> 6) & avail) >> 6) & avail;
    count += bitCount(m2);
    
    //diagonal /
    //shift tokens to the lower right
    m1 = map & (map << 8);
    m1 = (((m1 << 8) & avail) << 8) & avail;
    count += bitCount(m1);
    //shift tokens to the upper left
    m2 = map & (map >> 8);
    m2 = (((m2 >> 8) & avail) >> 8) & avail;
    count += bitCount(m2);
    
	return count;
}

int GameState::score()
{
    int s = 0;
	s += (centerPieces(curBitMap) * 30);
	s += (checkTwos(curBitMap) * 10);
	s += (checkThrees(curBitMap) * 80);
	if(winner()) s += 100000;
	
	//System.out.println("center: " + centerPieces());
	//System.out.println("2s: " + checkTwos());
	//System.out.println("3s: " + checkThrees());
	
	s -= (centerPieces(oppBitMap) * 30);
	s -= (checkTwos(oppBitMap) * 10);
	s -= (checkThrees(oppBitMap) * 80);
	if(loser()) s -= 100000;
	
	return s;
}

int GameState::getTotalMoves()
{
    return totalMoves;
}

long long GameState::getCurBM()
{
    return curBitMap;
} 

long long GameState::getOppBM()
{
    return oppBitMap;
}

long long GameState::getMaskBM()
{
    return maskBitMap;
}
