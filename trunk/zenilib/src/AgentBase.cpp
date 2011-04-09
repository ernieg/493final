#include "zenilib.h"

#include "Player.h"
#include "AgentBase.h"
#include "GameModel.h"
#include "Coin.h"
#include "Board.h"
#include "Tutorial_State.h"


AgentBase::AgentBase(int playerIndex_)
	: Player(playerIndex_), locationFound(false), destination(0)
{

}

void AgentBase::perform_logic() {
	if (!locationFound) {
		getNewDestination();
		calculateDirection();
		locationFound = true;
	} else {
		moveChip();
	}
}

void AgentBase::moveChip() {
	GameModel& model = getGameModel();
	Coin* coin = model.getCurrentCoin();
	Board* board = model.getBoard();

	coin->setMoveablePosition(coin->getPosition() + direction);
	if (board->checkCollide(coin) == destination) {
		locationFound = false;
		getGameModel().endTurn();
	}
}

void AgentBase::calculateDirection() {
	GameModel& model = getGameModel();
	Coin* coin = model.getCurrentCoin();
	direction.x = 0.0f;
	direction.y = -1.0f;
	direction.z = 0.0f;
}


