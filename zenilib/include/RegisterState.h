/*
 *  RegisterState.h
 *  Zenilib
 *
 *  Created by James Kovacs on 4/6/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef REGISTERSTATE_H
#define REGISTERSTATE_H

#include "GameModel.h"
#include "MenuState.h"
#include "Tutorial_State.h"
#include "EasyAgent.h"
#include "MediumAgent.h"
#include "HardAgent.h"

class RegisterState : public MenuState {
public:
  class P1Arrows : public MenuButton {
    P1Arrows(const P1Arrows &);
    P1Arrows & operator=(const P1Arrows &);
    
  public:
    P1Arrows()
    : MenuButton("<                   >", Zeni::Point2f(218.0f, 295.0f))
    {
    }
    
    void onAccept() {
      Zeni::get_Sound_Source_Pool().play_and_destroy(new Zeni::Sound_Source(Zeni::get_Sounds()["beep"]));
    }
  };
  
  class P1Ready : public MenuButton {
    P1Ready(const P1Ready &);
    P1Ready & operator=(const P1Ready &);
    
  public:
    P1Ready()
    : MenuButton("Ready?", Zeni::Point2f(218.0f, 500.0f))
    {
    }
    
    void onAccept() {
      Zeni::get_Sound_Source_Pool().play_and_destroy(new Zeni::Sound_Source(Zeni::get_Sounds()["beep"]));
    }
  };
  
  class P2Arrows : public MenuButton {
    P2Arrows(const P2Arrows &);
    P2Arrows & operator=(const P2Arrows &);
    
  public:
    P2Arrows()
    : MenuButton("<                   >", Zeni::Point2f(568.0f, 295.0f))
    {
    }
    
    void onAccept() {
      Zeni::get_Sound_Source_Pool().play_and_destroy(new Zeni::Sound_Source(Zeni::get_Sounds()["beep"]));
    }
  };
  
  class P2Ready : public MenuButton {
    P2Ready(const P2Ready &);
    P2Ready & operator=(const P2Ready &);
    
  public:
    P2Ready()
    : MenuButton("Ready?", Zeni::Point2f(568.0f, 500.0f))
    {
    }
    
    void onAccept() {
      Zeni::get_Sound_Source_Pool().play_and_destroy(new Zeni::Sound_Source(Zeni::get_Sounds()["beep"]));
    }
  };
  
public:
  RegisterState()
  : MenuState(), p1Coin(0), p2Coin(-1), p2SelectedButton(-1), 
    p1Done(false), p2Done(false), timePassed(0.0f)
  {
    chrono.start();
    
    /*  fill in our containers, doing it this way so we don't
        have to change any code when the purple one gets added */
    for(int i = 0; i < NUMCOINS; i++) {
      switch (i) {
        case 0:
          colors[i] = "Red";
          coins[i] = new Zeni::Model("models/redChip.3DS");
          break;
        case 1:
          colors[i] = "Black";
          coins[i] = new Zeni::Model("models/blackChip.3DS");
          break;
        case 2:
          colors[i] = "Green";
          coins[i] = new Zeni::Model("models/greenChip.3DS");
          break;
        case 3:
          colors[i] = "Blue";
          coins[i] = new Zeni::Model("models/blueChip.3DS");
          break;
        case 4:
          colors[i] = "White";
          coins[i] = new Zeni::Model("models/whiteChip.3DS");
          break;
        case 5:
          colors[i] = "Purple";
          coins[i] = new Zeni::Model("models/purpleChip.3DS");
          break;
      }
    }
    
    P1Arrows *p1arrows = new P1Arrows();
    p1Buttons.push_back(p1arrows);
    
    P1Ready *p1ready = new P1Ready();
    p1Buttons.push_back(p1ready);
    
    p1Buttons[selectedButton]->select();
    
    P2Arrows *p2arrows = new P2Arrows();
    p2Buttons.push_back(p2arrows);
    
    P2Ready *p2ready = new P2Ready();
    p2Buttons.push_back(p2ready);
  }
  
