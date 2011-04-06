#include <zenilib.h>
#include "GameModel.h"

#include "Board.h"
#include "Coin.h"

using namespace std;
using namespace Zeni;

GameModel & getGameModel() {
  static GameModel model;
  return model;
}

GameModel::GameModel() {
	board = new Board();

	playerZeroModel = new Model("models/redChip.3DS");
	playerOneModel = new Model("models/greenChip.3DS");

	tableModel = new Model("models/table.3DS");
	tableModel->set_scale(TABLE_SCALE); // I just fudged these numbers. They might need tweaking
	tableModel->set_translate(Point3f(BOARD_DIST_X,0.0f,-3.0f*TABLE_SCALE.z));

	currentCoin = NULL;
	pointerScale = 50.0f;
}

Board* GameModel::getBoard()
{
	return board;
}

Coin* GameModel::getCurrentCoin()
{
	return currentCoin;
}

void GameModel::makeNewCurrentCoin(int playerIndex)
{
	currentCoin = new Coin(playerIndex);
}

Model* GameModel::getPlayerZeroModel()
{
	return playerZeroModel;
}

Model* GameModel::getPlayerOneModel()
{
	return playerOneModel;
}

void GameModel::renderTable()
{
	tableModel->render();
}

void GameModel::reset()
{
	board->reset();

	if ( currentCoin != NULL )
	{
		delete currentCoin;
	}
}

float GameModel::getPointerScale()
{
	return pointerScale;
}

void GameModel::setPointerScale(float pointerScale_)
{
	pointerScale = pointerScale_;
}