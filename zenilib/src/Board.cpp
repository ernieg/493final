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

	// make the model
	// change this when the model has been repositioned !!
	Zeni::Model* board_model = new Zeni::Model("models/boardStandard.3DS");
	
	//board_model->set_translate(BOARD_CENTER + Zeni::Point3f(0.0f,25.0f,0.0f));
	//board_model->set_rotate(-Zeni::pi/2.0f,Zeni::Vector3f(0.0f,0.0f,1.0f));
	//board_model->set_scale(Zeni::Vector3f(7.0f,7.0f,7.0f));

	board_model->set_translate(BOARD_CENTER);
	board_model->set_scale(BOARD_SCALE);
	
	GameObject::setModel(board_model);

	// set the collision planes for the columns
	columnPlanes.resize(numCols);

	// the center column is numCols/2 (3 in the general case)
	for (int i = 0;  i < numCols; i++)
	{
		// y-coordinate of the center of each circle
		Zeni::Point3f centerPoint(BOARD_DIST_X, static_cast<float>(numCols/2-i)*2.0f*BOARD_SCALE.y, BOARD_SCALE.z);
		// this is an XZ plane, so the normal is a Y vector
		Zeni::Vector3f normal(0.0f,1.0f,0.0f);

		Zeni::Collision::Plane colPlane(centerPoint,normal);

		columnPlanes[i] = colPlane;
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

			// set the coin's position to be in the right slot
			float yPos = static_cast<float>(numCols/2-column)*2.0f*BOARD_SCALE.y;
			
			float zPos = 2.0f * i * BOARD_SCALE.z;

			coin->setPosition(Zeni::Point3f(BOARD_DIST_X, yPos, zPos));
			return true;
		}
	}

	// no empty slots in this column
	return false;
}

// no a simple checkWin() as well!

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

int Board::checkCollide(Coin* coin)
{
	// make a temporary collision sphere for the coin
	Zeni::Collision::Sphere coinSphere(coin->getPosition(),COIN_SCALE.x*2.0f);

	// check if it collides with any columns
	for (int i = 0; i < numCols; i++)
	{
		if ( coinSphere.intersects(columnPlanes[i]) )
		{
			return i;
		}
	}

	// no collision
	return -1;
}

void Board::reset()
{

}

void Board::render()
{
	GameObject::getModel()->render();

	// need to render all the coins in the board as well
	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numCols; j++)
		{
			if ( board[i][j] != NULL )
			{
				board[i][j]->render();
			}
		}
	}
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