private:
  
  void on_push() {
    reset();
  }
  
  void on_pop() {
    reset();
  }
  
  /*  reset the state to the default start, keeping the previous chip
   *  values. This gets called after pushing the play state on so a 
   *  fresh version of this state will appear on_pop
   */
  void reset() {
    p1Done = false;
    p2Done = false;
    p2SelectedButton = p2SelectedButton == -1 ? -1 : 0;
    selectedButton = 0;
    p1Buttons[selectedButton]->select();
    if(p2SelectedButton != -1)
      p2Buttons[p2SelectedButton]->select();
  }
  
  /*  so much logic behind making sure each player is done
   *  before continuing, making sure they're both not the
   *  same chip, showing certain buttons based on the 
   *  player's progress throughout the state, etc. There's
   *  probably a more efficient/readable way to do this
   *  by reusing code for each player, but oh well...
   */
  void on_wiimote_button(const Wiimote_Button_Event &event) {
    switch (event.wiimote) {
      
      /* Wiimote 0 */
      case 0:
        switch(event.button) {
          case BUTTON_B:
            if(event.pressed) {
              if(selectedButton > 1) {
                p1Done = false;
                selectedButton--;
                p1Buttons[selectedButton]->select();
              }
              else if(selectedButton == 1) {
                p1Buttons[selectedButton]->deselect();
                selectedButton--;
                p1Buttons[selectedButton]->select();
              }
              else
                Zeni::get_Game().pop_state();
            }
            break;
        
        case BUTTON_LEFT:
            if(event.pressed) {
              if(selectedButton == 0) {
                p1Coin--;
                if(p1Coin == p2Coin && p2Done) {
                  p1Coin--;
                }
                if(p1Coin < 0) {
                  if(p2Coin == NUMCOINS-1)
                    p1Coin = NUMCOINS-2;
                  else
                    p1Coin = NUMCOINS-1;
                }
              }
            }
            break;
        
        case BUTTON_RIGHT:
          if(event.pressed) {
            if(selectedButton == 0) {
              p1Coin++;
              if(p1Coin == p2Coin && p2Done) {
                p1Coin++;
              }
              if(p1Coin >= NUMCOINS) {
                if(p2Coin == 0)
                  p1Coin = 1;
                else
                  p1Coin = 0;
              }
            }
          }
          break;
        
          case BUTTON_A:
            if(event.pressed) {
              
              /* pressing A shouldn't do anything if were waiting */
              if(selectedButton < 2){
                p1Buttons[selectedButton]->onAccept();
                if(selectedButton == 0) { //set the players color, disable color for other player
                  p1Buttons[selectedButton]->deselect();
                  selectedButton++;
                  p1Buttons[selectedButton]->select();
                }
                
                /* player 1 has confirmed that they're ready */
                else if(selectedButton == 1) {
                  p1Buttons[selectedButton]->deselect();
                  selectedButton++;
                  p1Done = true;
                  
                  /*  player 1 has the same chip highlighted, 
                      highlight a new chip for player 1 */
                  if(p1Coin == p2Coin)
                    p2Coin = p1Coin == 0 ? 1 : 0;
                  
                  /*  player 2 either hasn't registered, or is done, 
                      time to bootstrap into the game */
                  if(p2Coin == -1 || p2Done) {
                    
                    /* ai player, select random chip */
                    if(p2Coin == -1) {
                      int p2;
                      do {
                        p2 = rand() % NUMCOINS;
                      } while (p2 == p1Coin);
                      getGameModel().setPlayerColors(colors[p1Coin], colors[p2]);
                      
                      getGameModel().getPlayersContainer()->pop_back();
                      switch(getGameModel().getDifficulty()) {
                        case 0: {
                          EasyAgent* play2 = new EasyAgent(1);
                          getGameModel().getPlayersContainer()->push_back(play2);
                          break;
                        }
                        case 1: { 
                          MediumAgent* play2 = new MediumAgent(1);
                          getGameModel().getPlayersContainer()->push_back(play2);
                          break;
                        }
                        case 2: {
                          HardAgent* play2 = new HardAgent(1);
                          getGameModel().getPlayersContainer()->push_back(play2);
                          break;
                        }
                        default: {
                          MediumAgent* play2 = new MediumAgent(1);
                          getGameModel().getPlayersContainer()->push_back(play2);
                          break;
                        }
                      }
                    }
                    
                    /* carry on */
                    else {
                      getGameModel().getPlayersContainer()->pop_back();
                      Player* play2 = new Player(1);
                      getGameModel().getPlayersContainer()->push_back(play2);
                      getGameModel().setPlayerColors(colors[p1Coin], colors[p2Coin]);
                    }
                    
                    /* TIME TO PLAY! */
                    Zeni::get_Game().push_state(new Tutorial_State());
                    reset();
                  }
                }
              }
            }
            break;
          }
        break;
        
      /* Wiimote 1 */
      case 1:
        switch(event.button) {
          case BUTTON_B:
            if(event.pressed) {
              if(p2SelectedButton > 1) {
                p2Done = false;
                p2SelectedButton--;
                p2Buttons[p2SelectedButton]->select();
              }
              else if(p2SelectedButton == 1) {
                p2Buttons[p2SelectedButton]->deselect();
                p2SelectedButton--;
                p2Buttons[p2SelectedButton]->select();
              }
              else
                Zeni::get_Game().pop_state();
            }
            break;
            
          case BUTTON_LEFT:
            if(event.pressed) {
              if(p2SelectedButton == 0) {
                p2Coin--;
                if(p1Coin == p2Coin && p1Done) {
                  p2Coin--;
                }
                if(p2Coin < 0) {
                  if(p1Coin == NUMCOINS-1)
                    p2Coin = NUMCOINS-2;
                  else
                    p2Coin = NUMCOINS-1;
                }
              }
            }
            break;
            
          case BUTTON_RIGHT:
            if(event.pressed) {
              if(p2SelectedButton == 0) {
                p2Coin++;
                if(p1Coin == p2Coin && p1Done) {
                  p2Coin++;
                }
                if(p2Coin >= NUMCOINS) {
                  if(p1Coin == 0)
                    p2Coin = 1;
                  else
                    p2Coin = 0;
                }
              }
            }
            break;
            
          case BUTTON_A:
            if(event.pressed) {
              if(p2SelectedButton == -1) {
                p2SelectedButton = 0;
                p2Coin = p1Coin == 0 ? 1 : 0;
                p2Buttons[p2SelectedButton]->select();
              }
              else if(p2SelectedButton < 2){
                p2Buttons[p2SelectedButton]->onAccept();
                if(p2SelectedButton == 0) { //set the players color, disable color for other player
                  p2Buttons[p2SelectedButton]->deselect();
                  p2SelectedButton++;
                  p2Buttons[p2SelectedButton]->select();
                }
                else if(p2SelectedButton == 1) {
                  p2Buttons[p2SelectedButton]->deselect();
                  p2SelectedButton++;
                  p2Done = true;
                  if(p1Coin == p2Coin)
                    p1Coin = p2Coin == 0 ? 1 : 0;
                  if(p1Done) {
                    getGameModel().getPlayersContainer()->pop_back();
                    Player* play2 = new Player(1);
                    getGameModel().getPlayersContainer()->push_back(play2);
                    
                    getGameModel().setPlayerColors(colors[p1Coin], colors[p2Coin]);
                    Zeni::get_Game().push_state(new Tutorial_State());
                    reset();
                  }
                }
              }
            }
            break;
        }
        break;
    }
  }
  
  /*  This is set up so both players can use the keyboard, if
   *  they wish. Player 0 uses the arrows, return/space, and esc.
   *  Player 1 uses 'a' and 'd' for arrows, 'q' for accept, and
   *  'x' for deny/back.
   */
  void on_key(const SDL_KeyboardEvent &event) {
    Wiimote_Button_Event fakeEvent;
    fakeEvent.wiimote = KEYBOARD_CONTROL; /*  assume it's player 1 */
    
    switch(event.keysym.sym) {
      case SDLK_q:
        fakeEvent.wiimote = 1;
      case SDLK_ESCAPE:
        if(event.state == SDL_PRESSED)
          fakeEvent.button = BUTTON_B;
        break;
        
      case SDLK_d:
        fakeEvent.wiimote = 1;
      case SDLK_RIGHT:
        if(event.state == SDL_PRESSED) {
          fakeEvent.button = BUTTON_RIGHT;
        }
        break;
        
      case SDLK_a:
        fakeEvent.wiimote = 1;
      case SDLK_LEFT:
        if(event.state == SDL_PRESSED) {
          fakeEvent.button = BUTTON_LEFT;
        }
        break;
        
      case SDLK_x:
        fakeEvent.wiimote = 1;
      case SDLK_RETURN:
      case SDLK_SPACE:
        if(event.state == SDL_PRESSED) {
          fakeEvent.button = BUTTON_A;
        }
        break;
    }
    
    fakeEvent.pressed = event.type == SDL_KEYDOWN;
    on_wiimote_button(fakeEvent);
  }
  
  void perform_logic() {
    MenuState::perform_logic();
    
    /* logic for spinning the coins */
    float deltaT = chrono.seconds() - timePassed;
    timePassed = chrono.seconds();
    for(int i = 0; i < NUMCOINS; i++) {
      float velocity = 2.0f * Zeni::pi; // rotate 2pi/s
      float currentTheta = coins[i]->get_rotate().second;
      coins[i]->set_rotate(currentTheta + velocity * deltaT, Zeni::Vector3f(0.0f, 0.0f, 1.0f));
    }
    
    /*  have to call performLogic() on the buttons individually
        since were using our own containers instead of the one
        provided by MenuState
    */
    for(int i = 0; i < p1Buttons.size(); i++)
      p1Buttons[i]->performLogic();
    
    for(int i = 0; i < p2Buttons.size(); i++)
      p2Buttons[i]->performLogic();
  }
  
  void render() {
    MenuState::render();
    
    /* Title */
    Zeni::Font &title = Zeni::get_Fonts()["system100"];
    title.render_text("Pick a Color!",
                      Zeni::Point2f(400.0f, 50.0f - 0.5f * title.get_text_height()),
                      Zeni::OFFWHITE,
                      Zeni::ZENI_CENTER);
    
    /* Player 1 */
    Zeni::Font &plyr = Zeni::get_Fonts()["system30"];
    plyr.render_text("Player 1",
                      Zeni::Point2f(218.0f, 185.0f - 0.5f * title.get_text_height()),
                      Zeni::OFFWHITE,
                      Zeni::ZENI_CENTER);
    
    /* Player 2 */
    plyr.render_text("Player 2",
                      Zeni::Point2f(568.0f, 185.0f - 0.5f * title.get_text_height()),
                      Zeni::OFFWHITE,
                      Zeni::ZENI_CENTER);
    
    /* The boxes the coins spin in */
    render_image("registerBox", Zeni::Point2f(75.0f, 175.0f), Zeni::Point2f(587.0f, 687.0f)); //286 x 286px
    render_image("registerBox", Zeni::Point2f(425.0f, 175.0f), Zeni::Point2f(937.0f, 687.0f));
    
    /* Player 1's buttons */
    for(int i = 0; i < p2Buttons.size(); i++) {
      if(p1Coin != -1) {
        if(i == 0)
          p1Buttons[i]->render();
        else if(i == 1 && selectedButton == 1)
          p1Buttons[i]->render();
        else if(selectedButton > 1)
          plyr.render_text("Waiting",
                           Zeni::Point2f(218.0f, 500.0f),
                           Zeni::OFFWHITE,
                           Zeni::ZENI_CENTER);
      }
    }
    
    /* Player 2's buttons */
    for(int i = 0; i < p2Buttons.size(); i++) {
      if(p2Coin != -1) {
        if(i == 0)
          p2Buttons[i]->render();
        else if(i == 1 && p2SelectedButton == 1)
          p2Buttons[i]->render();
        else if(p2SelectedButton > 1)
          plyr.render_text("Waiting",
                           Zeni::Point2f(568.0f, 500.0f),
                           Zeni::OFFWHITE,
                           Zeni::ZENI_CENTER);
      }
      else {
        plyr.render_text("Press A to join.",
                         Zeni::Point2f(568.0f, 295.0f),
                         Zeni::OFFWHITE,
                         Zeni::ZENI_CENTER);
      }
    }
    
    /* Render the spinning coin for player 2 if necc. */
    if(p2Coin != -1) {
      Zeni::Camera cam;
      cam.near_clip = 5.0f;
      Zeni::get_Video().set_3d(cam);
      coins[p2Coin]->set_scale(Zeni::Vector3f(1.5f,1.5f,1.5f));
      coins[p2Coin]->set_translate(Zeni::Point3f(8.0f, -4.5f, -0.5f));
      coins[p2Coin]->render();
      Zeni::get_Video().set_2d();
    }
    
    /* Render the spinning coin for player 1 */
    Zeni::Camera cam;
    cam.near_clip = 5.0f;
    Zeni::get_Video().set_3d(cam);
    coins[p1Coin]->set_scale(Zeni::Vector3f(1.5f,1.5f,1.5f));
    coins[p1Coin]->set_translate(Zeni::Point3f(8.0f, 4.75f, -0.5f));
    coins[p1Coin]->render();
    Zeni::get_Video().set_2d();
  }
  
private:
  int p1Coin, p2Coin;             // <--coin identifier, used to index into coins[] and colors[]
  Zeni::Model* coins[NUMCOINS];   // <--contains the coin models for displaying
  int p2SelectedButton;           // <--the button player 2 has currently selected (-1 if none)
  bool p1Done, p2Done;            // <--whether p1 or p2 is finished choosing their color      
  float timePassed;               // <--timestamp of the last iteration through performLogic
  Zeni::Chronometer<Zeni::Time> chrono; // <--keep track of time
  std::vector<MenuButton*> p1Buttons;   // <--container for player 1's buttons
  std::vector<MenuButton*> p2Buttons;   // <--container for player 2's buttons
  std::string colors[NUMCOINS];   // <--container of color names, used for assigning colors to GameModel
};

#endif

