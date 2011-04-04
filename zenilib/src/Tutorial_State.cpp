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

using namespace std;
using namespace Zeni;

Tutorial_State::Tutorial_State()
: m_time_passed(0.0f),
m_max_time_step(1.0f / 20.0f), // make the largest physics step 1/20 of a second
m_max_time_steps(10.0f), // allow no more than 10 physics steps per frame
turn_transition(false),
transition_angle(0.0f),
current_turn(0)
{
  set_pausable(true);
  getGameModel().makeNewCurrentCoin(0);

  m_player.camera.position.z = 100.0f;
  m_player.camera.look_at(BOARD_CENTER_MIDDLE);
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

void Tutorial_State::on_key(const SDL_KeyboardEvent &event) {
	switch(event.keysym.sym) {
		case SDLK_w:
			m_controls.up = event.type == SDL_KEYDOWN;
			break;

		case SDLK_a:
			m_controls.left = event.type == SDL_KEYDOWN;
			break;

        case SDLK_s:
			m_controls.down = event.type == SDL_KEYDOWN;
			break;

		case SDLK_d:
			m_controls.right = event.type == SDL_KEYDOWN;
			break;

		case SDLK_e:
			// debugging: (for testing out the board-turning animation)
			if ( event.type == SDL_KEYDOWN )
			{
			  current_turn = (current_turn+1)%2;
			  turn_transition = true;
			}
			break;

		default:
			Gamestate_Base::on_key(event);
	}
}

void Tutorial_State::on_mouse_motion(const SDL_MouseMotionEvent &event) {
  /*Camera &camera = m_player.camera;

  camera.turn_left_xy(-event.xrel / m_player.mouse_scale.i);
	
  // Back up a couple vectors
  const Quaternion prev_orientation = camera.orientation;
	const Vector3f prev_up = camera.get_up();

	camera.adjust_pitch(event.yrel / m_player.mouse_scale.j);

  // Restore the backup vectors if flipped over
  // (if the up vector is pointing down)
  //
  // Note that we want to be sure not to freeze a player 
  // who is already flipped (for whatever reason).
   
	if(camera.get_up().k < 0.0f && prev_up.k >= 0.0f)
    camera.orientation = prev_orientation;*/

	float yMovement = event.xrel / 100.0f;
	float zMovement = -event.yrel / 100.0f;

	yMovement *= (current_turn == 0 ) ? -1.0f : 1.0f;

	Point3f oldPos(getGameModel().getCurrentCoin()->getPosition());

	Point3f newPos = oldPos + Point3f(0.0f, yMovement, zMovement);

	// check for collisions between the board and the coin here
	if ( newPos.z < 90.0f )
	{
		newPos.z = 90.0f;
	}

	getGameModel().getCurrentCoin()->setPosition(newPos);
}

void Tutorial_State::perform_logic() {
  {// Update time_passed
    const Time temp_time = get_Timer().get_time();
    m_time_passed += temp_time.get_seconds_since(m_current_time);
    m_current_time = temp_time;
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
  if ( turn_transition )
  {
	transition_angle += m_time_passed * (-0.5f * cos(2.0f * transition_angle) + 0.6f) * 4.0f;

	m_player.camera.position.x = -BOARD_DIST_X*cos(transition_angle) + BOARD_DIST_X;
	m_player.camera.position.y = -BOARD_DIST_X*sin(transition_angle);

	m_player.camera.look_at(BOARD_CENTER_MIDDLE);

	// it is the first player's turn, so stop at angle == 0 (or 2pi)
	if ( current_turn == 0 )
	{
	  if ( 2.0f*pi - transition_angle < ANGLE_EPSILON )
	  {
	    transition_angle = 0.0f;
		turn_transition = false;

		m_player.camera.position.x = 0.0f;
		m_player.camera.position.y = 0.0f;
		m_player.camera.look_at(BOARD_CENTER_MIDDLE);
	  }
	}
	else // it is the second player's turn, so stop at angle pi
	{
	  if ( pi - transition_angle < ANGLE_EPSILON )
	  {
		transition_angle = pi;
		turn_transition = false;

		m_player.camera.position.x = 200.0f;
		m_player.camera.position.y = 0.0f;
		m_player.camera.look_at(BOARD_CENTER_MIDDLE);
	  }
	}
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
  m_player.camera.move_left_xy(m_player.max_velocity.i * time_step *
                               (m_controls.left - m_controls.right));
  m_player.camera.move_forward_xy(m_player.max_velocity.j * time_step *
                                  (m_controls.up - m_controls.down));
}

void Tutorial_State::render() {
  Video &vr = get_Video();

  m_player.camera.far_clip = 2000.0f;
  vr.set_3d(m_player.camera);

  Vertex3f_Texture p0(m_rect.position,                             Point2f(0.0f, 0.0f));
  Vertex3f_Texture p1(m_rect.position               + m_rect.vec1, Point2f(0.0f, 1.0f));
  Vertex3f_Texture p2(m_rect.position + m_rect.vec0 + m_rect.vec1, Point2f(1.0f, 1.0f)); 
  Vertex3f_Texture p3(m_rect.position + m_rect.vec0,               Point2f(1.0f, 0.0f));
  Material material("logo");

	Quadrilateral<Vertex3f_Texture> quad(p0, p1, p2, p3);
    quad.fax_Material(&material);

	//vr.render(quad);

	// skybox
	Vector3f v_f(1.0f, 0.0f, 0.0f);
    Vector3f v_u(0.0f, 0.0f, 1.0f);
    Vector3f v_l(0.0f, 1.0f, 0.0f);

	Point2f texture_a(0.0f, 0.0f);
	Point2f texture_b(0.0f, 1.0f);
	Point2f texture_c(1.0f, 1.0f);
	Point2f texture_d(1.0f, 0.0f);

	Point3f skybox_p0 = m_player.camera.position + SKYBOX_DIST*(v_f + v_u + v_l);
	Point3f skybox_p1 = m_player.camera.position + SKYBOX_DIST*(v_f - v_u + v_l);
	Point3f skybox_p2 = m_player.camera.position + SKYBOX_DIST*(v_f - v_u - v_l);
	Point3f skybox_p3 = m_player.camera.position + SKYBOX_DIST*(v_f + v_u - v_l);
	Point3f skybox_p4 = m_player.camera.position + SKYBOX_DIST*(-v_f - v_u + v_l);
	Point3f skybox_p5 = m_player.camera.position + SKYBOX_DIST*(-v_f + v_u + v_l);
	Point3f skybox_p6 = m_player.camera.position + SKYBOX_DIST*(-v_f - v_u - v_l);
	Point3f skybox_p7 = m_player.camera.position + SKYBOX_DIST*(-v_f + v_u - v_l);

	Vertex3f_Texture a_p0(skybox_p0, texture_a);
    Vertex3f_Texture a_p1(skybox_p1, texture_b);
    Vertex3f_Texture a_p2(skybox_p2, texture_c); 
    Vertex3f_Texture a_p3(skybox_p3, texture_d);

	Vertex3f_Texture b_p5(skybox_p5, texture_a);
	Vertex3f_Texture b_p4(skybox_p4, texture_b);
	Vertex3f_Texture b_p1(skybox_p1, texture_c);
	Vertex3f_Texture b_p0(skybox_p0, texture_d);

	Vertex3f_Texture c_p7(skybox_p7, texture_a);
	Vertex3f_Texture c_p6(skybox_p6, texture_b);
	Vertex3f_Texture c_p4(skybox_p4, texture_c);
	Vertex3f_Texture c_p5(skybox_p5, texture_d);
	
	Vertex3f_Texture d_p3(skybox_p3, texture_a);
	Vertex3f_Texture d_p2(skybox_p2, texture_b);
	Vertex3f_Texture d_p6(skybox_p6, texture_c);
	Vertex3f_Texture d_p7(skybox_p7, texture_d);

	Vertex3f_Texture e_p0(skybox_p0, texture_a);
	Vertex3f_Texture e_p3(skybox_p3, texture_b);
	Vertex3f_Texture e_p7(skybox_p7, texture_c);
	Vertex3f_Texture e_p5(skybox_p5, texture_d);

	Vertex3f_Texture f_p1(skybox_p1, texture_a);
	Vertex3f_Texture f_p4(skybox_p4, texture_b);
	Vertex3f_Texture f_p6(skybox_p6, texture_c);
	Vertex3f_Texture f_p2(skybox_p2, texture_d);
	
    Material skybox_material("skybox");

	Quadrilateral<Vertex3f_Texture> quad_a(a_p0, a_p1, a_p2, a_p3);
	Quadrilateral<Vertex3f_Texture> quad_b(b_p5, b_p4, b_p1, b_p0);
	Quadrilateral<Vertex3f_Texture> quad_c(c_p7, c_p6, c_p4, c_p5);
	Quadrilateral<Vertex3f_Texture> quad_d(d_p3, d_p2, d_p6, d_p7);
	Quadrilateral<Vertex3f_Texture> quad_e(e_p0, e_p3, e_p7, e_p5);
	Quadrilateral<Vertex3f_Texture> quad_f(f_p1, f_p4, f_p6, f_p2);
    quad_a.fax_Material(&skybox_material);
	quad_b.fax_Material(&skybox_material);
	quad_c.fax_Material(&skybox_material);
	quad_d.fax_Material(&skybox_material);
	quad_e.fax_Material(&skybox_material);
	quad_f.fax_Material(&skybox_material);

	vr.render(quad_a);
	vr.render(quad_b);
	vr.render(quad_c);
	vr.render(quad_d);
	vr.render(quad_e);
	vr.render(quad_f);

	// render the board (and all the coins in it)
	getGameModel().getBoard()->render();

	// render the current coin (the one being moved around by the player)
	getGameModel().getCurrentCoin()->render();
}
