#include <zenilib.h>
#include "GameModel.h"

#include "Board.h"
#include "Coin.h"
#include "Player.h"
#include "AgentBase.h"
#include "EasyAgent.h"
#include "MediumAgent.h"

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
  gameOver = false;
  turn_transition = false;
  difficulty = 0; //default to easy
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

void GameModel::endTurn() {
	  	  // debugging
	  //cout << getGameModel().getBoard()->checkCollide(getGameModel().getCurrentCoin()) << endl;
  	int col = getGameModel().getBoard()->checkCollide(getGameModel().getCurrentCoin());

	  if ( col != -1 )
	  {
		  // put the coin in the board
		  if ( getGameModel().getBoard()->putCoin(getGameModel().getCurrentCoin(),col) )
		  {
			  getGameModel().advanceTurn();

			  // check for win condition (or draw)
			  int winningIndex = getGameModel().getBoard()->checkWin();
			  if ( winningIndex == 0 || winningIndex == 1 || winningIndex == -2 )
			  {
				  getGameModel().setGameOver(true);
				  return;
			  }

			  turn_transition = true;
              
			  // debug
			  //cout << getGameModel().getBoard()->numMovingCoins() << endl;
		  }  
	  }
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

	gameOver = false;

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

void GameModel::setGameOver(bool gameOver_)
{
	gameOver = gameOver_;
}

bool GameModel::getGameOver()
{
	return gameOver;
}