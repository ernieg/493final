#include "zenilib.h"
#include "Player.h"

using namespace std;
using namespace Zeni;

Player::Player(int playerIndex_)
	:playerIndex(playerIndex_)
{
  switch(playerIndex_) {
    case 0:
      color = "Red";
      break;
    case 1:
      color = "Black";
      break;
  }
}

int Player::getPlayerIndex()
{
	return playerIndex;
}

void Player::perform_logic()
{

}

void Player::handleWiiIREvent(const Wiimote_IR_Event &event){
  cursor.x = event.x;
  cursor.y = event.y;
}

void Player::handleMouseMotion(const SDL_MouseMotionEvent &event) {
  cursor.x = event.x;
  cursor.y = event.y;
}

void Player::handleWiiButtonEvent(const Wiimote_Button_Event &event){
}

void Player::renderHand(){
  get_Video().set_2d();
  string img = "handOpen" + getPlayerColor();// + ".png";
  Zeni::render_image(img, Zeni::Point2f(cursor.x - 40.0f, cursor.y - 40.0f), Zeni::Point2f(cursor.x + 40.0f, cursor.y + 40.0f));

}

string Player::getPlayerColor() {
  return color;
}
