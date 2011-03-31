#ifndef COIN_H
#define COIN_H

#include "GameObject.h"

class Coin : public GameObject
{
public:
	void render();
	Coin(int playerIndex_);

	int getPlayerIndex();

private:
	static Zeni::Model* PlayerZeroModel;
	static Zeni::Model* PlayerOneModel;
	int playerIndex; // playerIndex of the Player who owns the coin
};

#endif // COIN_H