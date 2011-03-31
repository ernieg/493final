#include "zenilib.h"
#include "Board.h"

Board::Board(int numRows_, int numCols_)
	:numRows(numRows_), numCols(numCols_)
{
	// set the number of rows
	board.resize(numRows);

	// set each row
	for (int i = 0; i < numRows; i++)
	{
		board[i].resize(numCols);
	}

	// make everything null
	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numCols; j++)
		{
			board[i][j] = NULL;
		}
	}
}

bool Board::putCoin(Coin* coin, int column)
{
	return false;
}

int Board::checkWin()
{
	return -1;
}

void Board::reset()
{

}

void Board::render()
{

}