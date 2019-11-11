//package perfect_connect_four;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Panel;
import java.awt.Point;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.image.BufferedImage;
import java.util.Random;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.Arrays;
import java.util.LinkedHashMap;
import java.util.Map;


public class ConnectFourPanel extends Panel implements MouseListener, KeyListener{

	private static final long serialVersionUID = 1L;
	
	private Dimension dim = null;
	private Dimension boardDim = new Dimension();
	private Graphics osg;
	private BufferedImage osm;
	
    private int lvl = 0;
    private int first = 0;
	private int xOffset = 0, yOffset = 0, baseUnits = 0, squareLen = 0;
	double circleRad = 0;
    
	private int[][] board = new int[7][7];
	/*private int[][] board = new int[][] {
		{0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 2, 0, 0, 0},
		{0, 0, 1, 2, 2, 0, 0},
		{1, 1, 2, 1, 1, 2, 0}
		};
    */
    private String representation = "";
    private GameState currentGame;
    private String prevStateStr = "";
    private String curStateStr = "";
    
    private Color[] pieceColours = new Color[] {Color.WHITE, Color.RED, Color.YELLOW};
    private String[] whoseTurn = new String[] {"Nobody", "User", "Computer"};
	private int currentCol = 0;
    private Random r = new Random();
    
    private LinkedHashMap<Long, Integer> storedScore = 
        new LinkedHashMap<Long, Integer>() { 
            protected boolean removeEldestEntry(Map.Entry<Long, Integer> eldest) 
            { 
                return size() > 8388608 * 2; 
            } 
        }; 

	public ConnectFourPanel(int lvl, int first, String startState) {
		addMouseListener(this);
        addKeyListener(this);
        currentGame = new GameState(first, 0);
        currentCol = first;
        this.first = first;
        this.lvl = lvl;
        for(int i = 0; i < startState.length(); i++)
            placePiece(Integer.parseInt(Character.toString(startState.charAt(i))) - 1);
	}
	
	public void boardLocation() {
		double rows = board.length - 1;
		double cols = board[0].length;
		xOffset = dim.width/8;
		yOffset = dim.height/8;
		boardDim.width = dim.width - 2 * xOffset;
		boardDim.height = dim.height - 2 * yOffset;
		double[] divs = new double[] {boardDim.width/cols, boardDim.height/rows};
		squareLen = (int)Math.min(divs[0], divs[1]);
		boardDim.width = squareLen * (int)cols;
		boardDim.height = squareLen * (int)rows;
		xOffset = (dim.width - boardDim.width)/2;
		yOffset = (dim.height - boardDim.height)/2;
	}
	
	public void paint(Graphics g) {
		if(dim == null || !dim.equals(getSize())) {
            dim = getSize();
            //System.out.println(dim.toString());
		}
		boardLocation();
		baseUnits = (dim.width - 2*xOffset)/30;
		circleRad = baseUnits * 1.5;
		
		
		osm = new BufferedImage(dim.width, dim.height, BufferedImage.TYPE_INT_RGB);
		osg = osm.getGraphics();
		update(g);
	}
	
	public void update(Graphics g) {
		
		osg.setColor(Color.WHITE);
		osg.fillRect(0, 0, dim.width, dim.height);
		
		/* for testing bounds
		osg.setColor(Color.BLACK);
		osg.drawRect(xOffset, yOffset, dim.width - 2 * xOffset, dim.height - 2 * yOffset);
		osg.setColor(Color.RED);
		osg.drawRect(xOffset, yOffset, dim.width - 2 * xOffset, dim.height - 2 * yOffset);
		*/
		
		osg.setColor(Color.BLACK);
		osg.drawRect(xOffset, yOffset, dim.width - 2 * xOffset, dim.height - 2 * yOffset);
		for(int col = 0; col < board[0].length; col++) {
			for(int row = 1; row < board.length; row++) {
				//osg.drawRect(xOffset + col * squareLen, yOffset + row * squareLen, squareLen, squareLen);
				osg.setColor(pieceColours[board[row][col]]);
				osg.fillOval(xOffset + col * squareLen, yOffset + (row - 1) * squareLen, squareLen, squareLen);
				osg.setColor(Color.BLACK);
				osg.drawOval(xOffset + col * squareLen, yOffset + (row - 1) * squareLen, squareLen, squareLen);
			}
        }
        osg.setColor(pieceColours[currentCol]);
        osg.fillOval(0, yOffset - squareLen, squareLen, squareLen);
        osg.setColor(Color.BLACK);
        osg.drawString(whoseTurn[currentCol], squareLen, squareLen/2);
        osg.drawString(prevStateStr, squareLen, squareLen);
        osg.drawString(curStateStr, squareLen, squareLen + 10);
		
		g.drawImage(osm, 0, 0, this);
    }

