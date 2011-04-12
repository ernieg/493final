#ifndef MEDIUMAGENT_H
#define MEDIUMAGENT_H

#include "AgentBase.h"

const int NUM_COLUMNS = 7;
const int NUM_ROWS = 6;

class MediumAgent : public AgentBase {

public:
	MediumAgent(int playerIndex_);
	void getNewDestination();

protected:
	virtual void getValue(int index);		// returns the values of the column at index i
	void resetValues();
	int getRandomDestination();		// returns a random value in potentialDestinations
	bool twoInRow(int index);		// This is mostly just to prevent early game wins

	int values[NUM_COLUMNS];		// heuristic value for each column
	std::vector<int> potentialDestinations;
};

#endif