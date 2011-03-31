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

private:
	std::vector< std::vector<Coin*> > board;
}

#endif // BOARD_H