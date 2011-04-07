#include <zenilib.h>
#include "GameModel.h"

#include "Board.h"
#include "Coin.h"
#include "Player.h"

using namespace std;
using namespace Zeni;

GameModel & getGameModel() {
  static GameModel model;
  return model;
}

GameModel::GameModel() {
	board = new Board();

	tableModel = new Model("models/table.3DS");
	tableModel->set_scale(TABLE_SCALE); // I just fudged these numbers. They might need tweaking
	tableModel->set_translate(Point3f(BOARD_DIST_X,0.0f,-3.0f*TABLE_SCALE.z));

	currentCoin = NULL;
	pointerScale = 50.0f;

  Player* play1 = new Player(0);
  Player* play2 = new Player(1);
  players.push_back(play1);
  players.push_back(play2);
  setPlayerColors("Green", "Red");

  currentTurn = 0;
}

void GameModel::setPlayerColors(string play1, string play2) {
  playerZeroModel = new Model("models/"+play1+"Coin.3DS");
	playerOneModel = new Model("models/"+play2+"Coin.3DS");
  players[0]->setPlayerColor(play1);
  players[1]->setPlayerColor(play2);
}

Board* GameModel::getBoard()
{
	return board;
}

Coin* GameModel::getCurrentCoin()
{
	return currentCoin;
}

void GameModel::makeNewCurrentCoin()
{
	currentCoin = new Coin(currentTurn);
}

void GameModel::advanceTurn() {
  currentTurn = (currentTurn + 1) % 2;
	getGameModel().makeNewCurrentCoin();
}

Model* GameModel::getPlayerModel(int index){
	return index == 0 ? playerZeroModel : playerOneModel;
}

Player* GameModel::getPlayer(int index) {
  return players.at(index);
}

void GameModel::renderAll() {
  renderTable();

	// render the board (and all the coins in it)
	getBoard()->render();

	// render the current coin (the one being moved around by the player)
	getCurrentCoin()->render();

  players[currentTurn]->renderHand();
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
  currentTurn = 0;
  makeNewCurrentCoin();
}

float GameModel::getPointerScale()
{
	return pointerScale;
}

void GameModel::setPointerScale(float pointerScale_)
{
	pointerScale = pointerScale_;
}