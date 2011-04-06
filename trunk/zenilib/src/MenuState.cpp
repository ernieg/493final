/*
 *  MenuState.cpp
 *  Zenilib
 *
 *  Created by James Kovacs on 4/4/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include <zenilib.h>
#include "MenuState.h"
using namespace std;
using namespace Zeni;

MenuButton::MenuButton(string text_, 
                       Point2f position_, 
                       JUSTIFY justify_, 
                       Color color_)
: text(text_), position(position_), justify(justify_), color(color_), 
currentFont(0), exhaling(false), timePassed(0.0f), selected(false)
{
  chrono.start();
  
  fonts[0] = "system30";
  fonts[1] = "system31";
  fonts[2] = "system32";
  fonts[3] = "system33";
  fonts[4] = "system34";
  fonts[5] = "system35";
  fonts[6] = "system36";
}

void MenuButton::performLogic() {
  if(selected) {
    if(chrono.seconds() - timePassed > 0.03f) {
      timePassed = chrono.seconds();
      if(exhaling)
        currentFont--;
      else
        currentFont++;
      
      exhaling = (currentFont == 6 && !exhaling) || (currentFont > 0 && exhaling);
    }
  }
  else {
    currentFont = 0;
    exhaling = false;
  }
}

  //base class for creating menus with the style used throughout Connect493
  MenuState::MenuState(string background_)
  :timePassed(0.0f), selectedButton(0), background(background_), backgroundTimePassed(0.0f)
  {
    chrono.start();
  }
  
  void MenuState::on_key(const SDL_KeyboardEvent &/*event*/)
  {
  }
  
  void MenuState::perform_logic() {
    get_Video().set_clear_color(get_Colors()["title_bg"]);
    float deltaT = chrono.seconds() - backgroundTimePassed;
    backgroundTimePassed = chrono.seconds();
    
    for(int i = 0; i < buttons.size(); i++)
      buttons[i]->performLogic();
    
    if(rand() % 100 < 5 && glows.size() < 2) {
      glows.push_back(Point2f(rand()%800, 600));
    }
    for(int i = 0; i < glows.size(); i++) {
      glows[i].y -= (int)(1500.0f * deltaT); // velocity of -1500px/s * delta t
      if(glows[i].y < -128) {
        glows.erase(glows.begin() + i);
        i--;
      }
    }
  }
  
  void MenuState::render() {
    render_image(background, Point2f(0.0f, 0.0f), Point2f(1024.0f, 1024.0f)); /* background image */
    
    for(int i = 0; i < glows.size(); i++) /* the background commotion */
      render_image("glow", Point2f(glows[i].x, glows[i].y), Point2f(glows[i].x + 64, glows[i].y + 128), false, Color((float)((float)glows[i].y/600.0f), 1.0f, 1.0f, 1.0f));
    
    for(int i = 0; i < buttons.size(); i++) /* render buttons */
      buttons[i]->render();
  }

