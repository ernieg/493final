#ifndef WIIMOTE_STATE_H
#define WIIMOTE_STATE_H

#include <zenilib.h>
#include <wiiuse.h>
#include <queue>
#include "wiimote_events.h"

#define MAX_WIIMOTES (4)
#define JOYSTICK_THRESHOLD (0.05f)
#define KEYBOARD_CONTROL (0)

class Wiimote_Game_State : public Zeni::Gamestate_Base {
public:
    struct Wiimote_State{
        Wiimote_State() { 
          for(int i = 0; i <  NUMBER_OF_BUTTONS; i++){
            buttons[i] = false;
          } 
          ir_x = 0;
          ir_y = 0;
          mag = 0.0f;
          ang = 0.0f;
        }

        int ir_x, ir_y;
        float mag, ang;
        bool buttons[NUMBER_OF_BUTTONS];
    };

    Wiimote_Game_State();
    ~Wiimote_Game_State();

public:
    virtual void perform_logic();
    virtual void on_wiimote_button(const Wiimote_Button_Event &event);
    virtual void on_wiimote_ir(const Wiimote_IR_Event &event);
    virtual void on_wiimote_nunchuk(const Wiimote_Nunchuk_Event &event);

    static void process_event(Wiimote_State &wiimote_state, wiimote_t *wiimote, int wiimote_id);

    static bool running; //set to false during deconstructor of lowest wiimote_state
private:
    static std::queue<Wiimote_Button_Event> queue_wiimote_button_event;
    static std::queue<Wiimote_IR_Event> queue_wiimote_ir_event;
    static std::queue<Wiimote_Nunchuk_Event> queue_wiimote_nunchuk_event;

    static Zeni::Thread *thread;
    static Zeni::Mutex *mutex;
};

#endif
