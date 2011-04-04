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

//class for creating a menu button with the style used throughout Connect493
class MenuButton : public Zeni::Text_Button {
public:
  MenuButton(Zeni::Point2f topLeft, Zeni::Point2f bottomRight, std::string text_)
    : Text_Button(topLeft, bottomRight, "system30", text_), timePassed(0.0f),
  hovering(false), exhaling(false), text(text_), col(Zeni::OFFWHITE), font("system30")
  {
    give_Renderer(new Zeni::Widget_Renderer_Tricolor(Zeni::TRANSPARENT, Zeni::TRANSPARENT, Zeni::TRANSPARENT,
                                               Zeni::OFFWHITE, Zeni::OFFYELLOW, Zeni::OFFYELLOW));
    chrono.start();
  }
  
  void on_hover() {
    hovering = true;
    col = Zeni::OFFYELLOW;
  }
  
  void on_unhover() {
    hovering = false;
    exhaling = false;
    font[7] = '0';
    col = Zeni::OFFWHITE;
  }
  
  void on_accept() {
    Zeni::get_Sound_Source_Pool().play_and_destroy(new Zeni::Sound_Source(Zeni::get_Sounds()["beep"]));
  }
  
  void perform_logic() {
    if(hovering) {
      if(chrono.seconds() - timePassed > 0.03f) {
        timePassed = chrono.seconds();
        if(exhaling)
          font[7]--;
        else
          font[7]++;
      
        exhaling = (font[7] == '6' && !exhaling) || (font[7] > '0' && exhaling);
      }
    }
    else {
      font[7] = '0';
      exhaling = false;
    }
    
    give_Renderer(new Widget_Renderer_Text(font, text, col));    
  }
  
private:
  bool exhaling;
  bool hovering;
  std::string text;
  std::string font;
  Zeni::Color col;
  Zeni::Chronometer<Zeni::Time> chrono;
  float timePassed;
};

//base class for creating menus with the style used throughout Connect493
class MenuState : public Zeni::Widget_Gamestate {
  MenuState& operator=(const MenuState &);
  
public:
  //every menu comes with a back button
  class BackButton : public MenuButton {
    BackButton(const BackButton &);
    BackButton & operator=(const BackButton &);
    
  public:
    BackButton()
    : MenuButton(Zeni::Point2f(0.0f, Zeni::get_Video().get_screen_height() - 60.0f), 
                 Zeni::Point2f(100.0f, Zeni::get_Video().get_screen_height()), 
                 "Back")
    {
    }
    
    void on_accept() {
      MenuButton::on_accept();
      Zeni::get_Game().pop_state();
    }
  } backButton;
  
  MenuState()
  : Zeni::Widget_Gamestate(std::make_pair(Zeni::Point2f(0.0f, 0.0f), Zeni::Point2f(800.0f, 600.0f)))
  {
    m_widgets.lend_Widget(backButton);
    
    Zeni::get_Video().set_clear_color(Zeni::get_Colors()["title_bg"]);
  }
  
  void on_key(const SDL_KeyboardEvent &/*event*/)
  {
  }
  
  void perform_logic() {
    Widget_Gamestate::perform_logic();
    Zeni::get_Video().set_clear_color(Zeni::get_Colors()["title_bg"]);
    
    if(rand() % 100 < 5 && glows.size() < 2) {
      glows.push_back(Zeni::Point2f(rand()%800, 600));
    }
    for(int i = 0; i < glows.size(); i++) {
      glows[i].y -= 30;
      if(glows[i].y < -128) {
        glows.erase(glows.begin() + i);
        i--;
      }
    }
  }
  
  void render() {
    Zeni::render_image("MenuBG", Zeni::Point2f(0.0f, 0.0f), Zeni::Point2f(1024.0f, 1024.0f));
    for(int i = 0; i < glows.size(); i++) 
      Zeni::render_image("glow", Zeni::Point2f(glows[i].x, glows[i].y), Zeni::Point2f(glows[i].x + 64, glows[i].y + 128), false, Zeni::Color((float)((float)glows[i].y/600.0f), 1.0f, 1.0f, 1.0f));
    Widget_Gamestate::render();
  }
  
private:
  std::vector<Zeni::Point2f> glows; //container for the background commotion
};

#endif

