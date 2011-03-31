#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <zenilib.h>
#include "Board.h"
#include "Player.h"

class GameModel
{
public:
	Player* getPlayer(int playerIndex);
	Board* getBoard();
	GameModel* getGameModel();
	void reset();

private:
	std::vector<Player*> players;
	Board board;
}

#endif