#include "zenilib.h"
#include "Coin.h"

Coin::Coin(int playerIndex_)
	:playerIndex(playerIndex_)
{
}

void Coin::render()
{
	return;
}

int Coin::getPlayerIndex()
{
	return playerIndex;
}