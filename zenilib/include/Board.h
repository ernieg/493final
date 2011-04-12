#ifndef BOARD_H
#define BOARD_H

#include "Coin.h"
#include "GameModel.h"

// distance from the game board to the camera.
// the camera is at the origin initially (player 0)
const float BOARD_DIST_X = 100.0f;
const Zeni::Point3f BOARD_CENTER(BOARD_DIST_X,0.0f,0.0f); // the bottom-center
const Zeni::Point3f BOARD_CENTER_MIDDLE(BOARD_DIST_X,0.0f,50.0f); // the rough center of the board
const Zeni::Vector3f BOARD_SCALE(7.0f,7.0f,7.0f);

class Board : public GameObject
{
public:
	bool putCoin(Coin* coin, int column); // returns false if the column is full, and true otherwise
	int checkWin(); // returns which player index has won the game, -1 if there is no winner, and -2 if there is a draw
	int simpleCheckWin();
	int checkCollide(Coin* coin); // returns the column number of the column the coin is on top of, or -1 if there is no collision
	void reset();
	void render();
	void advanceMovingCoins(float timeStep);
	int numMovingCoins();
	int getNumCoins(); // number of coins in board
	// returns the player index of the coin in this slot
	// or -1 if the slot is empty
	// or -2 if the slot is off the board
	int ownerOfSlot(int row, int col);
	int getEmptyRow(int col);					// returns the first instance of an empty row or -1 if column full
	bool columnFull(int col);
	bool threeInARow(int col, int playerIndex); // returns true if placing a coin in that column will win the game
												// for playerIndex. also handles placing a coin in the middle of
												// a row for a win
	bool threeInARow2(int col, int playerIndex);// Confusing name I know, will think of something better later
												// More demanding checks. Finds ALL cases where win is imminent
	bool lookAheadWin(int col, int playerIndex);// returns true if action results in win for opponent
												// poorly written.. could could use improvement

	Board(int numRows_ = 6, int numCols_ = 7);

private:

	bool allEqual(int a, int b, int c, int d);

	std::vector< std::vector<Coin*> > board; // board[i][j] is null if the slot at row i, col j is empty
										     // the bottom left corner is board[0][0]
	std::vector<Zeni::Collision::Plane> columnPlanes; // an XZ plane for each column
													  // numbered 0-numCols-1 from left to right
	
	int numRows;
	int numCols;
	int numCoins; // number of coins on the board

	int recentRow; // the row that most recently had a coin put in it, or -1 for the start of the game
	int recentColumn; // the column that most recently had a coin put in it, or -1 for the start of the game

	std::vector<Coin*> movingCoins; // the coins currently falling down
};

#endif // BOARD_H