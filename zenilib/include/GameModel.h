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
	Coin* getCurrentCoin();
	void makeNewCurrentCoin(int playerIndex);
	void reset();
	Zeni::Model* getPlayerZeroModel();
	Zeni::Model* getPlayerOneModel();

private:
	std::vector<Player*> players;
	Board* board;
	Coin* currentCoin; // the coin about to be put on the board
	Zeni::Model* playerZeroModel; // the two coin models
	Zeni::Model* playerOneModel;
};

GameModel & getGameModel();

#endif