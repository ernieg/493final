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