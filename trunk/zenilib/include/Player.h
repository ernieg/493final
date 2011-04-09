#ifndef PLAYER_H
#define PLAYER_H

#include "wiimote_events.h"

class Player
{
public:
	virtual void perform_logic();
	virtual void handleWiiEvent();

  void handleMouseMotion(const SDL_MouseMotionEvent &event);
  void handleMouseButton(const SDL_MouseButtonEvent &event);
  void handleWiiIREvent(const Wiimote_IR_Event &event);
  void handleWiiButtonEvent(const Wiimote_Button_Event &event);

  virtual void renderHand();

	Player(int playerIndex_);

	int getPlayerIndex();

  std::string getPlayerColor();

  void setPlayerColor(std::string newColor) {
    color = newColor;
  }

protected:
	int playerIndex; // zero-indexed

  struct Cursor {
    float x;
    float y;
  } cursor;

  bool pressed;
  bool grabbed;

  std::string color;

  void moveCurrentCoin();
};

#endif