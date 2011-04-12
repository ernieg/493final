#include "zenilib.h"

#include <cmath>

#include "Player.h"
#include "GameModel.h"
#include "Coin.h"
#include "Board.h"

using namespace std;
using namespace Zeni;

#define HANDSIZE 25
#define GRABDIST 30

Player::Player(int playerIndex_)
	:playerIndex(playerIndex_), pressed(0), grabbed(0)
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

void Player::perform_logic(float &timeStep)
{

}

void Player::handleWiiEvent() {

}

void Player::handleWiiIREvent(const Wiimote_IR_Event &event){
  cursor.x = event.x;
  cursor.y = event.y;
  moveCurrentCoin();
}

void Player::handleMouseMotion(const SDL_MouseMotionEvent &event) {
  cursor.x = event.x;
  cursor.y = event.y;
  moveCurrentCoin();
}

void Player::moveCurrentCoin() {
  if(!pressed) {
    return;
  }

  GameModel& model = getGameModel();
  Coin* coin = model.getCurrentCoin();
  Projector3D projector(model.getCamera(),
    std::make_pair(Point2i(0,0), get_Video().get_screen_size()));
  Point3f point = projector.project(coin->getPosition());

  float distance = 0.0f;
  if(!grabbed) {
    float xDist = point.x - cursor.x;
    xDist *= xDist; // ^2
    float yDist = point.y - cursor.y;
    yDist *= yDist; //^2
    distance = sqrt(xDist + yDist); 
  }

  if(distance < GRABDIST || grabbed) {
    Point3f newPos = projector.unproject(Point3f(cursor.x, cursor.y, point.z));
    newPos.x = BOARD_DIST_X;
    coin->setMoveablePosition(newPos);
    grabbed = true;
  }

}

void Player::handleMouseButton(const SDL_MouseButtonEvent &event) {
   // 1 == left mouse button
  // 3 == right mouse button

  if ( static_cast<int>(event.button) == 1){
    pressed = event.type == SDL_MOUSEBUTTONDOWN;
    if (event.type == SDL_MOUSEBUTTONUP) {
      grabbed = false;
    }
  }
}

void Player::handleWiiButtonEvent(const Wiimote_Button_Event &event){
    switch(event.button){
      case BUTTON_A:
      case BUTTON_B:
        pressed = event.pressed;
        if(!pressed){
          grabbed = false;
        }
        break;
      case BUTTON_C:
        break;
      case BUTTON_Z:
        break;
      case BUTTON_UP:
          break;
      case BUTTON_DOWN:
          break;
      default:
          break;
        
    }
}

void Player::renderHand(){
  get_Video().set_2d();
  string handPos = pressed ? "Grab" : "Open";
  string img = "hand" + handPos + getPlayerColor();// + ".png";
  Zeni::render_image(img, Zeni::Point2f(cursor.x - HANDSIZE, cursor.y - HANDSIZE),
    Zeni::Point2f(cursor.x + HANDSIZE, cursor.y + HANDSIZE));
  get_Video().set_3d(getGameModel().getCamera());

}

string Player::getPlayerColor() {
  return color;
}
