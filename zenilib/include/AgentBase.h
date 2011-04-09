#ifndef AGENTBASE_H
#define AGENTBASE_H

#include "Player.h"

const Zeni::Vector3f DIRECTION(0.0f, -60.0f, 0.0f);

class AgentBase : public Player {

public:
	AgentBase(int playerIndex_);

	void perform_logic(float &timeStep);

	void handleWiiEvent() {}
	void renderHand() {} // So hand isn't rendered when AI is playing

protected:
	void moveChip(float &timeStep);
	virtual void getNewDestination() = 0;

	bool locationFound;		// False if looking for position
	int destination;		// Column 
	float moveDelay;
};

#endif