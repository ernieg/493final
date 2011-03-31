#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <zenilib.h>
#include "Board.h"
class Player;

class GameModel {
    // Get reference to only instance;
    friend GameModel & getGameModel();
    GameModel();

    // undefined
    GameModel(const GameModel &);
    GameModel & operator=(const GameModel &);
public:
	Player* getPlayer(int playerIndex);
	Board* getBoard();
	void reset();

private:
	std::vector<Player*> players;
	Board board;
};

GameModel & getGameModel();

#endif