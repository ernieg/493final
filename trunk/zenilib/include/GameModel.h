#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <zenilib.h>
class Board;
class Coin;
class Player;

const Zeni::Vector3f TABLE_SCALE(7.0f,7.0f,7.0f);

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
	void renderTable();
	Zeni::Model* getPlayerZeroModel();
	Zeni::Model* getPlayerOneModel();
	float getPointerScale();
	void setPointerScale(float pointerScale_);

private:
	std::vector<Player*> players;
	Board* board;
	Coin* currentCoin; // the coin about to be put on the board
	Zeni::Model* playerZeroModel; // the two coin models
	Zeni::Model* playerOneModel;
	Zeni::Model* tableModel;
	float pointerScale; // scale factor for adjusting the mouse/Wiimote
};

GameModel & getGameModel();

#endif