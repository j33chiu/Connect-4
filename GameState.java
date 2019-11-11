//package perfect_connect_four;

import java.util.Arrays;
/*
bitstring representation:
board:
(6, 13, 20, 27, 34, 41, 48) <-- sentinel row, always filled with 0s
[5, 12, 19, 26, 33, 40, 47] << all rows below this represent the actual board
[4, 11, 18, 25, 32, 39, 46] << numbers in these rows only represent the bit count
[3, 10, 17, 24, 31, 38, 45]
[2, 9, 16, 23, 30, 37, 44]
[1, 8, 15, 22, 29, 36, 43]
[0, 7, 14, 21, 28, 35, 42]

note that this class is based on information gained in
https://towardsdatascience.com/creating-the-perfect-connect-four-ai-bot-c165115557b0
*/
public class GameState {
    private int[] bitRepCur; //position of current player's pieces
    private int[] bitRepMask; //all pieces
    private int[] bitRepOpp; //opponent pieces
    private long bitstringCur = 0;
    private long bitstringMask = 0;
    private long bitstringOpp = 0;
    public int totalMoves = 0;
    public final int WIDTH = 7;
    public final int HEIGHT = 6;
    public final int MINIMUM_SCORE = -(WIDTH * HEIGHT + 1)/2 - 3;
    public final int MAXIMUM_SCORE = (WIDTH * HEIGHT + 1)/2 - 3;
    public int perspective = 0; // if 1, the current player is the user, 2 is the computer
    public String[] perspText = new String[]{"", "User", "AI"};

    /*public GameState(int[][] board) {
        int i = 0;
        int bits = (board.length) * board[0].length;
        bitRepComp = new int[bits];
        bitRepMask = new int[bits];
        bitRepPl = new int[bits];
        for(int col = board[0].length - 1; col >= 0; col--){
            for(int row = 0; row < board.length; row++) {
                if(board[row][col] == 2) bitRepComp[i] = 1;
                if(board[row][col] != 0) bitRepMask[i] = 1;
                bitRepPl[i] = bitRepComp[i]^bitRepMask[i];
                i++;
                int power = bits - i;
                bitstringComp += Math.pow(2, power) * bitRepComp[i-1];
                bitstringMask += Math.pow(2, power) * bitRepMask[i-1];
                bitstringPl += Math.pow(2, power) * bitRepPl[i-1];
            }
        }
        //System.out.println(bitstringComp + " " + bitstringMask + " " + bitstringPl);
        //System.out.println(Arrays.toString(bitRepMask));
    }*/

    public GameState(int perspective, int totalMoves) {
        this.perspective = perspective;
        this.totalMoves = totalMoves;
    }

    public GameState(long bitstringCur, long bitstringOpp, long mask, int perspective, int totalMoves) { //bitstring takes for the object being made in reference to the current player
        this.bitstringCur = bitstringCur;
        this.bitstringOpp = bitstringOpp;
        this.bitstringMask = mask;
        this.perspective = perspective;
        this.totalMoves = totalMoves;
        //System.out.println("AI: " + bitstringComp + " Player: " + bitstringPl + " Mask: " + bitstringMask);
    }

    public boolean winner() {
        long m = 0;
        //horizontal check
        m = bitstringCur & (bitstringCur >> 7);
        if((m & (m >> 14)) > 0)
            return true;
        //diagonal \ check
        m = bitstringCur & (bitstringCur >> 6);
        if((m & (m >> 12)) > 0)
            return true;
        //diagonal / check
        m = bitstringCur & (bitstringCur >> 8);
        if((m & (m >> 16)) > 0)
            return true;
        //vertical check
        m = bitstringCur & (bitstringCur >> 1);
        if((m & (m >> 2)) > 0)
            return true;
        //no win pattern found
        return false;
    }

    public boolean loser() {
        long m = 0;
        //horizontal check
        m = bitstringOpp & (bitstringOpp >> 7);
        if((m & (m >> 14)) > 0)
            return true;
        //diagonal \ check
        m = bitstringOpp & (bitstringOpp >> 6);
        if((m & (m >> 12)) > 0)
            return true;
        //diagonal / check
        m = bitstringOpp & (bitstringOpp >> 8);
        if((m & (m >> 16)) > 0)
            return true;
        //vertical check
        m = bitstringOpp & (bitstringOpp >> 1);
        if((m & (m >> 2)) > 0)
            return true;
        //no win pattern found
        return false;
    }

    public long key(){
        return bitstringCur + bitstringMask;
    }

    public GameState move(int col) { //the current player moves
        //piece = 1: player, = 2: computer
        long nextOppPos = bitstringCur ^ bitstringMask; //gives opponent position
        long nextMask = bitstringMask | (bitstringMask + (1L << (long)(col * 7))); //new mask
        long nextPos = nextOppPos ^ nextMask; //current player's position
        int newMovesNum = totalMoves + 1;
        GameState newState = new GameState(nextOppPos, nextPos, nextMask, (perspective % 2) + 1, newMovesNum); //this state is now in a different perspective (swapped to simulate turns)
        return newState;
        /*if(piece == 1) {
            long nextPosComp = bitstringPl ^ bitstringMask;
            long nextMask = bitstringMask | (bitstringMask + (1L << (long)(col * 7)));
            GameState newState = new GameState(nextPosComp, nextMask, 2);
            return newState;
        }
        else if (piece == 2) {
            long nextPosPl = bitstringComp ^ bitstringMask;
            long nextMask = bitstringMask | (bitstringMask + (1L << (long)(col * 7)));
            GameState newState = new GameState(nextPosPl, nextMask, 1);
            return newState;
        }
        return null;*/
    }

    public String getInfo()
    {
        String currentInfo = perspText[perspective] + ": " + bitstringCur + ", " + perspText[(perspective % 2) + 1] + ": " + bitstringOpp + ", Mask: " + bitstringMask;
        return currentInfo;
    }

    public long[] getBitstrings() {
        long[] out = new long[]{bitstringCur, bitstringMask, bitstringOpp};
        return out;
    }

    public boolean canMove(int col) {
        long topPieceMask = (1L << (HEIGHT - 1)) << col*(HEIGHT+1);
        return (bitstringMask & topPieceMask) == 0;
    }

    

    
}