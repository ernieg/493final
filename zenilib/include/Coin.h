#ifndef COIN_H
#define COIN_H

#include "GameObject.h"

const Zeni::Vector3f COIN_SCALE(5.0f,5.0f,5.0f);

class Coin : public GameObject
{
public:
	void render();
	Coin(int playerIndex_);

	int getPlayerIndex();

private:
	int playerIndex; // playerIndex of the Player who owns the coin
};

#endif // COIN_H