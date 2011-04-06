/* This file is part of the Zenipex Library.
* Copyleft (C) 2011 Mitchell Keith Bloch a.k.a. bazald
*
* The Zenipex Library is free software; you can redistribute it and/or 
* modify it under the terms of the GNU General Public License as 
* published by the Free Software Foundation; either version 2 of the 
* License, or (at your option) any later version.
*
* The Zenipex Library is distributed in the hope that it will be useful, 
* but WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License 
* along with the Zenipex Library; if not, write to the Free Software 
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 
* 02110-1301 USA.
*
* As a special exception, you may use this file as part of a free software
* library without restriction.  Specifically, if other files instantiate
* templates or use macros or inline functions from this file, or you compile
* this file and link it with other files to produce an executable, this
* file does not by itself cause the resulting executable to be covered by
* the GNU General Public License.  This exception does not however
* invalidate any other reasons why the executable file might be covered by
* the GNU General Public License.
*/

/**
* \class Zeni::Title_State
*
* \ingroup zenilib
*
* \brief Provide a simple title screen for games
*
* \author bazald
*
* Contact: bazald@zenipex.com
*/

#ifndef TITLE_STATE_H
#define TITLE_STATE_H

#include <Zeni/Popup_State.h>
#include <Zeni/Sound_Source_Pool.h>
#include <Zeni/Widget_Gamestate.h>

#include "MenuState.h"

namespace Zeni {

  template <typename PLAY_STATE, typename INSTRUCTIONS_STATE>
  class Title_State : public MenuState {
    Title_State(const Title_State<PLAY_STATE, INSTRUCTIONS_STATE> &);
    Title_State<PLAY_STATE, INSTRUCTIONS_STATE> & operator=(const Title_State<PLAY_STATE, INSTRUCTIONS_STATE> &);

  public:
    class Play : public MenuButton {
      Play(const Play &);
      Play & operator=(const Play &);
      
    public:
      Play()
      : MenuButton("Play", Zeni::Point2f(Zeni::get_Video().get_screen_width()/2.0f, 300.0f))
      {
      }
      
      void onAccept() {
        Zeni::get_Sound_Source_Pool().play_and_destroy(new Zeni::Sound_Source(Zeni::get_Sounds()["beep"]));
        get_Game().push_state(new PLAY_STATE());
      }
    };

    class Instructions_Button : public MenuButton {
      Instructions_Button(const Instructions_Button &);
      Instructions_Button & operator=(const Instructions_Button &);

    public:
      Instructions_Button()
        : MenuButton("Instructions", Zeni::Point2f(Zeni::get_Video().get_screen_width()/2.0f, 350.0f))
      {
      }
      
      void onAccept() {
        Zeni::get_Sound_Source_Pool().play_and_destroy(new Zeni::Sound_Source(Zeni::get_Sounds()["beep"]));
        get_Game().push_state(new INSTRUCTIONS_STATE());
      }
    };

    class Quit_Button : public MenuButton {
      Quit_Button(const Quit_Button &);
      Quit_Button & operator=(const Quit_Button &);
      
      public:
        Quit_Button()
        : MenuButton("Quit", Zeni::Point2f(Zeni::get_Video().get_screen_width()/2.0f, 400.0f))
        {
        }
      
        void onAccept() {
          Zeni::get_Sound_Source_Pool().play_and_destroy(new Zeni::Sound_Source(Zeni::get_Sounds()["beep"]));
          throw Quit_Event();
        }
    };

    Title_State(const std::string background_)
    : MenuState(background_)
    {
      Play *playButton = new Play();
      Instructions_Button *instructionsButton = new Instructions_Button();
      Quit_Button *quitButton = new Quit_Button();
      
      buttons.push_back(playButton);
      buttons.push_back(instructionsButton);
      buttons.push_back(quitButton);
      
      buttons[selectedButton]->select(); /* set default selected */
    }

    ~Title_State() {
      get_Video().set_clear_color(Color(1.0f, 0.0f, 0.0f, 0.0f));
    }
    
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
              if(selectedButton < 2) {
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

    void perform_logic() {
      MenuState::perform_logic();
      
      for(int i = 0; i < buttons.size(); i++)
        buttons[i]->performLogic();
      
    }

    void render() {
      MenuState::render();
    }
  };

}

#endif
