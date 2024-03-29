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

	// for spinning the winning coins
	winningCoins.resize(4);
	winningCoins[0] = winningCoins[1] = winningCoins[2] = winningCoins[3] = NULL;
}

bool Board::putCoin(Coin* coin, int column)
{
	assert(coin != NULL);
	assert(column >= 0 && column < numCols);

	// if a player has already won, don't let the other player put coins on the board
	if ( getGameModel().getGameOver() )
	{
		return false;
	}

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

			
			coin->setFinalPosition(Zeni::Point3f(BOARD_DIST_X, yPos, zPos));

			// also set the coin's rotation to be aligned with the board
			coin->setOrientation(Zeni::Quaternion::Axis_Angle(Zeni::Vector3f(0.0f,0.0f,1.0f),0.0f));
			
			// make the coin fall from its current position to the board
			movingCoins.push_back(coin);

			return true;
		}
	}

	// no empty slots in this column
	return false;
}

void Board::advanceMovingCoins(float timeStep)
{
	// advance the falling coins
	for (int i = 0; i < movingCoins.size(); i++)
	{
		Zeni::Vector3f moveVector = movingCoins[i]->getFinalPosition() - movingCoins[i]->getPosition();
		moveVector.normalize();

		movingCoins[i]->setPosition(movingCoins[i]->getPosition() + moveVector * FALL_SCALE * timeStep);
	}

	// set them in their exact final positions
	for (int i = 0; i < movingCoins.size(); i++)
	{
		if ( (movingCoins[i]->getPosition() - movingCoins[i]->getFinalPosition()).magnitude() < BOARD_SCALE.z )
		{
			movingCoins[i]->setPosition(movingCoins[i]->getFinalPosition());
			movingCoins.erase(movingCoins.begin()+i);
			i--;

			// play a sound to give the player more feedback
			Zeni::play_sound("coin_drop_1");
		}
	}
}

int Board::simpleCheckWin()
{
	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numCols; j++)
		{
			if ( ownerOfSlot(i,j) == 0 || ownerOfSlot(i,j) == 1 )
			{
				// not implemented yet -- normal checkWin() should be working!

				// horizontal

				// vertical

				// diagonal (\)

				// diagonal (/)
			}
		}
	}
  return 0;
}

bool Board::allEqual(int a, int b, int c, int d)
{
	if ( (a == b) && (b == c) && (c == d) )
	{
		return true;
	}
	else
	{
		return false;
	}
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
		if ( allEqual(ownerOfSlot(recentRow,recentColumn-k),
					  ownerOfSlot(recentRow,recentColumn-k+1),
					  ownerOfSlot(recentRow,recentColumn-k+2),
					  ownerOfSlot(recentRow,recentColumn-k+3)) )
		{
			for (int w = 0; w < 4; w++)
			{
				winningCoins[w] = board[recentRow][recentColumn-k+w];
			}
		
			return ownerOfSlot(recentRow,recentColumn-k);
		}
	}

	// check vertical win cases
	for (int k = 0; k < 4; k++)
	{
		if ( allEqual(ownerOfSlot(recentRow-k,recentColumn),
					  ownerOfSlot(recentRow-k+1,recentColumn),
					  ownerOfSlot(recentRow-k+2,recentColumn),
					  ownerOfSlot(recentRow-k+3,recentColumn)) )
		{
			for (int w = 0; w < 4; w++)
			{
				winningCoins[w] = board[recentRow-k+w][recentColumn];
			}

			return ownerOfSlot(recentRow-k,recentColumn);
		}
	}

	// check diagonal (/) cases
	for (int k = 0; k < 4; k++)
	{
		if ( allEqual(ownerOfSlot(recentRow-k,recentColumn-k),
					  ownerOfSlot(recentRow-k+1,recentColumn-k+1),
					  ownerOfSlot(recentRow-k+2,recentColumn-k+2),
					  ownerOfSlot(recentRow-k+3,recentColumn-k+3)) )
		{
			for (int w = 0; w < 4; w++)
			{
				winningCoins[w] = board[recentRow-k+w][recentColumn-k+w];
			}

			return ownerOfSlot(recentRow-k,recentColumn-k);
		}
	}

	// check diagonal (\) cases
	for (int k = 0; k < 4; k++)
	{
		if ( allEqual(ownerOfSlot(recentRow+k,recentColumn-k),
					  ownerOfSlot(recentRow+k-1,recentColumn-k+1),
					  ownerOfSlot(recentRow+k-2,recentColumn-k+2), 
					  ownerOfSlot(recentRow+k-3,recentColumn-k+3)) )
		{
			for (int w = 0; w < 4; w++)
			{
				winningCoins[w] = board[recentRow+k-w][recentColumn-k+w];
			}

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
	recentRow = -1;
	recentColumn = -1;

	numCoins = 0;

	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numCols; j++)
		{
			if ( board[i][j] != NULL )
			{
				delete board[i][j];
				board[i][j] = NULL;
			}
		}
	}

	movingCoins.clear();

	winningCoins[0] = winningCoins[1] = winningCoins[2] = winningCoins[3] = NULL;
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

