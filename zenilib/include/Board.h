#ifndef BOARD_H
#define BOARD_H

#include "Coin.h"


class Board : public GameObject
{
public:
	bool putCoin(Coin* coin, int column); // returns false if the column is full, and true otherwise
	int checkWin(); // returns which player index has won the game, -1 if there is no winner, and -2 if there is a draw
	void reset();
	void render();

	Board(int numRows_ = 6, int numCols_ = 7);

private:
	// returns the player index of the coin in this slot
	// or -1 if the slot is empty
	// or -2 if the slot is off the board
	int ownerOfSlot(int row, int col);

	std::vector< std::vector<Coin*> > board; // board[i][j] is null if the slot at row i, col j is empty
										     // the bottom left corner is board[0][0]
	int numRows;
	int numCols;
	int numCoins; // number of coins on the board

	int recentRow; // the row that most recently had a coin put in it, or -1 for the start of the game
	int recentColumn; // the column that most recently had a coin put in it, or -1 for the start of the game
};

#endif // BOARD_H