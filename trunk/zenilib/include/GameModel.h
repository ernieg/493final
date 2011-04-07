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
	void reset();
	void renderTable();
	Zeni::Model* getPlayerModel(int index);
	float getPointerScale();
	void setPointerScale(float pointerScale_);

  void renderAll();

  int getCurrentTurn() {
    return currentTurn;
  }

  void advanceTurn();

private:
	std::vector<Player*> players;
	Board* board;
	Coin* currentCoin; // the coin about to be put on the board
	Zeni::Model* playerZeroModel; // the two coin models
	Zeni::Model* playerOneModel;
	Zeni::Model* tableModel;
	float pointerScale; // scale factor for adjusting the mouse/Wiimote
  void setPlayerColors(std::string play1, std::string play2);
  void makeNewCurrentCoin();

  
  // whose turn is it? 0 for the first player, 1 for the second player
  // note: when the camera is transitioning from the first player's side to the second player's side,
  //       current turn will already be 1 (not 0)
  int currentTurn;
};

GameModel & getGameModel();

#endif