    public void printBoard(){
        for(int[] row : board){
            System.out.println(Arrays.toString(row));
        }
    }

    public void placePiece(int col) { //assumes the move is possible
        representation = representation + Integer.toString(col);
        for(int i = 2; i < board.length; i++) {
            if(board[i][col] != 0) {
                board[i - 1][col] = currentCol;
                currentCol = (currentCol % 2) + 1;
                repaint();
                break;
            }
            else if(i == board.length - 1){
                board[i][col] = currentCol;
                currentCol = (currentCol % 2) + 1;
                repaint();
            }
        }
        //printBoard();

        prevStateStr = currentGame.getInfo();
        currentGame = currentGame.move(col);
        curStateStr = currentGame.getInfo();
        if(currentGame.winner()) System.out.println("player " + currentGame.perspective + " wins");
        //currentGame.move(rowPlaced, col, justPlaced);
    }

	@Override
	public void keyPressed(KeyEvent ke) {
		//System.out.println(ke.getKeyCode());
		int keyCode = ke.getKeyCode();
		if(keyCode == 82) { //82, 'r'
			board = new int[7][7];
            currentCol = first;
            currentGame = new GameState(first, 0);
            curStateStr = "";
            prevStateStr = "";
			repaint();
        }
        else if(keyCode == 83) {
            //int[] scores = currentGame.columnScores();
            //System.out.println(Arrays.toString(scores));
            testing();
        }
	}
	@Override
	public void keyReleased(KeyEvent ke) {}
	@Override
	public void keyTyped(KeyEvent ke) {}
	
	@Override
	public void mouseClicked(MouseEvent me) {}
	@Override
	public void mouseEntered(MouseEvent arg0) {}
	@Override
	public void mouseExited(MouseEvent arg0) {}
	@Override
	public void mousePressed(MouseEvent me) {
		// TODO Auto-generated method stub
		Point clicked = me.getPoint();
		//System.out.println(clicked.toString());
		for(int col = 0; col < board[0].length; col++) {
			if(xOffset + squareLen + col * squareLen > clicked.x && col >= 0 && xOffset < clicked.x) {
                if(currentGame.canMove(col)) placePiece(col);
                if(!currentGame.winner()) testing();
				break;
			}
		}
	}

	@Override
    public void mouseReleased(MouseEvent arg0) {}
    
    public void testing(){
        if(currentGame.totalMoves < 13 || currentCol == 1) return;
        int[] columnScore = new int[currentGame.WIDTH];
        int bestScore = Integer.MIN_VALUE;
        int bestColumn = 0;
        for(int col = 0; col < currentGame.WIDTH; col++){
        	columnScore[col] = Integer.MIN_VALUE;
            if(currentGame.canMove(col)){
                GameState nextState = currentGame.move(col);
                if(nextState.loser()) {
                    System.out.println("column " + col + " wins.");
                    columnScore[col] = (nextState.WIDTH * nextState.HEIGHT - currentGame.totalMoves + 1)/2;
                    bestColumn = col;
                    break;
                }
                //System.out.println(nextState.getInfo());
                //System.out.println(currentGame.getInfo());
                int min = -1; //-currentGame.WIDTH * currentGame.HEIGHT / 2;
                int max = 1; //currentGame.WIDTH * currentGame.HEIGHT / 2
                           
                columnScore[col] = -negaMax(nextState, -1, 1); 
                if(columnScore[col] > bestScore) {
                	bestScore = columnScore[col];
                	bestColumn = col;
                }
                else if(columnScore[col] == bestScore) {
                	int random = r.nextInt(1);
                	if(random == 1) 
                		bestColumn = col;
                }
            }
        }
        System.out.println(Arrays.toString(columnScore));
        placePiece(bestColumn);
    }

