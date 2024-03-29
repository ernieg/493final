#include "zenilib.h"

#include "Player.h"
#include "AgentBase.h"
#include "GameModel.h"
#include "Coin.h"
#include "Board.h"
#include "Tutorial_State.h"

#define DELAY 1.0f

AgentBase::AgentBase(int playerIndex_)
	: Player(playerIndex_), locationFound(false), destination(0), moveDelay(DELAY)
{

}

void AgentBase::perform_logic(float &timeStep) {
	if (!locationFound) {
		getNewDestination();
		locationFound = true;
	} else {
		moveChip(timeStep);
	}
}

void AgentBase::moveChip(float &timeStep) {
	moveDelay -= timeStep;
	if (moveDelay >= 0.0f) {
		return;
	}
	
	GameModel& model = getGameModel();
	Coin* coin = model.getCurrentCoin();
	Board* board = model.getBoard();

	coin->setMoveablePosition(coin->getPosition() + DIRECTION * timeStep);
	if (board->checkCollide(coin) == destination) {
		locationFound = false;
		moveDelay = DELAY;
		getGameModel().endTurn();
	}
}



