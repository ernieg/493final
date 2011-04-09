#ifndef EASYAGENT_H
#define EASYAGENT_H

#include "AgentBase.h" 

class EasyAgent : public AgentBase {

public:
	EasyAgent(int playerIndex_);
	void getNewDestination();
};

#endif