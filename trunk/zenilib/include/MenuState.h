/*
 *  MenuState.h
 *  Zenilib
 *
 *  Created by James Kovacs on 4/4/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MENUSTATE_H
#define MENUSTATE_H

#include <Zeni/Popup_State.h>
#include <Zeni/Sound_Source_Pool.h>
#include <Zeni/Widget_Gamestate.h>

#include "wiimote_state.h"

//class for creating a menu button with the style used throughout Connect493
class MenuButton {
public:
  MenuButton(std::string text_, Zeni::Point2f position_, 
             Zeni::JUSTIFY justify_ = Zeni::ZENI_CENTER, Zeni::Color color_ = Zeni::OFFWHITE);
  
  void performLogic();
  
  void select() {
    selected = true;
  }
  
  void deselect() {
    selected = false;
  }
  
  void render() {
    Zeni::Color renderColor = selected ? Zeni::OFFYELLOW : Zeni::OFFWHITE;
    Zeni::Font &fr = Zeni::get_Fonts()[fonts[currentFont]];
    fr.render_text(text, position, renderColor, justify);
  }
  
  virtual void onAccept() = 0;
  
private:
  bool selected;
  std::string text;
  Zeni::Point2f position;
  Zeni::JUSTIFY justify;
  Zeni::Color color;
  std::string fonts[7];
  int currentFont;
  float timePassed;
  Zeni::Chronometer<Zeni::Time> chrono;
  bool exhaling;
};

//base class for creating menus with the style used throughout Connect493
#ifdef _MACOSX
class MenuState : public Zeni::Gamestate_Base {
#else
  class MenuState : public Wiimote_Game_State {
#endif
    
    MenuState& operator=(const MenuState &);
    
  public:
    //every menu comes with a back button
    class BackButton : public MenuButton {
      BackButton(const BackButton &);
      BackButton & operator=(const BackButton &);
      
    public:
      BackButton()
      : MenuButton("Back", Zeni::Point2f(45.0f, Zeni::get_Video().get_screen_height() - 45.0f))
      {
      }
      
      void onAccept() {
        Zeni::get_Sound_Source_Pool().play_and_destroy(new Zeni::Sound_Source(Zeni::get_Sounds()["beep"]));
        Zeni::get_Game().pop_state();
      }
    };
    
    MenuState(std::string background_ = "MenuBG");
    
    void on_key(const SDL_KeyboardEvent &/*event*/);
    
#ifdef _MACOSX
    void on_wiimote_button(const Wiimote_Button_Event &event);
#endif
    
    void perform_logic();
    
    void render();
    
    int selectedButton;
    
    std::vector<MenuButton*> buttons;
    
  private:
    std::vector<Zeni::Point2f> glows; //container for the background commotion
    float timePassed;
    float backgroundTimePassed;
    Zeni::Chronometer<Zeni::Time> chrono;
    std::string background;
  };
  
#endif
  
