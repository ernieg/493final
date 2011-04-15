/* This file is part of the Zenipex Library.
* Copyleft (C) 2008 Mitchell Keith Bloch a.k.a. bazald
*
* This source file is simply under the public domain.
*/

#include "zenilib.h"

#include "Tutorial_State.h"

#include <Zeni/Quadrilateral.hxx>
#include <Zeni/Timer.hxx>
#include <Zeni/Vertex2f.h>

#include "GameModel.h"
#include "Board.h"
#include "Coin.h"
#include "Player.h"

using namespace std;
using namespace Zeni;

#define MOUSE_SPEED_ADJUST 70

class Postgame_State : public MenuState {

public:
	Postgame_State(int winningPlayerIndex_)
		:winningPlayerIndex(winningPlayerIndex_)
	{
    Rematch *rematch = new Rematch();
    PlayerSelect *playerSelect = new PlayerSelect();
    MainMenu *mainMenu = new MainMenu();
    
    buttons.push_back(rematch);
    buttons.push_back(playerSelect);
    buttons.push_back(mainMenu);
    
    buttons[selectedButton]->select();
	}
  
  class Rematch : public MenuButton {
    Rematch(const Rematch &);
    Rematch & operator=(const Rematch &);
    
  public:
    Rematch()
    : MenuButton("Rematch", Zeni::Point2f(400.0f, 300.0f))
    {
    }
    
    void onAccept() {
      get_Game().pop_state();
      get_Game().push_state(new Tutorial_State());
    }
  };
  
  class PlayerSelect : public MenuButton {
    PlayerSelect(const PlayerSelect &);
    PlayerSelect & operator=(const PlayerSelect &);
    
  public:
    PlayerSelect()
    : MenuButton("Player Select", Zeni::Point2f(400.0f, 350.0f))
    {
    }
    
    void onAccept() {
      get_Game().pop_state();
    }
  };
  
  class MainMenu : public MenuButton {
    MainMenu(const MainMenu &);
    MainMenu & operator=(const MainMenu &);
    
  public:
    MainMenu()
    : MenuButton("Main Menu", Zeni::Point2f(400.0f, 400.0f))
    {
    }
    
    void onAccept() {
      get_Game().pop_state();
      get_Game().pop_state();
    }
  };

private:
	int winningPlayerIndex; // 0 or 1, or -2 if a draw

	void on_wiimote_button(const Wiimote_Button_Event &event) {
    switch(event.button) {
      case BUTTON_A:
        if(event.pressed)
          buttons[selectedButton]->onAccept();
        break;
        
      case BUTTON_UP:
        if(event.pressed) {
          if(selectedButton > 0) {
            buttons[selectedButton]->deselect();
            selectedButton--;
            buttons[selectedButton]->select();
          }
        }
        break;
        
      case BUTTON_DOWN:
        if(event.pressed) {
          if(selectedButton < buttons.size()-1) {
            buttons[selectedButton]->deselect();
            selectedButton++;
            buttons[selectedButton]->select();
          }
        }
        break;
    }
  }
  
  void on_key(const SDL_KeyboardEvent &event) {
    Wiimote_Button_Event fakeEvent;
    switch(event.keysym.sym) {
      case SDLK_UP:
        if(event.state == SDL_PRESSED) {
          fakeEvent.button = BUTTON_UP;
        }
        break;
        
      case SDLK_DOWN:
        if(event.state == SDL_PRESSED){
          fakeEvent.button = BUTTON_DOWN;
        }
        break;
        
      case SDLK_SPACE:
      case SDLK_RETURN:
        if(event.state == SDL_PRESSED) {
          fakeEvent.button = BUTTON_A;
        }
        break;
    }
    
    fakeEvent.pressed = event.type == SDL_KEYDOWN;
    fakeEvent.wiimote = KEYBOARD_CONTROL;
    on_wiimote_button(fakeEvent);
  }

