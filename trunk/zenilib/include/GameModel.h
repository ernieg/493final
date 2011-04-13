#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <zenilib.h>
class Board;
class Coin;
class Player;

#define NUMCOINS (6) // change this to 6 when the purple chip gets uploaded

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
  void setPlayerColors(std::string play1, std::string play2);

  void renderAll();

  int getCurrentTurn() {
    return currentTurn;
  }

  bool isTurning() { return turn_transition; }
  void setTurning(bool turning) { turn_transition = turning; }

  void advanceTurn();
  void endTurn();

  Zeni::Camera& getCamera() {
    return camera;
  }
  
  int getDifficulty() const {
    return difficulty;
  }
  
  void setDifficulty(int diff_) {
    difficulty = diff_;
  }
  
  std::vector<Player*>* getPlayersContainer() {
    return &players;
  }

  void setGameOver(bool gameOver_);
  bool getGameOver();

  Zeni::Chronometer<Zeni::Time> & getEndGameTimer()
  {
	  return endGameTimer;
  }

private:
	std::vector<Player*> players;
	Board* board;
	Coin* currentCoin; // the coin about to be put on the board
	Zeni::Model* playerZeroModel; // the two coin models
	Zeni::Model* playerOneModel;
	Zeni::Model* tableModel;
	float pointerScale; // scale factor for adjusting the mouse/Wiimote
  void makeNewCurrentCoin();
  bool turn_transition;
  int difficulty; //0 - easy, 1 - medium, 2 - hard, etc..., defaults to easy

  
  // whose turn is it? 0 for the first player, 1 for the second player
  // note: when the camera is transitioning from the first player's side to the second player's side,
  //       current turn will already be 1 (not 0)
  int currentTurn;

  Zeni::Camera camera;

  // the game is over, but we may still be waiting for coins to fall
  bool gameOver;

  Zeni::Chronometer<Zeni::Time> endGameTimer;
};

GameModel & getGameModel();

#endif