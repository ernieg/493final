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

namespace Zeni {

  template <typename PLAY_STATE, typename INSTRUCTIONS_STATE>
  class Title_State : public Widget_Gamestate {
    Title_State(const Title_State<PLAY_STATE, INSTRUCTIONS_STATE> &);
    Title_State<PLAY_STATE, INSTRUCTIONS_STATE> & operator=(const Title_State<PLAY_STATE, INSTRUCTIONS_STATE> &);

  public:
    class Play : public Text_Button {
      Play(const Play &);
      Play & operator=(const Play &);
      
    public:
      Play()
      : Text_Button(Point2f(200.0f, 250.0f), Point2f(600.0f, 310.0f), "system_36_800x600", "Play"), timePassed(0.0f),
      hovering(false), exhaling(false), text("Play"), col(Zeni::OFFWHITE), font("system30")
      {
        give_Renderer(new Widget_Renderer_Tricolor(TRANSPARENT, TRANSPARENT, TRANSPARENT,
                                 OFFWHITE, OFFYELLOW, OFFYELLOW));
        chrono.start();
      }
      
      void on_accept() {
        Zeni::get_Sound_Source_Pool().play_and_destroy(new Zeni::Sound_Source(Zeni::get_Sounds()["beep"]));
        get_Game().push_state(new PLAY_STATE());
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
    } playButton;


    class Instructions_Button : public Text_Button {
      Instructions_Button(const Instructions_Button &);
      Instructions_Button & operator=(const Instructions_Button &);

    public:
      Instructions_Button()
        : Text_Button(Point2f(200.0f, 330.0f), Point2f(600.0f, 390.0f),
                      "system_36_800x600", "Instructions"), timePassed(0.0f),
      hovering(false), exhaling(false), text("Instructions"), col(Zeni::OFFWHITE), font("system30")
      {
        give_Renderer(new Widget_Renderer_Tricolor(TRANSPARENT, TRANSPARENT, TRANSPARENT,
                                                   OFFWHITE, OFFYELLOW, OFFYELLOW));
        chrono.start();
      }
      
      void on_accept() {
        Zeni::get_Sound_Source_Pool().play_and_destroy(new Zeni::Sound_Source(Zeni::get_Sounds()["beep"]));
        get_Game().push_state(new INSTRUCTIONS_STATE());
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
    } instructions_button;

    class Quit_Button : public Text_Button {
      Quit_Button(const Quit_Button &);
      Quit_Button & operator=(const Quit_Button &);
      
    public:
      Quit_Button()
      : Text_Button(Point2f(200.0f, 410.0f), Point2f(600.0f, 470.0f),
                    "system_36_800x600", "Quit"), timePassed(0.0f),
      hovering(false), exhaling(false), text("Quit"), col(Zeni::OFFWHITE), font("system30")
      {
        give_Renderer(new Widget_Renderer_Tricolor(TRANSPARENT, TRANSPARENT, TRANSPARENT,
                                                   OFFWHITE, OFFYELLOW, OFFYELLOW));
        chrono.start();
      }
      
      void on_accept() {
        Zeni::get_Sound_Source_Pool().play_and_destroy(new Zeni::Sound_Source(Zeni::get_Sounds()["beep"]));
        throw Quit_Event();
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
    } quit_button;

    Title_State(const std::string &title_)
      : Widget_Gamestate(std::make_pair(Point2f(0.0f, 0.0f), Point2f(800.0f, 600.0f))),
      chip(Model("models/redChip.3DS")),
      proj(Projector2D())
    {
      m_widgets.lend_Widget(playButton);
      m_widgets.lend_Widget(instructions_button);
      m_widgets.lend_Widget(quit_button);

      get_Video().set_clear_color(get_Colors()["title_bg"]);
    }

    ~Title_State() {
      get_Video().set_clear_color(Color(1.0f, 0.0f, 0.0f, 0.0f));
    }

    void on_key(const SDL_KeyboardEvent &/*event*/)
    {
    }

    void perform_logic() {
      Widget_Gamestate::perform_logic();
      get_Video().set_clear_color(get_Colors()["title_bg"]);
      chip.set_translate(proj.unproject(Point3f(20.0f, 20.0f, 0.0f)));
      chip.set_rotate(chip.get_rotate().second + pi/100, Vector3f(1.0f, 0.0f, 0.0f));
      chip.set_scale(Vector3f(4.0f, 4.0f, 4.0f));
      
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
      //Video &vid = get_Video();
      Zeni::render_image("TitleBG", Zeni::Point2f(0.0f, 0.0f), Zeni::Point2f(1024.0f, 1024.0f));
      for(int i = 0; i < glows.size(); i++) 
        Zeni::render_image("glow", Zeni::Point2f(glows[i].x, glows[i].y), Zeni::Point2f(glows[i].x + 64, glows[i].y + 128), false, Color((float)((float)glows[i].y/600.0f), 1.0f, 1.0f, 1.0f));
      //vid.set_3d(Camera());
      //chip.render();
      //vid.set_2d();
      Widget_Gamestate::render();
    }
    
  private:
    Zeni::Model chip;
    Projector2D proj;
    std::vector<Zeni::Point2f> glows;

  };

}

#endif