int Board::numMovingCoins()
{
	return movingCoins.size();
}

bool Board::columnFull(int col) {
	for (int i = 0; i < numCols; i++) {
		if (ownerOfSlot(i, col) == -1) {
			return false;
		}
	}
	return true;
}

bool Board::threeInARow(int col, int playerIndex) {
	int emptyRow = getEmptyRow(col);

	// Check horizontal
	if (ownerOfSlot(emptyRow, col + 1) == playerIndex &&
		ownerOfSlot(emptyRow, col + 2) == playerIndex &&
		ownerOfSlot(emptyRow, col + 3) == playerIndex) {
			return true;
	}
	if (ownerOfSlot(emptyRow, col - 1) == playerIndex &&
		ownerOfSlot(emptyRow, col - 2) == playerIndex &&
		ownerOfSlot(emptyRow, col - 3) == playerIndex) {
			return true;
	}

	// Check below
	if (ownerOfSlot(emptyRow - 1, col) == playerIndex &&
		ownerOfSlot(emptyRow - 2, col) == playerIndex &&
		ownerOfSlot(emptyRow - 3, col) == playerIndex) {
			return true;
	}

	// Check diagnols
	if (ownerOfSlot(emptyRow - 1, col - 1) == playerIndex &&
		ownerOfSlot(emptyRow - 2, col - 2) == playerIndex &&
		ownerOfSlot(emptyRow - 3, col - 3) == playerIndex) {
			return true;
	}
	if (ownerOfSlot(emptyRow - 1, col + 1) == playerIndex &&
		ownerOfSlot(emptyRow - 2, col + 2) == playerIndex &&
		ownerOfSlot(emptyRow - 3, col + 3) == playerIndex) {
			return true;
	}
	if (ownerOfSlot(emptyRow + 1, col - 1) == playerIndex &&
		ownerOfSlot(emptyRow + 2, col - 2) == playerIndex &&
		ownerOfSlot(emptyRow + 3, col - 3) == playerIndex) {
			return true;
	}
	if (ownerOfSlot(emptyRow + 1, col + 1) == playerIndex &&
		ownerOfSlot(emptyRow + 2, col + 2) == playerIndex &&
		ownerOfSlot(emptyRow + 3, col + 3) == playerIndex) {
			return true;
	}

	return false;
}

bool Board::threeInARow2(int col, int playerIndex) {
	// If lesser function is true, then this is obviously true
	if (threeInARow(col, playerIndex)) {
		return true;
	}

	int emptyRow = getEmptyRow(col);
	
	// Check if inserting chip amongst other chips will win
	
	// Horizontal
	if (ownerOfSlot(emptyRow, col + 1) == playerIndex &&
		ownerOfSlot(emptyRow, col + 2) == playerIndex && 
		ownerOfSlot(emptyRow, col - 1) == playerIndex) {
			return true;
	}
	if (ownerOfSlot(emptyRow, col - 1) == playerIndex &&
		ownerOfSlot(emptyRow, col - 2) == playerIndex && 
		ownerOfSlot(emptyRow, col + 1) == playerIndex) {
			return true;
	}

	// Below
	// JK! Doesn't need to be done ^^

	// Diagnol
	if (ownerOfSlot(emptyRow - 1, col - 1) == playerIndex &&
		ownerOfSlot(emptyRow - 2, col - 2) == playerIndex && 
		ownerOfSlot(emptyRow + 1, col + 1) == playerIndex) {
			return true;
	}
	if (ownerOfSlot(emptyRow + 1, col + 1) == playerIndex &&
		ownerOfSlot(emptyRow + 2, col + 2) == playerIndex && 
		ownerOfSlot(emptyRow - 1, col - 1) == playerIndex) {
			return true;
	}
	if (ownerOfSlot(emptyRow - 1, col + 1) == playerIndex &&
		ownerOfSlot(emptyRow - 2, col + 2) == playerIndex && 
		ownerOfSlot(emptyRow + 1, col - 1) == playerIndex) {
			return true;
	}
	if (ownerOfSlot(emptyRow - 1, col + 1) == playerIndex &&
		ownerOfSlot(emptyRow + 1, col - 1) == playerIndex && 
		ownerOfSlot(emptyRow + 2, col - 2) == playerIndex) {
			return true;
	}
	return false;
}