	void render()
	{
    get_Video().set_2d();
		MenuState::render();
    
		Zeni::Font &title = get_Fonts()["system80"];
    
		string message;

		if ( winningPlayerIndex == 0 )
		{
			message = "Player One Wins!";
		}
		else if ( winningPlayerIndex == 1 )
		{
			message = "Player Two Wins!";
		}
		else
		{
			message = "Draw!";
		}

		title.render_text(message,
					   Point2f(400.0f, 50.0f - 0.5f * title.get_text_height()),
					   OFFWHITE,
					   ZENI_CENTER);
    
		/*Zeni::Font &fr = get_Fonts()["system20"];
    
		fr.render_text("HOW TO PLAY:\n\n"
					   "The first player to line up four chips vertically, \nhorizontally, or diagonally wins the game.\n\n"
					   "CONTROLS:\n\n"
					   "Move the Wiimote until your chip is in the correct \ncolumn, then press the B button to drop it.\n",
					   Point2f(400.0f, 250.0f - 0.5f * fr.get_text_height()),
					   OFFWHITE,
					   ZENI_CENTER);*/
    }
};


Tutorial_State::Tutorial_State()
: m_time_passed(0.0f),
m_max_time_step(1.0f / 20.0f), // make the largest physics step 1/20 of a second
m_max_time_steps(10.0f), // allow no more than 10 physics steps per frame
transition_angle(0.0f), 
camera(getGameModel().getCamera())
{
  set_pausable(true);
  
  getGameModel().reset();
 // camera = getGameModel().getCamera();
  camera.position.x = 0.0f; // make sure the camera is always at the first player's position when the game starts
  camera.position.y = 0.0f;
  camera.position.z = 60.0f;
  camera.look_at(BOARD_CENTER_MIDDLE);
}

Tutorial_State::~Tutorial_State() {
}

void Tutorial_State::on_push() {
  SDL_ShowCursor(0);
  SDL_WM_GrabInput(SDL_GRAB_ON);
}

void Tutorial_State::on_pop() {
  SDL_WM_GrabInput(SDL_GRAB_OFF);
  SDL_ShowCursor(1);
}

void Tutorial_State::on_mouse_button(const SDL_MouseButtonEvent &event) {
  
  // 1 == left mouse button
  // 3 == right mouse button
  getGameModel().getPlayer(getGameModel().getCurrentTurn())->handleMouseButton(event);
  if ( static_cast<int>(event.button) == 1 && event.type == SDL_MOUSEBUTTONUP && !getGameModel().isTurning() )
  {
    endTurn();
  }
};

void Tutorial_State::endTurn() {
    getGameModel().endTurn();
}

void Tutorial_State::on_key(const SDL_KeyboardEvent &event) {
	switch(event.keysym.sym) {
    case SDLK_ESCAPE:
      get_Game().push_state(new PauseState());
      break;
		case SDLK_w:
			break;

		case SDLK_a:
			break;

    case SDLK_s:
			break;

		case SDLK_d:
			break;

		case SDLK_e:
			// debugging: (for testing out the board-turning animation)
			if ( event.type == SDL_KEYDOWN )
			{
			  getGameModel().advanceTurn();
			  getGameModel().setTurning(true);
			}
			break;

		default:
			Gamestate_Base::on_key(event);
	}
}

void Tutorial_State::on_mouse_motion(const SDL_MouseMotionEvent &event) {
  getGameModel().getPlayer(getGameModel().getCurrentTurn())->handleMouseMotion(event);
}

