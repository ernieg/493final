/* This file is part of the Zenipex Library.
* Copyleft (C) 2008 Mitchell Keith Bloch a.k.a. bazald
*
* This source file is simply under the public domain.
*/

#include <zenilib.h>

#include "Tutorial_State.h"
#include "MenuState.h"

using namespace std;
using namespace Zeni;

static vector<string> g_args;

class Instructions_State : public MenuState {

public:
  Instructions_State()
  {
  }

private:
  void on_key(const SDL_KeyboardEvent &event) {
    if(event.keysym.sym == SDLK_ESCAPE) {
      if(event.state == SDL_PRESSED)
        get_Game().pop_state();
    }
  }

  void render() {
    MenuState::render();
    
    Zeni::Font &title = get_Fonts()["system100"];
    
    title.render_text("Instructions",
                   Point2f(400.0f, 50.0f - 0.5f * title.get_text_height()),
                   OFFWHITE,
                   ZENI_CENTER);
    
    Zeni::Font &fr = get_Fonts()["system20"];
    
    fr.render_text("HOW TO PLAY:\n\n"
                   "The first player to line up four chips vertically, \nhorizontally, or diagonally wins the game.\n\n"
                   "CONTROLS:\n\n"
                   "Move the Wiimote until your chip is in the correct \ncolumn, then press the B button to drop it.\n",
                   Point2f(400.0f, 250.0f - 0.5f * fr.get_text_height()),
                   OFFWHITE,
                   ZENI_CENTER);
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

	get_Sound().set_BGM("music/bg_music_1.ogg");
	get_Sound().set_BGM_looping(true);
	get_Sound().set_BGM_gain(0.5f);
	get_Sound().play_BGM();
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