bool Board::lookAheadWin(int col, int playerIndex) {
	int emptyRow = getEmptyRow(col);
	emptyRow++;

	// Check horizontal
	if (ownerOfSlot(emptyRow, col + 1) == playerIndex &&
		ownerOfSlot(emptyRow, col + 2) == playerIndex &&
		ownerOfSlot(emptyRow, col + 3) == playerIndex) {
			return true;
	}
	if (ownerOfSlot(emptyRow, col - 1) == playerIndex &&
		ownerOfSlot(emptyRow, col - 2) == playerIndex &&
		ownerOfSlot(emptyRow, col - 3) == playerIndex) {
			return true;
	}

	// Check below
	if (ownerOfSlot(emptyRow - 1, col) == playerIndex &&
		ownerOfSlot(emptyRow - 2, col) == playerIndex &&
		ownerOfSlot(emptyRow - 3, col) == playerIndex) {
			return true;
	}

	// Check diagnols
	if (ownerOfSlot(emptyRow - 1, col - 1) == playerIndex &&
		ownerOfSlot(emptyRow - 2, col - 2) == playerIndex &&
		ownerOfSlot(emptyRow - 3, col - 3) == playerIndex) {
			return true;
	}
	if (ownerOfSlot(emptyRow - 1, col + 1) == playerIndex &&
		ownerOfSlot(emptyRow - 2, col + 2) == playerIndex &&
		ownerOfSlot(emptyRow - 3, col + 3) == playerIndex) {
			return true;
	}
	if (ownerOfSlot(emptyRow + 1, col - 1) == playerIndex &&
		ownerOfSlot(emptyRow + 2, col - 2) == playerIndex &&
		ownerOfSlot(emptyRow + 3, col - 3) == playerIndex) {
			return true;
	}
	if (ownerOfSlot(emptyRow + 1, col + 1) == playerIndex &&
		ownerOfSlot(emptyRow + 2, col + 2) == playerIndex &&
		ownerOfSlot(emptyRow + 3, col + 3) == playerIndex) {
			return true;
	}

	// Horizontal
	if (ownerOfSlot(emptyRow, col + 1) == playerIndex &&
		ownerOfSlot(emptyRow, col + 2) == playerIndex && 
		ownerOfSlot(emptyRow, col - 1) == playerIndex) {
			return true;
	}
	if (ownerOfSlot(emptyRow, col - 1) == playerIndex &&
		ownerOfSlot(emptyRow, col - 2) == playerIndex && 
		ownerOfSlot(emptyRow, col + 1) == playerIndex) {
			return true;
	}

	// Below
	// JK! Doesn't need to be done ^^

	// Diagnol
	if (ownerOfSlot(emptyRow - 1, col - 1) == playerIndex &&
		ownerOfSlot(emptyRow - 2, col - 2) == playerIndex && 
		ownerOfSlot(emptyRow + 1, col + 1) == playerIndex) {
			return true;
	}
	if (ownerOfSlot(emptyRow + 1, col + 1) == playerIndex &&
		ownerOfSlot(emptyRow + 2, col + 2) == playerIndex && 
		ownerOfSlot(emptyRow - 1, col - 1) == playerIndex) {
			return true;
	}
	if (ownerOfSlot(emptyRow - 1, col + 1) == playerIndex &&
		ownerOfSlot(emptyRow - 2, col + 2) == playerIndex && 
		ownerOfSlot(emptyRow + 1, col - 1) == playerIndex) {
			return true;
	}
	if (ownerOfSlot(emptyRow - 1, col + 1) == playerIndex &&
		ownerOfSlot(emptyRow + 1, col - 1) == playerIndex && 
		ownerOfSlot(emptyRow + 2, col - 2) == playerIndex) {
			return true;
	}
	return false;
}

int Board::getNumCoins() {
	return numCoins;
}

int Board::getEmptyRow(int col) {
	// Get first row value that isn't taken
	for (int emptyRow = 0; emptyRow < numRows; emptyRow++) {
		if (ownerOfSlot(emptyRow, col) == -1) {
			return emptyRow;
		}
	}
	return -1;
}