void Tutorial_State::handleCursorEvents(float yMovement, float zMovement){
 // yMovement = yMovement/MOUSE_SPEED_ADJUST;
 // zMovement = zMovement/MOUSE_SPEED_ADJUST;
 // yMovement *= (getGameModel().getCurrentTurn() == 0 ) ? -1.0f : 1.0f;

  // Restore the backup vectors if flipped over
  // (if the up vector is pointing down)
  //
  // Note that we want to be sure not to freeze a player 
  // who is already flipped (for whatever reason).
   
	//if(camera.get_up().k < 0.0f && prev_up.k >= 0.0f)
  //  camera.orientation = prev_orientation;

	//float yMovement = yMovement / getGameModel().getPointerScale();
	//float zMovement = zMovement / getGameModel().getPointerScale();

  yMovement *= (getGameModel().getCurrentTurn() == 0 ) ? -1.0f : 1.0f;

	Point3f oldPos(getGameModel().getCurrentCoin()->getPosition());

	Point3f newPos = oldPos + Point3f(0.0f, yMovement, zMovement);

	// check for collisions between the board and the coin here
	if ( newPos.z < 90.0f )
	{
		newPos.z = 90.0f;
  } else if ( newPos.z > 135.0f) {
    newPos.z = 135;
  }
  if (newPos.y < -120) {
    newPos.y = -120;
  } else if (newPos.y > 120) {
    newPos.y = 120;
  }

	getGameModel().getCurrentCoin()->setPosition(newPos);

}

void Tutorial_State::perform_logic() {
#ifndef _MACOSX
        Wiimote_Game_State::perform_logic();
#else
        Gamestate_Base::perform_logic();
#endif
  {// Update time_passed
    const Time temp_time = get_Timer().get_time();
    m_time_passed += temp_time.get_seconds_since(m_current_time);
    m_current_time = temp_time;
  }

  //cout << getGameModel().getEndGameTimer().seconds() << endl;

    // if someone has won and all the coins have finished falling
  if ( getGameModel().getGameOver() && (getGameModel().getBoard()->numMovingCoins() == 0) && (getGameModel().getEndGameTimer().seconds() >= END_GAME_DELAY) )
  //if ( getGameModel().getGameOver() && (getGameModel().getEndGameTimer().seconds() >= END_GAME_DELAY) )
    {
		get_Game().pop_state();
		get_Game().push_state(new Postgame_State(getGameModel().getBoard()->checkWin()));
	}

  /* Shrink time passed to an upper bound
   *
   * If your program is ever paused by your computer for 10 
   * minutes and then allowed to continue running, you don't want 
   * it to pause at the physics loop for another 10 minutes.
   */
  if(m_time_passed / m_max_time_step > m_max_time_steps)
    m_time_passed = m_max_time_steps * m_max_time_step;

  /* Constant time-step physics update
   *
   * This type of update increases the stability of your program 
   * in scenarios where the time step could allow things to move 
   * further than you expect, potentially jumping though walls or 
   * falling through floors... that sort of thing.  Keep the 
   * max_time_step small and you are safe.
   */
  while(m_time_passed > m_max_time_step) {
    m_time_passed -= m_max_time_step;
    step(m_max_time_step);
  }

  // move the camera from one side of the board to the other
  if ( getGameModel().isTurning() )
  {
	transition_angle += m_time_passed * (-0.5f * cos(2.0f * transition_angle) + 0.6f) * 8.0f;

	camera.position.x = -BOARD_DIST_X*cos(transition_angle) + BOARD_DIST_X;
	camera.position.y = -BOARD_DIST_X*sin(transition_angle);

	camera.look_at(BOARD_CENTER_MIDDLE);

	// it is the first player's turn, so stop at angle == 0 (or 2pi)
  if ( getGameModel().getCurrentTurn() == 0 )
	{
	  if ( 2.0f*pi - transition_angle < ANGLE_EPSILON )
	  {
	    transition_angle = 0.0f;
		getGameModel().setTurning(false);

		camera.position.x = 0.0f;
		camera.position.y = 0.0f;
		camera.look_at(BOARD_CENTER_MIDDLE);
	  }
	}
	else // it is the second player's turn, so stop at angle pi
	{
	  if ( pi - transition_angle < ANGLE_EPSILON )
	  {
		transition_angle = pi;
		getGameModel().setTurning(false);

		camera.position.x = 200.0f;
		camera.position.y = 0.0f;
		camera.look_at(BOARD_CENTER_MIDDLE);
	  }
	}
  } else {
	  GameModel& model = getGameModel();
	  Player *player = model.getPlayer(getGameModel().getCurrentTurn());
	  player->perform_logic(m_time_passed);
  }



  /* Simple physics update
   *
   * This type of update could be the only update you do if you 
   * don't have concerns with respect to jumping though walls, 
   * etc...
   *
   * Otherwise, if you have the other update, this serves to 
   * improve the smoothness of your animations.  With the other 
   * version alone, things can jerk a bit from frame to frame.
   *
   * To really see the improvement, turn up multisampling and try 
   * it with this section (A) commented out, and then (B) not
   * commented out.
   */
  {
    step(m_time_passed);
    m_time_passed = 0.0f;
  }
}

