/* This file is part of the Zenipex Library.
* Copyleft (C) 2008 Mitchell Keith Bloch a.k.a. bazald
*
* This source file is simply under the public domain.
*/

#ifndef TUTORIAL_STATE_H
#define TUTORIAL_STATE_H

#include "wiimote_state.h"
#include <Zeni/Camera.h>
#include <Zeni/Game.h>
#include <Zeni/Timer.h>
#include <Zeni/Video.h>
#include <Zeni/Vector3f.h>
#include "GameModel.h"

// distance from the camera to the skybox
const float SKYBOX_DIST = 1000.0f;
const float ANGLE_EPSILON = .01f;

#ifdef _MACOSX
class Tutorial_State : public Zeni::Gamestate_Base {
#else
class Tutorial_State : public Wiimote_Game_State{
#endif
  

  /*struct Player {
    Player()
      : max_velocity(20.0f, 20.0f, 0.0f),
      mouse_scale(128.0f, 128.0f, 0.0f)
    {
		
    }

    Zeni::Camera camera;
    Zeni::Vector3f max_velocity;
    Zeni::Vector3f mouse_scale;
  } m_player;*/

public:
  Tutorial_State();
  ~Tutorial_State();

  void on_push();
  void on_pop();

  void on_key(const SDL_KeyboardEvent &event);

  void on_mouse_motion(const SDL_MouseMotionEvent &event);
  void on_mouse_button(const SDL_MouseButtonEvent &event);

  void perform_logic();

private:
  void step(const float &time_step);

  void render();

  Zeni::Time m_current_time;
  float m_time_passed;
  float m_max_time_step;  //< Optional
  float m_max_time_steps; //< Optional

  // whether or not we are doing the animation between turns
  bool turn_transition;
  // how far we are in the transition. 0 (or 2pi) is the first player's position, pi is the second player's position
  float transition_angle;

  // whose turn is it? 0 for the first player, 1 for the second player
  // note: when the camera is transitioning from the first player's side to the second player's side,
  //       current turn will already be 1 (not 0)
  int current_turn;
  Zeni::Camera camera;
};

#endif
