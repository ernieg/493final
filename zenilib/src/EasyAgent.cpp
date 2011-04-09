#include "zenilib.h"
#include "Player.h"
#include "AgentBase.h"
#include "EasyAgent.h"

using namespace Zeni;

EasyAgent::EasyAgent(int playerIndex_)
	: AgentBase(playerIndex_)
{
}

void EasyAgent::getNewDestination() {
	Random rand;
	destination = rand.rand_lt(7);
}