#ifndef COIN_H
#define COIN_H

#include "GameObject.h"

const Zeni::Vector3f COIN_SCALE(5.0f,5.0f,5.0f);
const float ROTATE_SCALE = 5.0f;
const float FALL_SCALE = 150.0f;

class Coin : public GameObject
{
public:
	void render();
	Coin(int playerIndex_);

	int getPlayerIndex();
	void rotate(float timeStep); // for rotating the current coin

	void setFinalPosition(Zeni::Point3f finalPosition);
	Zeni::Point3f getFinalPosition();

private:
	int playerIndex; // playerIndex of the Player who owns the coin
	float rotation;
	Zeni::Point3f finalPosition; // when the coin is falling down, this is its final position
};

#endif // COIN_H