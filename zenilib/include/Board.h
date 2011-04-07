#ifndef BOARD_H
#define BOARD_H

#include "Coin.h"

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

	Board(int numRows_ = 6, int numCols_ = 7);

private:
	// returns the player index of the coin in this slot
	// or -1 if the slot is empty
	// or -2 if the slot is off the board
	int ownerOfSlot(int row, int col);
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