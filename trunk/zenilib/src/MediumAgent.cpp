#include "zenilib.h"

#include "Player.h"
#include "AgentBase.h"
#include "MediumAgent.h"
#include "Board.h"

using namespace Zeni;

MediumAgent::MediumAgent(int playerIndex_)
	: AgentBase(playerIndex_)
{
	for (int i = 0; i < NUM_COLUMNS; i++) {
		values[i] = 0;
	}
}

void MediumAgent::getNewDestination() {
	destination = 0;
	resetValues();
	for (int i = 0; i < NUM_COLUMNS; i++) {
		getValue(i);
	}

	// If two or more values come out to be the same, randomly pick one
	potentialDestinations.clear();
	potentialDestinations.push_back(0);
	for (int i = 1; i < NUM_COLUMNS; i++) {
		if (values[i] > values[potentialDestinations[0]]) {
			potentialDestinations.clear();
			potentialDestinations.push_back(i);
		} else if (values[i] == values[potentialDestinations[0]]){
			potentialDestinations.push_back(i);
		}
	}

	if (potentialDestinations.size() == 1) {
		destination = potentialDestinations[0];
	} else {
		destination = getRandomDestination();
	}
}

void MediumAgent::getValue(int index) {
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

	if (board->threeInARow(index, playerIndex)) {
		values[index] += 800;
	}
	// If opponent is about to win, stop him
	if (board->threeInARow(index, (playerIndex == 0) ? 1 : 0)) {
		values[index] += 400;
	}

	if (twoInRow(index)) {
		values[index] += 100;
	}

	// Don't put coin in full column
	if (board->columnFull(index)) {
		values[index] -= 1000;
	}
}

bool MediumAgent::twoInRow(int col) {
	GameModel& model = getGameModel();
	Board *board = model.getBoard();

	int emptyRow = board->getEmptyRow(col);

	if (board->ownerOfSlot(emptyRow, col - 1) == (playerIndex == 0) ? 1 : 0 &&
		board->ownerOfSlot(emptyRow, col - 2) == (playerIndex == 0) ? 1 : 0) {
			return true;
	}
	if (board->ownerOfSlot(emptyRow, col + 1) == (playerIndex == 0) ? 1 : 0 &&
		board->ownerOfSlot(emptyRow, col + 2) == (playerIndex == 0) ? 1 : 0) {
			return true;
	}	

	return false;
}

void MediumAgent::resetValues() {
	for (int i = 0; i < NUM_COLUMNS; i++) {
		values[i] = 0;
	}
}

int MediumAgent::getRandomDestination() {
	Random rand;
	return potentialDestinations[rand.rand_lt(potentialDestinations.size())];
}