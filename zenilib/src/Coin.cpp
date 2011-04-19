#include "zenilib.h"
#include "Board.h"
#include "GameModel.h"

using namespace std;
using namespace Zeni;

Coin::Coin(int playerIndex_)
	:playerIndex(playerIndex_), rotation(0.0f)
{
  setModel(getGameModel().getPlayerModel(playerIndex));
	assert(GameObject::getModel() != NULL);

	// put the coin in a default position
	setPosition(Zeni::Point3f(BOARD_DIST_X,65.0f,100.0f));
}

void Coin::render()
{
	// debugging
	assert(GameObject::getModel() != NULL);

	// move the model to the coin's current position
	getModel()->set_rotate(GameObject::getOrientation());
	getModel()->set_translate(GameObject::getPosition());
	getModel()->set_scale(COIN_SCALE);

	getModel()->render();
}

int Coin::getPlayerIndex()
{
	return playerIndex;
}

void Coin::rotate(float timeStep)
{
	rotation += timeStep * ROTATE_SCALE;

	GameObject::setOrientation(Zeni::Quaternion::Axis_Angle(Zeni::Vector3f(0.0f,0.0f,1.0f),rotation));
}

void Coin::setMoveablePosition(Zeni::Point3f position_) {
  Point3f newPos = position_;
  if ( newPos.z < 90.0f )
	{
		newPos.z = 90.0f;
  } else if ( newPos.z > 135.0f) {
    newPos.z = 135;
  }
  if (newPos.y < -120) {
    newPos.y = -120;
  } else if (newPos.y > 120) {
    newPos.y = 120;
  }
  GameObject::setPosition(newPos);
}

void Coin::setFinalPosition(Zeni::Point3f finalPosition_)
{
	finalPosition = finalPosition_;
}

Zeni::Point3f Coin::getFinalPosition()
{
	return finalPosition;
}