/*
 *  OptionsState.h
 *  Zenilib
 *
 *  Created by James Kovacs on 4/11/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef OPTIONS_H
#define OPTIONS_H
//get_Sound().set_listener_muted(!is_checked());
#include <zenilib.h>
#include <fstream>
#include "GameModel.h"

class OptionsState : public MenuState {
  
public:
	OptionsState()
	{
    soundOn = !Zeni::get_Sound().is_listener_muted();
    std::ifstream infile;
    infile.open("difficulty.txt");
    
    if(infile.fail())
      difficulty = 0; //default to easy
    else {
      int temp;
      infile >> temp;
      if(temp > 2 || temp < 0)
        difficulty = 0;
      else
        difficulty = temp;
    }
    
    DifficultyButton *difficultyButton = new DifficultyButton();
    SoundButtonOn *soundButtonOn = new SoundButtonOn();
    SoundButtonOff *soundButtonOff = new SoundButtonOff();
    BackButton *backButton = new BackButton();
    
    buttons.push_back(difficultyButton);
    buttons.push_back(soundButtonOn);
    buttons.push_back(soundButtonOff);
    buttons.push_back(backButton);
    
    buttons[selectedButton]->select();
	}
  
  class DifficultyButton : public MenuButton {
    DifficultyButton(const DifficultyButton &);
    DifficultyButton & operator=(const DifficultyButton &);
    
  public:
    DifficultyButton()
    : MenuButton("<                    >", Zeni::Point2f(400.0f, 250.0f))
    {
    }
    
    void onAccept() {
      Zeni::get_Sound_Source_Pool().play_and_destroy(new Zeni::Sound_Source(Zeni::get_Sounds()["beep"]));
    }
  };
  
  class SoundButtonOn : public MenuButton {
    SoundButtonOn(const SoundButtonOn &);
    SoundButtonOn & operator=(const SoundButtonOn &);
    
  public:
    SoundButtonOn()
    : MenuButton("On", Zeni::Point2f(300.0f, 425.0f))
    {
    }
    
    void onAccept() {
      Zeni::get_Sound_Source_Pool().play_and_destroy(new Zeni::Sound_Source(Zeni::get_Sounds()["beep"]));
      Zeni::get_Sound().set_listener_muted(false);
    }
  };
  
  class SoundButtonOff : public MenuButton {
    SoundButtonOff(const SoundButtonOff &);
    SoundButtonOff & operator=(const SoundButtonOff &);
    
  public:
    SoundButtonOff()
    : MenuButton("Off", Zeni::Point2f(500.0f, 425.0f))
    {
    }
    
    void onAccept() {
      Zeni::get_Sound_Source_Pool().play_and_destroy(new Zeni::Sound_Source(Zeni::get_Sounds()["beep"]));
      Zeni::get_Sound().set_listener_muted(true);
    }
  };
  
private:
  
  void on_pop() {
    std::ofstream outfile;
    outfile.open("difficulty.txt");
    outfile << difficulty;
    
    getGameModel().setDifficulty(difficulty);
  }
  
	void on_wiimote_button(const Wiimote_Button_Event &event) {
    switch(event.button) {
      case BUTTON_A:
        if(event.pressed) {
          buttons[selectedButton]->onAccept();
          if(selectedButton == 1)
            soundOn = true;
          else if(selectedButton == 2)
            soundOn = false;
        }
        break;
        
      case BUTTON_RIGHT:
        if(event.pressed) {
          if(selectedButton == 0) {
            difficulty = difficulty == 2 ? 0 : difficulty + 1;
          }
          else if(selectedButton == 1) {
            buttons[selectedButton]->deselect();
            selectedButton = 2;
            buttons[selectedButton]->select();
          }
        }
        break;
        
      case BUTTON_LEFT:
        if(event.pressed) {
          if(selectedButton == 0) {
            difficulty = difficulty == 0 ? 2 : difficulty - 1;
          }
          else if(selectedButton == 2) {
            buttons[selectedButton]->deselect();
            selectedButton = 1;
            buttons[selectedButton]->select();
          }
        }
        break;
        
      case BUTTON_UP:
        if(event.pressed) {
          if(selectedButton > 0) {
            if(selectedButton == 1 || selectedButton == 2) {
              buttons[selectedButton]->deselect();
              selectedButton = 0;
              buttons[selectedButton]->select();
            }
            else if(selectedButton == 3) {
              buttons[selectedButton]->deselect();
              selectedButton = soundOn ? 1 : 2;
              buttons[selectedButton]->select();
            }
          }
        }
        break;
        
      case BUTTON_DOWN:
        if(event.pressed) {
          if(selectedButton == 0) {
            buttons[selectedButton]->deselect();
            selectedButton = soundOn ? 1 : 2;
            buttons[selectedButton]->select();
          }
          else if(selectedButton == 1 || selectedButton == 2) {
            buttons[selectedButton]->deselect();
            selectedButton = 3;
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
        
      case SDLK_LEFT:
        if(event.state == SDL_PRESSED) {
          fakeEvent.button = BUTTON_LEFT;
        }
        break;
        
      case SDLK_RIGHT:
        if(event.state == SDL_PRESSED){
          fakeEvent.button = BUTTON_RIGHT;
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
    
		title.render_text("Options",
                      Zeni::Point2f(400.0f, 50.0f - 0.5f * title.get_text_height()),
                      Zeni::OFFWHITE,
                      Zeni::ZENI_CENTER);
    
    Zeni::Font &text = Zeni::get_Fonts()["system30"];
    
    text.render_text("Difficulty",
                 Zeni::Point2f(400.0f, 240.0f - 0.5f * title.get_text_height()),
                 Zeni::OFFWHITE,
                 Zeni::ZENI_CENTER);
    
    std::string difficulty_s;
    switch (difficulty) {
      case 0:
        difficulty_s = "Easy";
        break;
      case 1:
        difficulty_s = "Medium";
        break;
      case 2:
        difficulty_s = "Hard";
        break;
      default:
        difficulty_s = "Easy";
        break;
    }
    text.render_text(difficulty_s,
                     Zeni::Point2f(400.0f, 250.0f),
                     Zeni::OFFWHITE,
                     Zeni::ZENI_CENTER);
    
    text.render_text("Sound",
                     Zeni::Point2f(400.0f, 415.0f - 0.5f * title.get_text_height()),
                     Zeni::OFFWHITE,
                     Zeni::ZENI_CENTER);
    
    Zeni::Point2f offCheck(485.0f, 465.0f);
    Zeni::Point2f onCheck(285.0f, 465.0f);
    Zeni::Point2f checkPos = soundOn ? onCheck : offCheck;
    render_image("Check", checkPos, Zeni::Point2f(checkPos.x + 32.0f, checkPos.y + 32.0f));
    
  }
  
  int difficulty;
  bool soundOn;
};

#endif

