/* This file is part of the Zenipex Library.
* Copyleft (C) 2008 Mitchell Keith Bloch a.k.a. bazald
*
* This source file is simply under the public domain.
*/

#include <zenilib.h>

#include "Tutorial_State.h"

using namespace std;
using namespace Zeni;

static vector<string> g_args;

class Instructions_State : public Gamestate_Base {
  Text_Box tb;

public:
  Instructions_State()
    : tb(Point2f(), Point2f(800.0f, 600.0f), "system_36_800x600",
    "This game uses WASD keys in addition to mouse input. "
    "It shows how to render a very simple 3D scene.", Color())
  {
    tb.give_BG_Renderer(new Widget_Renderer_Color(get_Colors()["black"]));
  }

private:
  void on_key(const SDL_KeyboardEvent &event) {
    if(event.keysym.sym == SDLK_ESCAPE) {
      if(event.state == SDL_PRESSED)
        get_Game().pop_state();
    }
  }

  void render() {
    get_Video().set_2d(make_pair(Point2f(), Point2f(800.0f, 600.0f)), true);

    tb.render();
  }
};

namespace Zeni {

  Gamestate_One::Gamestate_One(const vector<string> * const args) {
#ifdef GRAB_MOUSE
    SDL_WM_GrabInput(SDL_GRAB_ON);
#endif
#ifdef HIDE_MOUSE
    SDL_ShowCursor(0);
#endif

    if(args)
      g_args = *args;

    get_Textures();
    get_Fonts();
    get_Sounds();
    get_Video().set_title("Wii Connect Four");
  }

  Gamestate_One::~Gamestate_One() {
  }

  void Gamestate_One::perform_logic() {
    // TODO: Parse commandline arguments and bootstrap into titlestate or other gamestate;
 
    Game &gr = get_Game();
    gr.pop_state();
    gr.push_state(new Title_State<Tutorial_State, Instructions_State>("Wii Connect Four"));
  }

}