void Tutorial_State::step(const float &time_step) {
    // make the current coin rotate
	getGameModel().getCurrentCoin()->rotate(time_step);

	// advance any coins falling down
	getGameModel().getBoard()->advanceMovingCoins(time_step);

	// rotate the winning coins (if there are any)
	for (int i = 0; i < 4; i++)
	{
		if ( getGameModel().getBoard()->getWinningCoins()[i] != NULL )
		{
			getGameModel().getBoard()->getWinningCoins()[i]->rotate(time_step);
		}
	}
}

void Tutorial_State::render() {
  Video &vr = get_Video();

  camera.far_clip = 2000.0f;
  vr.set_3d(camera);

	//vr.render(quad);
  
  float maxX = 300.0f;
  float minX = -100.0f;
  float maxY = 200.0f;
  float minY = -200.0f;
  float maxZ = 150.0f;
  float minZ = -50.0f;
  
  Material brick("Brick");
  Material floorTile("Floortile");
  Material ceiling("Ceiling");
  Material door("Door");
  Material um("UM");
  Material osu("OSU");
  Material oly("Olympics");
  
  Vertex3f_Texture back1(Point3f(minX, maxY, maxZ), Point2f(0.0f, 0.0f));
	Vertex3f_Texture back2(Point3f(minX, maxY, minZ), Point2f(0.0f, 1.0f));
	Vertex3f_Texture back3(Point3f(minX, minY, minZ), Point2f((maxY-minY)/50.0f, 1.0f));
	Vertex3f_Texture back4(Point3f(minX, minY, maxZ), Point2f((maxY-minY)/50.0f, 0.0f));
  
  Vertex3f_Texture front1(Point3f(maxX, maxY, maxZ), Point2f(0.0f, 0.0f));
	Vertex3f_Texture front2(Point3f(maxX, maxY, minZ), Point2f(0.0f, 1.0f));
	Vertex3f_Texture front3(Point3f(maxX, minY, minZ), Point2f((maxY-minY)/50.0f, 1.0f));
	Vertex3f_Texture front4(Point3f(maxX, minY, maxZ), Point2f((maxY-minY)/50.0f, 0.0f));
  
  Vertex3f_Texture left1(Point3f(minX, maxY, maxZ), Point2f(0.0f, 0.0f));
	Vertex3f_Texture left2(Point3f(minX, maxY, minZ), Point2f(0.0f, 1.0f));
	Vertex3f_Texture left3(Point3f(maxX, maxY, minZ), Point2f((maxX-minX)/50.0f, 1.0f));
	Vertex3f_Texture left4(Point3f(maxX, maxY, maxZ), Point2f((maxX-minX)/50.0f, 0.0f));
  
  Vertex3f_Texture right1(Point3f(minX, minY, maxZ), Point2f(0.0f, 0.0f));
	Vertex3f_Texture right2(Point3f(minX, minY, minZ), Point2f(0.0f, 1.0f));
	Vertex3f_Texture right3(Point3f(maxX, minY, minZ), Point2f((maxX-minX)/50.0f, 1.0f));
	Vertex3f_Texture right4(Point3f(maxX, minY, maxZ), Point2f((maxX-minX)/50.0f, 0.0f));
  
  Vertex3f_Texture floor1(Point3f(maxX, maxY, minZ), Point2f(0.0f, 0.0f));
	Vertex3f_Texture floor2(Point3f(minX, maxY, minZ), Point2f(0.0f, (maxX-minX)/30.0f));
	Vertex3f_Texture floor3(Point3f(minX, minY, minZ), Point2f((maxY-minY)/30.0f, (maxX-minX)/30.0f));
	Vertex3f_Texture floor4(Point3f(maxX, minY, minZ), Point2f((maxY-minY)/30.0f, 0.0f));
  
  Vertex3f_Texture ceiling1(Point3f(maxX, maxY, maxZ), Point2f(0.0f, 0.0f));
	Vertex3f_Texture ceiling2(Point3f(minX, maxY, maxZ), Point2f(0.0f, (maxX-minX)/20.0f));
	Vertex3f_Texture ceiling3(Point3f(minX, minY, maxZ), Point2f((maxY-minY)/50.0f, (maxX-minX)/20.0f));
	Vertex3f_Texture ceiling4(Point3f(maxX, minY, maxZ), Point2f((maxY-minY)/50.0f, 0.0f));
  
  Vertex3f_Texture door1(Point3f((maxX+minX)/2.0f - 50.0f, minY+.1f, minZ + 160.0f), Point2f(0.0f, 0.0f));
	Vertex3f_Texture door2(Point3f((maxX+minX)/2.0f - 50.0f, minY+.1f, minZ), Point2f(0.0f, 1.0f));
	Vertex3f_Texture door3(Point3f((maxX+minX)/2.0f + 50.0f, minY+.1f, minZ), Point2f(1.0f, 1.0f));
	Vertex3f_Texture door4(Point3f((maxX+minX)/2.0f + 50.0f, minY+.1f, minZ + 160.0f), Point2f(1.0f, 0.0f));
  
  Vertex3f_Texture osu1(Point3f(maxX - 0.1f, 100.0f, maxZ - 20.0f), Point2f(0.0f, 0.0f));
	Vertex3f_Texture osu2(Point3f(maxX - 0.1f, 100.0f, maxZ - 120.0f), Point2f(0.0f, 1.0f));
	Vertex3f_Texture osu3(Point3f(maxX - 0.1f, -100.0f, maxZ - 120.0f), Point2f(1.0f, 1.0f));
	Vertex3f_Texture osu4(Point3f(maxX - 0.1f, -100.0f, maxZ - 20.0f), Point2f(1.0f, 0.0f));
  
  Vertex3f_Texture um1(Point3f(minX + 0.1f, -100.0f, maxZ - 20.0f), Point2f(0.0f, 0.0f));
	Vertex3f_Texture um2(Point3f(minX + 0.1f, -100.0f, maxZ - 120.0f), Point2f(0.0f, 1.0f));
	Vertex3f_Texture um3(Point3f(minX + 0.1f, 100.0f, maxZ - 120.0f), Point2f(1.0f, 1.0f));
	Vertex3f_Texture um4(Point3f(minX + 0.1f, 100.0f, maxZ - 20.0f), Point2f(1.0f, 0.0f));
  
  Vertex3f_Texture oly1(Point3f((maxX+minX)/2.0f - 100.0f, maxY-.1f, maxZ - 20.0f), Point2f(0.0f, 0.0f));
	Vertex3f_Texture oly2(Point3f((maxX+minX)/2.0f - 100.0f, maxY-.1f, maxZ - 150.0f), Point2f(0.0f, 1.0f));
	Vertex3f_Texture oly3(Point3f((maxX+minX)/2.0f + 100.0f, maxY-.1f, maxZ - 150.0f), Point2f(1.0f, 1.0f));
	Vertex3f_Texture oly4(Point3f((maxX+minX)/2.0f + 100.0f, maxY-.1f, maxZ - 20.0f), Point2f(1.0f, 0.0f));
  
  Quadrilateral<Vertex3f_Texture> brickQuadBack(back1, back2, back3, back4);
  Quadrilateral<Vertex3f_Texture> brickQuadFront(front1, front2, front3, front4);
  Quadrilateral<Vertex3f_Texture> brickQuadLeft(left1, left2, left3, left4);
  Quadrilateral<Vertex3f_Texture> brickQuadRight(right1, right2, right3, right4);
  Quadrilateral<Vertex3f_Texture> floorQuad(floor1, floor2, floor3, floor4);
  Quadrilateral<Vertex3f_Texture> ceilingQuad(ceiling1, ceiling2, ceiling3, ceiling4);
  Quadrilateral<Vertex3f_Texture> doorQuad(door1, door2, door3, door4);
  Quadrilateral<Vertex3f_Texture> umQuad(um1, um2, um3, um4);
  Quadrilateral<Vertex3f_Texture> osuQuad(osu1, osu2, osu3, osu4);
  Quadrilateral<Vertex3f_Texture> olyQuad(oly1, oly2, oly3, oly4);
  
  brickQuadBack.fax_Material(&brick);
  brickQuadFront.fax_Material(&brick);
  brickQuadLeft.fax_Material(&brick);
  brickQuadRight.fax_Material(&brick);
  floorQuad.fax_Material(&floorTile);
  ceilingQuad.fax_Material(&ceiling);
  doorQuad.fax_Material(&door);
  umQuad.fax_Material(&um);
  osuQuad.fax_Material(&osu);
  olyQuad.fax_Material(&oly);
  
  vr.render(brickQuadBack);
  vr.render(brickQuadFront);
  vr.render(brickQuadLeft);
  vr.render(brickQuadRight);
  vr.render(floorQuad);
  vr.render(ceilingQuad);
  vr.render(doorQuad);
  vr.render(umQuad);
  vr.render(osuQuad);
  vr.render(olyQuad);

	// skybox
//	Vector3f v_f(1.0f, 0.0f, 0.0f);
//    Vector3f v_u(0.0f, 0.0f, 1.0f);
//    Vector3f v_l(0.0f, 1.0f, 0.0f);
//
//	Point2f texture_a(0.0f, 0.0f);
//	Point2f texture_b(0.0f, 1.0f);
//	Point2f texture_c(1.0f, 1.0f);
//	Point2f texture_d(1.0f, 0.0f);
//
//	Point3f skybox_p0 = camera.position + SKYBOX_DIST*(v_f + v_u + v_l);
//	Point3f skybox_p1 = camera.position + SKYBOX_DIST*(v_f - v_u + v_l);
//	Point3f skybox_p2 = camera.position + SKYBOX_DIST*(v_f - v_u - v_l);
//	Point3f skybox_p3 = camera.position + SKYBOX_DIST*(v_f + v_u - v_l);
//	Point3f skybox_p4 = camera.position + SKYBOX_DIST*(-v_f - v_u + v_l);
//	Point3f skybox_p5 = camera.position + SKYBOX_DIST*(-v_f + v_u + v_l);
//	Point3f skybox_p6 = camera.position + SKYBOX_DIST*(-v_f - v_u - v_l);
//	Point3f skybox_p7 = camera.position + SKYBOX_DIST*(-v_f + v_u - v_l);
//
//	Vertex3f_Texture a_p0(skybox_p0, texture_a);
//    Vertex3f_Texture a_p1(skybox_p1, texture_b);
//    Vertex3f_Texture a_p2(skybox_p2, texture_c); 
//    Vertex3f_Texture a_p3(skybox_p3, texture_d);
//
//	Vertex3f_Texture b_p5(skybox_p5, texture_a);
//	Vertex3f_Texture b_p4(skybox_p4, texture_b);
//	Vertex3f_Texture b_p1(skybox_p1, texture_c);
//	Vertex3f_Texture b_p0(skybox_p0, texture_d);
//
//	Vertex3f_Texture c_p7(skybox_p7, texture_a);
//	Vertex3f_Texture c_p6(skybox_p6, texture_b);
//	Vertex3f_Texture c_p4(skybox_p4, texture_c);
//	Vertex3f_Texture c_p5(skybox_p5, texture_d);
//	
//	Vertex3f_Texture d_p3(skybox_p3, texture_a);
//	Vertex3f_Texture d_p2(skybox_p2, texture_b);
//	Vertex3f_Texture d_p6(skybox_p6, texture_c);
//	Vertex3f_Texture d_p7(skybox_p7, texture_d);
//
//	Vertex3f_Texture e_p0(skybox_p0, texture_a);
//	Vertex3f_Texture e_p3(skybox_p3, texture_b);
//	Vertex3f_Texture e_p7(skybox_p7, texture_c);
//	Vertex3f_Texture e_p5(skybox_p5, texture_d);
//
//	Vertex3f_Texture f_p1(skybox_p1, texture_a);
//	Vertex3f_Texture f_p4(skybox_p4, texture_b);
//	Vertex3f_Texture f_p6(skybox_p6, texture_c);
//	Vertex3f_Texture f_p2(skybox_p2, texture_d);
//	
//    Material skybox_material("skybox");
//
//	Quadrilateral<Vertex3f_Texture> quad_a(a_p0, a_p1, a_p2, a_p3);
//	Quadrilateral<Vertex3f_Texture> quad_b(b_p5, b_p4, b_p1, b_p0);
//	Quadrilateral<Vertex3f_Texture> quad_c(c_p7, c_p6, c_p4, c_p5);
//	Quadrilateral<Vertex3f_Texture> quad_d(d_p3, d_p2, d_p6, d_p7);
//	Quadrilateral<Vertex3f_Texture> quad_e(e_p0, e_p3, e_p7, e_p5);
//	Quadrilateral<Vertex3f_Texture> quad_f(f_p1, f_p4, f_p6, f_p2);
//    quad_a.fax_Material(&skybox_material);
//	quad_b.fax_Material(&skybox_material);
//	quad_c.fax_Material(&skybox_material);
//	quad_d.fax_Material(&skybox_material);
//	quad_e.fax_Material(&skybox_material);
//	quad_f.fax_Material(&skybox_material);
//
//	vr.render(quad_a);
//	vr.render(quad_b);
//	vr.render(quad_c);
//	vr.render(quad_d);
//	vr.render(quad_e);
//	vr.render(quad_f);

	// render the table
	getGameModel().renderAll();
  
  //render HUD
  vr.set_2d();
  Zeni::Font &text = Zeni::get_Fonts()["system20"];
  
  text.render_text("Player 1",
                    Zeni::Point2f(0.0f, 0.0f),
                    Zeni::OFFWHITE,
                    Zeni::ZENI_LEFT);
  text.render_text("Player 2",
                    Zeni::Point2f(800.0f, 0.0f),
                    Zeni::OFFWHITE,
                    Zeni::ZENI_RIGHT);
  
  string p1Coin = getGameModel().getPlayer(0)->getPlayerColor() + "CHIP";
  string p2Coin = getGameModel().getPlayer(1)->getPlayerColor() + "CHIP";
  
  for(int i = 0; i < p1Coin.size(); i++)
    p1Coin[i] = toupper(p1Coin[i]);
  for(int i = 0; i < p2Coin.size(); i++)
    p2Coin[i] = toupper(p2Coin[i]);
  
  p1Coin = p1Coin.substr(0, 8) + ".PNG";
  p2Coin = p2Coin.substr(0, 8) + ".PNG";
  
  render_image(p1Coin, Point2f(85.0f, 5.0f), Point2f(105.0f, 25.0f));
  render_image(p2Coin, Point2f(695.0f, 5.0f), Point2f(715.0f, 25.0f));
}


#ifndef _MACOSX
void Tutorial_State::on_wiimote_button(const Wiimote_Button_Event &event){
   getGameModel().getPlayer(getGameModel().getCurrentTurn())->handleWiiButtonEvent(event);
   switch(event.button){
     case BUTTON_HOME:
       get_Game().push_state(new PauseState());
       break;
      case BUTTON_A:
      case BUTTON_B:
        if (!event.pressed) {
          endTurn();
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

void Tutorial_State::on_wiimote_ir(const Wiimote_IR_Event &event){
  getGameModel().getPlayer(getGameModel().getCurrentTurn())->handleWiiIREvent(event);
}

void Tutorial_State::on_wiimote_nunchuk(const Wiimote_Nunchuk_Event &event){
}
#endif