    private int negaMax(GameState game, int alpha, int beta){
        //for all comments, assume the GameState object is after the ai moves:
        //thus game is from the perspective of the user
        if(game.totalMoves == game.HEIGHT * game.WIDTH - 1)
            return 0;
        for(int col = 0; col < game.WIDTH; col++){
            //game.move gives a GameState object where the perpective is the ai. so when the ai is the loser, return the win score
            //the function that first called negamax was assumed to give the gamestate after the ai moves, so it would negate this score.
            if(game.canMove(col) && game.move(col).loser()){
                int out = (game.HEIGHT * game.WIDTH + 1 - game.totalMoves)/2;
                //System.out.println(out);
                return out;
            }
        }
        int max = (game.HEIGHT*game.WIDTH - game.totalMoves - 1)/2;
        //System.out.println(max);
        //int bestScore = -(game.HEIGHT * game.WIDTH);

        //retreive from transposition table:
        if(storedScore.get(game.key()) != null){
            int ret = storedScore.get(game.key());
            max = ret + game.MINIMUM_SCORE - 1;
        }
        if(beta > max){
            beta = max;
            if(alpha >= beta) return beta;
        }

        for(int i = 0; i < game.WIDTH; i++){
            int col = game.WIDTH/2 + (1-2*(i%2))*(i+1)/2; //formula to start checking the centre column first always
            if(game.canMove(col)){
                GameState nextMove = game.move(col);
                int score = -negaMax(nextMove, -beta, -alpha);
                //if(score > bestScore) bestScore = score;
                if(score >= beta) return score;
                if(score > alpha) alpha = score;
            }
        }
        //System.out.println("best: " + bestScore);
        //return bestScore;
        storedScore.put(game.key(), alpha - game.MINIMUM_SCORE + 1);
        return alpha;
    }

    private void ai() {
        int[] columnScores = new int[board[0].length];
        int bestColumn = 0;
        int highestScore = -100;
        for(int i = 0; i < board[0].length; i++) {
            if(!currentGame.canMove(i)) continue;
            GameState simulated = currentGame.move(i);
            if(simulated.winner()) {
                bestColumn = i;
                break;
            }
            int score = -minimaxPL(simulated, (currentCol % 2) + 1, -1, 1, 10, i);
            System.out.print("Column: " + i + ", ");
            System.out.println("Score: " + score);
            if(score > highestScore){
                bestColumn = i;
                highestScore = score;
            }
            columnScores[i] = score;
        }
        System.out.println(Arrays.toString(columnScores));
        //placePiece(bestColumn);
    }

    private int minimaxPL(GameState game, int player, int alpha, int beta, int depth, int column)  {
        if(game.totalMoves == (board.length - 1)*(board[0].length))
            return 0; //draw
        for(int col = 0; col < board[0].length; col++){
            if(game.move(col).winner())
                return ((board.length - 1)*(board[0].length) - game.totalMoves + 1)/2;
        } //player can win on next turn

        int max = ((board.length - 1)*(board[0].length) - game.totalMoves + 1)/2; // if player were to win on the next move, this is the highest score possible for the player (or lowest for the ai)

        int bestScore = -(board.length - 1)*(board[0].length); //best for the player, worst for ai

        if(depth <= 0) return -100;

		for(int i = 0; i < board[0].length; i++){
            int col = board[0].length/2 + (1-2*(i%2))*(i+1)/2; //formula to start checking the centre column first always
            if(currentGame.canMove(col)){
                GameState nextMove = game.move(col);
                player = (player % 2) + 1;
                depth--;
                int score = -minimaxAI(nextMove, player, -beta, -alpha, depth, col); //negative, since minimaxAI gets the highest ai score. minimizer is trying to make this the lowest
                if(score > bestScore) bestScore = score;
            }
        }

        return bestScore;
    }

