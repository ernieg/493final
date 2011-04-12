#ifndef HARDAGENT_H
#define HARDAGENT_H

#include "AgentBase.h"
#include "MediumAgent.h"

class HardAgent : public MediumAgent {
public:
	HardAgent(int playerIndex_);

protected:
	void getValue(int index);
};

#endif