#include "zenilib.h"
#include "Board.h"
#include "GameModel.h"

Coin::Coin(int playerIndex_)
	:playerIndex(playerIndex_)
{
	// get the right 3D model
	if ( playerIndex == 0 )
	{
		GameObject::setModel(getGameModel().getPlayerZeroModel());
		assert(GameObject::getModel() != NULL);
	}
	else
	{
		GameObject::setModel(getGameModel().getPlayerOneModel());
		assert(GameObject::getModel() != NULL);
	}

	// put the coin in a default position
	GameObject::setPosition(Zeni::Point3f(BOARD_DIST_X,50.0f,100.0f));
}

void Coin::render()
{
	// debugging
	assert(GameObject::getModel() != NULL);

	// move the model to the coin's current position
	GameObject::getModel()->set_translate(GameObject::getPosition());
	GameObject::getModel()->set_scale(COIN_SCALE);

	GameObject::getModel()->render();
}

int Coin::getPlayerIndex()
{
	return playerIndex;
}