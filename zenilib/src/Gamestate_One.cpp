/* This file is part of the Zenipex Library.
* Copyleft (C) 2008 Mitchell Keith Bloch a.k.a. bazald
*
* This source file is simply under the public domain.
*/

#include <zenilib.h>

#include "Tutorial_State.h"
#include "RegisterState.h"


using namespace std;
using namespace Zeni;

static vector<string> g_args;


class Instructions_State : public MenuState {
  
  class ToKeyInst : public MenuButton {
    ToKeyInst(const ToKeyInst &);
    ToKeyInst & operator=(const ToKeyInst &);
    
  public:
    ToKeyInst()
    : MenuButton("Keyboard >", Zeni::Point2f(695.0f, Zeni::get_Video().get_screen_height() - 45.0f))
    {
    }
    
    void onAccept() {
      Zeni::get_Sound_Source_Pool().play_and_destroy(new Zeni::Sound_Source(Zeni::get_Sounds()["beep"]));
    }
  };

  class ToWiiInst : public MenuButton {
    ToWiiInst(const ToWiiInst &);
    ToWiiInst & operator=(const ToWiiInst &);
    
  public:
    ToWiiInst()
    : MenuButton("< Wii Instructions", Zeni::Point2f(218.0f, 295.0f))
    {
    }
    
    void onAccept() {
      Zeni::get_Sound_Source_Pool().play_and_destroy(new Zeni::Sound_Source(Zeni::get_Sounds()["beep"]));
    }
  };
  
public:
  Instructions_State()
  : MenuState(), wii(true)
  {
    ToKeyInst *toKeyInst = new ToKeyInst();
    BackButton *backButton = new BackButton();
    buttons.push_back(toKeyInst);
    buttons.push_back(backButton);
    buttons[0]->select();
  }

private:
  void on_wiimote_button(const Wiimote_Button_Event &event) {
    switch(event.button) {
      case BUTTON_B:
        if(event.pressed)
          get_Game().pop_state();
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
        
      case BUTTON_A:
        if(event.pressed) {
          buttons[selectedButton]->onAccept();
          if(buttons.size() > 1 && selectedButton == 0) {
            buttons.erase(buttons.begin());
            buttons[selectedButton]->select();
            wii = false;
          }
        }
        break;
    }
  }
  
  void on_key(const SDL_KeyboardEvent &event) {
    Wiimote_Button_Event fakeEvent;
    
    switch(event.keysym.sym) {
      case SDLK_BACKSPACE:
      case SDLK_ESCAPE:
        if(event.state == SDL_PRESSED)
          fakeEvent.button = BUTTON_B;
        break;
        
      case SDLK_RIGHT:
      case SDLK_UP:
        if(event.state == SDL_PRESSED) {
          fakeEvent.button = BUTTON_UP;
        }
        break;
        
      case SDLK_LEFT:
      case SDLK_DOWN:
        if(event.state == SDL_PRESSED) {
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

  void render() {
    MenuState::render();
    
    Zeni::Font &title = get_Fonts()["system100"];
    
    title.render_text("Instructions",
                   Point2f(400.0f, 50.0f - 0.5f * title.get_text_height()),
                   OFFWHITE,
                   ZENI_CENTER);
    
    if(wii) {
    Zeni::Font &fr = get_Fonts()["system20"];
    
    fr.render_text("HOW TO PLAY:\n\n"
                   "The first player to line up four chips vertically, \nhorizontally, or diagonally wins the game.\n\n",
                   Point2f(400.0f, 150.0f - 0.5f * fr.get_text_height()),
                   OFFWHITE,
                   ZENI_CENTER);
    
    render_image("Instructions", Point2f(100.0f, 235.0f), Point2f(1124.0f, 747.0f));
    }
    else {
      render_image("KeyInst", Point2f(0.0f, 0.0f), Point2f(1024.0f, 1024.0f));
    }
  }
  
  bool wii;
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
    get_Video().set_title("Connect 493");

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
    gr.push_state(new Title_State<RegisterState, Instructions_State>("TitleBG"));
  }

}