    private int minimaxAI(GameState game, int ai, int alpha, int beta, int depth, int column) {
        if(game.totalMoves == (board.length - 1)*(board[0].length))
            return 0; //draw
        for(int col = 0; col < board[0].length; col++){
            if(game.move(col).winner())
                return ((board.length - 1)*(board[0].length) - game.totalMoves + 1)/2;
        } //ai can win on next turn

        int max = ((board.length - 1)*(board[0].length) - game.totalMoves + 1)/2; // if ai were to win on the next move, this is the highest score possible for the ai (or lowest for the ai)

        int bestScore = -(board.length - 1)*(board[0].length); //best for the ai, worst for player

        if(depth <= 0) return 0;

		for(int i = 0; i < board[0].length; i++){
            int col = board[0].length/2 + (1-2*(i%2))*(i+1)/2; //formula to start checking the centre column first always
            if(currentGame.canMove(col)){
                GameState nextMove = game.move(col);
                ai = (ai % 2) + 1;
                depth--;
                int score = -minimaxPL(nextMove, ai, -beta, -alpha, depth, col); //negative, since minimaxAI gets the highest ai score. minimizer is trying to make this the lowest
                if(score > bestScore) bestScore = score;
            }
        }

        return bestScore;
    }

    /*
	private int minimax(GameState game, int currentPlayer, int alpha, int beta) {
        if(game.totalMoves == (board.length - 1)*(board[0].length))
            return 0; //draw

        for(int col = 0; col < board[0].length; col++){
            if(game.move(col, currentPlayer).compWins())
                return ((board.length - 1)*(board[0].length) - game.totalMoves + 1)/2;
        } //wins on next move

        int max = ((board.length - 1)*(board[0].length) - game.totalMoves + 1)/2; // if we were to win on the next move, this is the highest score possible
        
        int storedValue = 100;
        if(currentPlayer == 1){
            storedValue = storedScore.get(game.getBitstrings()[2]);
            if(storedValue != 100) max = storedValue + ((board[0].length * (board.length - 1))/2) - 3 - 1;
        }
        else if(currentPlayer == 2) {
            storedValue = storedScore.get(game.getBitstrings()[0]);
            if(storedValue != 100) max = storedValue - ((board[0].length * (board.length - 1))/2) + 3 - 1;
        }

        if(beta > max){
            beta = max;
            if(alpha >= beta) return beta;
        }

        int bestScore = -(board.length - 1)*(board[0].length);

		for(int i = 0; i < board[0].length; i++){
            int col = board[0].length/2 + (1-2*(i%2))*(i+1)/2; //formula to start checking the centre column first always
            if(currentGame.canMove(col)){
                GameState nextMove = game.move(col, currentPlayer);
                currentPlayer = (currentPlayer % 2) + 1;
                int score = -minimax(nextMove, currentPlayer, -beta, -alpha);
                if(score >= beta) return score;
                if(score > alpha) alpha = score;
            }
        }
        if(currentPlayer == 1){
            storedScore.put(game.getBitstrings()[2], alpha - ((board[0].length * (board.length - 1))/2) + 3 + 1);
        }
        else if(currentPlayer == 2) {
            storedScore.put(game.getBitstrings()[0], alpha + ((board[0].length * (board.length - 1))/2) - 3 + 1);
        }
        storedScore.put(game., alpha - Position::MIN_SCORE + 1);
        return alpha;
	}*/

	private int minimaxDepth(GameState game, int currentPlayer, int alpha, int beta, int depth, int column) {
        if(game.totalMoves == (board.length - 1)*(board[0].length))
            return 0; //draw

        for(int col = 0; col < board[0].length; col++){
            if(game.move(col).winner())
                return ((board.length - 1)*(board[0].length) - game.totalMoves + 1)/2;
        } //wins on next move

        int max = ((board.length - 1)*(board[0].length) - game.totalMoves + 1)/2; // if we were to win on the next move, this is the highest score possible

        if(beta > max){
            beta = max;
            if(alpha >= beta) return beta;
        }

        int bestScore = -(board.length - 1)*(board[0].length);

        if(depth <= 0) return 0;

		for(int i = 0; i < board[0].length; i++){
            int col = board[0].length/2 + (1-2*(i%2))*(i+1)/2; //formula to start checking the centre column first always
            if(currentGame.canMove(col)){
                GameState nextMove = game.move(col);
                currentPlayer = (currentPlayer % 2) + 1;
                depth--;
                int score = -minimaxDepth(nextMove, currentPlayer, -beta, -alpha, depth, col);
                if(score >= beta) return score;
                if(score > alpha) alpha = score;
            }
        }

        return alpha;
	}
	
	
}

