#ifndef AGENTBASE_H
#define AGENTBASE_H

class AgentBase : public Player {

public:
	AgentBase(int playerIndex_);

	void perform_logic();

	void handleWiiEvent() {}
	void renderHand() {} // So hand isn't rendered when AI is playing

protected:
	void moveChip();
	void calculateDirection();
	virtual void getNewDestination() = 0;

	bool locationFound;		// False if looking for position
	int destination;		// Column 
	Zeni::Vector3f direction;
};

#endif