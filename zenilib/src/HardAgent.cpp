#include "zenilib.h"

#include "HardAgent.h"
#include "MediumAgent.h"
#include "GameModel.h"
#include "Board.h"


HardAgent::HardAgent(int playerIndex_) 
	: MediumAgent(playerIndex_)
{
	for (int i = 0; i < NUM_COLUMNS; i++) {
		values[i] = 0;
	}
}

void HardAgent::getValue(int index) {
	GameModel& model = getGameModel();
	Board *board = model.getBoard();

	// Chooses the first location at random
	if (board->getNumCoins() == 1 || board->getNumCoins() == 0) {
		values[index] = 0;
		return;
	}

	int emptyRow = board->getEmptyRow(index);

	// Filling in values
	for (int i = 1; i < 4; i++) {
		// Check horizontal rows
		if (board->ownerOfSlot(emptyRow, index + i) == playerIndex) values[index] += 2 * (4 - i) * (4 - i);
		if (board->ownerOfSlot(emptyRow, index - i) == playerIndex) values[index] += 2 * (4 - i) * (4 - i);
		
		// Check below
		if (board->ownerOfSlot(emptyRow - i, index) == playerIndex) values[index] += 4 * (4 - i) * (4 - i);

		// Check diagnols
		if (board->ownerOfSlot(emptyRow - i, index + i) == playerIndex) values[index] += 1 * (4 - i) * (4 - i);
		if (board->ownerOfSlot(emptyRow - i, index - i) == playerIndex) values[index] += 1 * (4 - i) * (4 - i);
		if (board->ownerOfSlot(emptyRow + i, index + i) == playerIndex) values[index] += 1 * (4 - i) * (4 - i);
		if (board->ownerOfSlot(emptyRow + i, index - i) == playerIndex) values[index] += 1 * (4 - i) * (4 - i);

		// Take away value for empty spaces
		if (board->ownerOfSlot(emptyRow, index + i) == -1) values[index] -= 1 * (4 - i) * (4 - i);
		if (board->ownerOfSlot(emptyRow, index - i) == -1) values[index] -= 1 * (4 - i) * (4 - i);

		if (board->ownerOfSlot(emptyRow - i, index + i) == -1) values[index] -= 1 * (4 - i) * (4 - i);
		if (board->ownerOfSlot(emptyRow - i, index - i) == -1) values[index] -= 1 * (4 - i) * (4 - i);
		if (board->ownerOfSlot(emptyRow + i, index + i) == -1) values[index] -= 1 * (4 - i) * (4 - i);
		if (board->ownerOfSlot(emptyRow + i, index - i) == -1) values[index] -= 1 * (4 - i) * (4 - i);
	}

	// Prioritize winning
	if (board->threeInARow2(index, playerIndex)) {
		values[index] += 800;
	}
	// If opponent is about to win, stop him
	if (board->threeInARow2(index, (playerIndex == 0) ? 1 : 0)) {
		values[index] += 400;
	}

	if (twoInRow(index)) {
		values[index] += 100;
	}

	// Avoid putting coin in place where player can exploit a win resulting from Agent's coin
	if (board->lookAheadWin(index, (playerIndex == 0) ? 1 : 0)) {
		values[index] -= 1000;
	}

	// Don't put coin in full column
	if (board->columnFull(index)) {
		values[index] -= 1000;
	}
}
