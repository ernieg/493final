#ifndef PLAYER_H
#define PLAYER_H

#include "wiimote_events.h"

class Player
{
public:
	void perform_logic();
	void handleWiiEvent();

  void handleWiiIREvent(const Wiimote_IR_Event &event);
  void handleWiiButtonEvent(const Wiimote_Button_Event &event);

  void renderHand();

	Player(int playerIndex_);

	int getPlayerIndex();

  std::string getPlayerColor();

  void setPlayerColor(std::string newColor) {
    color = newColor;
  }

private:
	int playerIndex; // zero-indexed

  struct Cursor {
    float x;
    float y;
  } cursor;

  std::string color;
};

#endif