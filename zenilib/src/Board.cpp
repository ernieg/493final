#include "zenilib.h"
#include "Board.h"

Board::Board(int numRows_, int numCols_)
	:numRows(numRows_), numCols(numCols_), recentColumn(-1), recentRow(-1), numCoins(0)
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
	assert(coin != NULL);
	assert(column >= 0 && column < numCols);

	// check from lowest row to highest row for an empty slot
	for (int i = 0; i < numRows; i++)
	{
		if ( board[i][column] == NULL )
		{
			board[i][column] = coin;
			numCoins++;
			recentRow = i;
			recentColumn = column;
			return true;
		}
	}

	// no empty slots in this column
	return false;
}

int Board::checkWin()
{
	// the slot with the most recent coin in it should be valid
	assert(recentRow != -1 && recentColumn != -1);
	assert(board[recentRow][recentColumn] != NULL);
	assert(board[recentRow][recentColumn]->getPlayerIndex() == 0 ||
		   board[recentRow][recentColumn]->getPlayerIndex() == 1 );

	// check horizontal win cases
	for (int k = 0; k < 4; k++)
	{
		if ( ownerOfSlot(recentRow,recentColumn-k)   == 
			 ownerOfSlot(recentRow,recentColumn-k+1) ==
			 ownerOfSlot(recentRow,recentColumn-k+2) ==
			 ownerOfSlot(recentRow,recentColumn-k+3) )
		{
			return ownerOfSlot(recentRow,recentColumn-k);
		}
	}

	// check vertical win cases
	for (int k = 0; k < 4; k++)
	{
		if ( ownerOfSlot(recentRow-k,recentColumn)   ==
			 ownerOfSlot(recentRow-k+1,recentColumn) ==
			 ownerOfSlot(recentRow-k+2,recentColumn) ==
			 ownerOfSlot(recentRow-k+3,recentColumn) )
		{
			return ownerOfSlot(recentRow-k,recentColumn);
		}
	}

	// check diagonal (/) cases
	for (int k = 0; k < 4; k++)
	{
		if ( ownerOfSlot(recentRow-k,recentColumn-k)     ==
			 ownerOfSlot(recentRow-k+1,recentColumn-k+1) ==
			 ownerOfSlot(recentRow-k+2,recentColumn-k+2) ==
			 ownerOfSlot(recentRow-k+3,recentColumn-k+3) )
		{
			return ownerOfSlot(recentRow-k,recentColumn-k);
		}
	}

	// check diagonal (\) cases
	for (int k = 0; k < 4; k++)
	{
		if ( ownerOfSlot(recentRow+k,recentColumn-k)     ==
			 ownerOfSlot(recentRow+k-1,recentColumn-k+1) ==
			 ownerOfSlot(recentRow+k-2,recentColumn-k+2) == 
			 ownerOfSlot(recentRow+k-3,recentColumn-k+3) )
		{
			return ownerOfSlot(recentRow+k,recentColumn-k);
		}
	}

	// there is no winner, but there might be a draw
	if ( numCoins == numRows*numCols )
	{
		return -2;
	}

	// the game is still in progress
	return -1;
}

void Board::reset()
{

}

void Board::render()
{

}

int Board::ownerOfSlot(int row, int col)
{
	if ( row < 0 || row > numRows-1 ) // row number is off the board 
	{
		return -2;
	}
	else if ( col < 0 || col > numCols-1 ) // column number is off the board
	{
		return -2;
	}
	else if ( board[row][col] == NULL ) // slot is empty
	{
		return -1;
	}
	else // there is a coin at this slot
	{
		return board[row][col]->getPlayerIndex();
	}
}