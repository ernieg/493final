/*
 *  PauseState.h
 *  Zenilib
 *
 *  Created by James Kovacs on 4/11/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PAUSESTATE_H
#define PAUSESTATE_H

#include <zenilib.h>

class PauseState : public MenuState {
  
public:
	PauseState()
	{
    ContinueButton *continueButton = new ContinueButton();
    QuitButton *quitButton = new QuitButton();
    
    buttons.push_back(continueButton);
    buttons.push_back(quitButton);
    
    buttons[selectedButton]->select();
    
    grabbed = Zeni::get_Video().is_mouse_grabbed();
    hidden = Zeni::get_Video().is_mouse_hidden();
    
    Zeni::get_Video().mouse_hide(false);
    Zeni::get_Video().mouse_grab(false);
	}
  
  class ContinueButton : public MenuButton {
    ContinueButton(const ContinueButton &);
    ContinueButton & operator=(const ContinueButton &);
    
  public:
    ContinueButton()
    : MenuButton("Continue", Zeni::Point2f(400.0f, 300.0f))
    {
    }
    
    void onAccept() {
      Zeni::get_Sound_Source_Pool().play_and_destroy(new Zeni::Sound_Source(Zeni::get_Sounds()["beep"]));
      Zeni::get_Game().pop_state();
    }
  };
  
  class QuitButton : public MenuButton {
    QuitButton(const QuitButton &);
    QuitButton & operator=(const QuitButton &);
    
  public:
    QuitButton()
    : MenuButton("Quit", Zeni::Point2f(400.0f, 400.0f))
    {
    }
    
    void onAccept() {
      Zeni::get_Sound_Source_Pool().play_and_destroy(new Zeni::Sound_Source(Zeni::get_Sounds()["beep"]));
      Zeni::get_Game().pop_state(); //this
      Zeni::get_Game().pop_state(); //game
      Zeni::get_Game().pop_state(); //player select
    }
  };
  
private:
  
  void on_pop() {
    Zeni::get_Video().mouse_hide(hidden);
    Zeni::get_Video().mouse_grab(grabbed);
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
    Zeni::get_Video().set_2d();
		MenuState::render();
    
    Zeni::Font &title = Zeni::get_Fonts()["system100"];
    
		title.render_text("Paused",
                      Zeni::Point2f(400.0f, 50.0f - 0.5f * title.get_text_height()),
                      Zeni::OFFWHITE,
                      Zeni::ZENI_CENTER);
    
  }
  
  bool hidden, grabbed;
};

#endif

