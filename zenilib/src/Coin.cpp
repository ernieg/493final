#include "zenilib.h"
#include "Board.h"
#include "GameModel.h"

Coin::Coin(int playerIndex_)
	:playerIndex(playerIndex_)
{
  setModel(getGameModel().getPlayerModel(playerIndex));
	assert(GameObject::getModel() != NULL);

	// put the coin in a default position
	setPosition(Zeni::Point3f(BOARD_DIST_X,50.0f,100.0f));
}

void Coin::render()
{
	// debugging
	assert(GameObject::getModel() != NULL);

	// move the model to the coin's current position
	getModel()->set_translate(GameObject::getPosition());
	getModel()->set_scale(COIN_SCALE);

	getModel()->render();
}

int Coin::getPlayerIndex()
{
	return playerIndex;
}