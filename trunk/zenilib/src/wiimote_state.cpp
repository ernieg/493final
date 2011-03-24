#include <zenilib.h>
#include <wiimote_state.h>
#include "wiimote_events.h"

using namespace std;
using namespace Zeni;

int rumble_state[MAX_WIIMOTES];
//Zeni::Mutex rumble_mutex;

Thread *Wiimote_Game_State::thread = NULL;
Mutex *Wiimote_Game_State::mutex = NULL;
bool Wiimote_Game_State::running = true;

queue<Wiimote_Button_Event> Wiimote_Game_State::queue_wiimote_button_event;
queue<Wiimote_IR_Event> Wiimote_Game_State::queue_wiimote_ir_event;
queue<Wiimote_Nunchuk_Event> Wiimote_Game_State::queue_wiimote_nunchuk_event;

void Wiimote_Game_State::perform_logic(){
    mutex->lock();
    while(!queue_wiimote_button_event.empty()){
        Wiimote_Button_Event event = queue_wiimote_button_event.front();
        queue_wiimote_button_event.pop();
        mutex->unlock();
        on_wiimote_button(event);
        mutex->lock();
    }

    while(!queue_wiimote_ir_event.empty()){
        Wiimote_IR_Event event = queue_wiimote_ir_event.front();
        queue_wiimote_ir_event.pop();
        mutex->unlock();
        on_wiimote_ir(event);
        mutex->lock();
    }

    while(!queue_wiimote_nunchuk_event.empty()){
        Wiimote_Nunchuk_Event event = queue_wiimote_nunchuk_event.front();
        queue_wiimote_nunchuk_event.pop();
        mutex->unlock();
        on_wiimote_nunchuk(event);
        mutex->lock();
    }
    mutex->unlock();
}

void Wiimote_Game_State::process_event(Wiimote_State &wiimote_state, wiimote_t *wiimote, int wiimote_id){
    //TODO dont do anything if paused

    // wiimote buttons
    if(!wiimote_state.buttons[BUTTON_A] && IS_PRESSED(wiimote, WIIMOTE_BUTTON_A)){ 
        wiimote_state.buttons[BUTTON_A] = true;
        //on_wiimote_button(Wiimote_Button_Event(true, wiimote_id, BUTTON_A));
        mutex->lock();
        queue_wiimote_button_event.push(Wiimote_Button_Event(true, wiimote_id, BUTTON_A));
        mutex->unlock();
    }else if(wiimote_state.buttons[BUTTON_A] && IS_RELEASED(wiimote, WIIMOTE_BUTTON_A)){ 
        wiimote_state.buttons[BUTTON_A] = false;
        //on_wiimote_button(Wiimote_Button_Event(false, wiimote_id, BUTTON_A));
        mutex->lock();
        queue_wiimote_button_event.push(Wiimote_Button_Event(false, wiimote_id, BUTTON_A));
        mutex->unlock();
    }

    if(!wiimote_state.buttons[BUTTON_B] && IS_PRESSED(wiimote, WIIMOTE_BUTTON_B)){ 
        wiimote_state.buttons[BUTTON_B] = true;
        //on_wiimote_button(Wiimote_Button_Event(true, wiimote_id, BUTTON_B));
        mutex->lock();
        queue_wiimote_button_event.push(Wiimote_Button_Event(true, wiimote_id, BUTTON_B));
        mutex->unlock();
    }else if( wiimote_state.buttons[BUTTON_B] && IS_RELEASED(wiimote, WIIMOTE_BUTTON_B)){ 
        wiimote_state.buttons[BUTTON_B] = false;
        //on_wiimote_button(Wiimote_Button_Event(false, wiimote_id, BUTTON_B));
        mutex->lock();
        queue_wiimote_button_event.push(Wiimote_Button_Event(false, wiimote_id, BUTTON_B));
        mutex->unlock();
    }

    if(!wiimote_state.buttons[BUTTON_ONE] && IS_PRESSED(wiimote, WIIMOTE_BUTTON_ONE)){ 
        wiimote_state.buttons[BUTTON_ONE] = true;
        //on_wiimote_button(Wiimote_Button_Event(true, wiimote_id, BUTTON_ONE));
        mutex->lock();
        queue_wiimote_button_event.push(Wiimote_Button_Event(true, wiimote_id, BUTTON_ONE));
        mutex->unlock();
    }else if(wiimote_state.buttons[BUTTON_ONE] && IS_RELEASED(wiimote, WIIMOTE_BUTTON_ONE)){ 
        wiimote_state.buttons[BUTTON_ONE] = false;
        //on_wiimote_button(Wiimote_Button_Event(false, wiimote_id, BUTTON_ONE));
        mutex->lock();
        queue_wiimote_button_event.push(Wiimote_Button_Event(false, wiimote_id, BUTTON_ONE));
        mutex->unlock();
    }
    
    if(!wiimote_state.buttons[BUTTON_TWO] && IS_PRESSED(wiimote, WIIMOTE_BUTTON_TWO)){ 
        wiimote_state.buttons[BUTTON_TWO] = true;
        //on_wiimote_button(Wiimote_Button_Event(true, wiimote_id, BUTTON_TWO));
        mutex->lock();
        queue_wiimote_button_event.push(Wiimote_Button_Event(true, wiimote_id, BUTTON_TWO));
        mutex->unlock();
    }else if(wiimote_state.buttons[BUTTON_TWO] && IS_RELEASED(wiimote, WIIMOTE_BUTTON_TWO)){ 
        wiimote_state.buttons[BUTTON_TWO] = false;
        //on_wiimote_button(Wiimote_Button_Event(false, wiimote_id, BUTTON_TWO));
        mutex->lock();
        queue_wiimote_button_event.push(Wiimote_Button_Event(false, wiimote_id, BUTTON_TWO));
        mutex->unlock();
    }

    if(!wiimote_state.buttons[BUTTON_MINUS] && IS_PRESSED(wiimote, WIIMOTE_BUTTON_MINUS)){ 
        wiimote_state.buttons[BUTTON_MINUS] = true;
        //on_wiimote_button(Wiimote_Button_Event(true, wiimote_id, BUTTON_MINUS));
        mutex->lock();
        queue_wiimote_button_event.push(Wiimote_Button_Event(true, wiimote_id, BUTTON_MINUS));
        mutex->unlock();
    }else if(wiimote_state.buttons[BUTTON_MINUS] && IS_RELEASED(wiimote, WIIMOTE_BUTTON_MINUS)){ 
        wiimote_state.buttons[BUTTON_MINUS] = false;
        //on_wiimote_button(Wiimote_Button_Event(false, wiimote_id, BUTTON_MINUS));
        mutex->lock();
        queue_wiimote_button_event.push(Wiimote_Button_Event(false, wiimote_id, BUTTON_MINUS));
        mutex->unlock();
    }

    if(!wiimote_state.buttons[BUTTON_PLUS] && IS_PRESSED(wiimote, WIIMOTE_BUTTON_PLUS)){ 
        wiimote_state.buttons[BUTTON_PLUS] = true;
        //on_wiimote_button(Wiimote_Button_Event(true, wiimote_id, BUTTON_PLUS));
        mutex->lock();
        queue_wiimote_button_event.push(Wiimote_Button_Event(true, wiimote_id, BUTTON_PLUS));
        mutex->unlock();
    }else if(wiimote_state.buttons[BUTTON_PLUS] && IS_RELEASED(wiimote, WIIMOTE_BUTTON_PLUS)){ 
        wiimote_state.buttons[BUTTON_PLUS] = false;
        //on_wiimote_button(Wiimote_Button_Event(false, wiimote_id, BUTTON_PLUS));
        mutex->lock();
        queue_wiimote_button_event.push(Wiimote_Button_Event(false, wiimote_id, BUTTON_PLUS));
        mutex->unlock();
    }

    if(!wiimote_state.buttons[BUTTON_HOME] && IS_PRESSED(wiimote, WIIMOTE_BUTTON_HOME)){ 
        wiimote_state.buttons[BUTTON_HOME] = true;
        //on_wiimote_button(Wiimote_Button_Event(true, wiimote_id, BUTTON_HOME));
        mutex->lock();
        queue_wiimote_button_event.push(Wiimote_Button_Event(true, wiimote_id, BUTTON_HOME));
        mutex->unlock();
    }else if(wiimote_state.buttons[BUTTON_HOME] && IS_RELEASED(wiimote, WIIMOTE_BUTTON_HOME)){ 
        wiimote_state.buttons[BUTTON_HOME] = false;
        //on_wiimote_button(Wiimote_Button_Event(false, wiimote_id, BUTTON_HOME));
        mutex->lock();
        queue_wiimote_button_event.push(Wiimote_Button_Event(false, wiimote_id, BUTTON_HOME));
        mutex->unlock();
    }

    if(!wiimote_state.buttons[BUTTON_UP] && IS_PRESSED(wiimote, WIIMOTE_BUTTON_UP)){ 
        wiimote_state.buttons[BUTTON_UP] = true;
        //on_wiimote_button(Wiimote_Button_Event(true, wiimote_id, BUTTON_UP));
        mutex->lock();
        queue_wiimote_button_event.push(Wiimote_Button_Event(true, wiimote_id, BUTTON_UP));
        mutex->unlock();
    }else if(wiimote_state.buttons[BUTTON_UP] && IS_RELEASED(wiimote, WIIMOTE_BUTTON_UP)){ 
        wiimote_state.buttons[BUTTON_UP] = false;
        //on_wiimote_button(Wiimote_Button_Event(false, wiimote_id, BUTTON_UP));
        mutex->lock();
        queue_wiimote_button_event.push(Wiimote_Button_Event(false, wiimote_id, BUTTON_UP));
        mutex->unlock();
    }

    if(!wiimote_state.buttons[BUTTON_DOWN] && IS_PRESSED(wiimote, WIIMOTE_BUTTON_DOWN)){ 
        wiimote_state.buttons[BUTTON_DOWN] = true;
        //on_wiimote_button(Wiimote_Button_Event(true, wiimote_id, BUTTON_DOWN));
        mutex->lock();
        queue_wiimote_button_event.push(Wiimote_Button_Event(true, wiimote_id, BUTTON_DOWN));
        mutex->unlock();
    }else if(wiimote_state.buttons[BUTTON_DOWN] && IS_RELEASED(wiimote, WIIMOTE_BUTTON_DOWN)){ 
        wiimote_state.buttons[BUTTON_DOWN] = false;
        //on_wiimote_button(Wiimote_Button_Event(false, wiimote_id, BUTTON_DOWN));
        mutex->lock();
        queue_wiimote_button_event.push(Wiimote_Button_Event(false, wiimote_id, BUTTON_DOWN));
        mutex->unlock();
    }

    if(!wiimote_state.buttons[BUTTON_LEFT] && IS_PRESSED(wiimote, WIIMOTE_BUTTON_LEFT)){ 
        wiimote_state.buttons[BUTTON_LEFT] = true;
        //on_wiimote_button(Wiimote_Button_Event(true, wiimote_id, BUTTON_LEFT));
        mutex->lock();
        queue_wiimote_button_event.push(Wiimote_Button_Event(true, wiimote_id, BUTTON_LEFT));
        mutex->unlock();
    }else if(wiimote_state.buttons[BUTTON_LEFT] && IS_RELEASED(wiimote, WIIMOTE_BUTTON_LEFT)){ 
        wiimote_state.buttons[BUTTON_LEFT] = false;
        //on_wiimote_button(Wiimote_Button_Event(false, wiimote_id, BUTTON_LEFT));
        mutex->lock();
        queue_wiimote_button_event.push(Wiimote_Button_Event(false, wiimote_id, BUTTON_LEFT));
        mutex->unlock();
    }

    if(!wiimote_state.buttons[BUTTON_RIGHT] && IS_PRESSED(wiimote, WIIMOTE_BUTTON_RIGHT)){ 
        wiimote_state.buttons[BUTTON_RIGHT] = true;
        //on_wiimote_button(Wiimote_Button_Event(true, wiimote_id, BUTTON_RIGHT));
        mutex->lock();
        queue_wiimote_button_event.push(Wiimote_Button_Event(true, wiimote_id, BUTTON_RIGHT));
        mutex->unlock();
    }else if(wiimote_state.buttons[BUTTON_RIGHT] && IS_RELEASED(wiimote, WIIMOTE_BUTTON_RIGHT)){ 
        wiimote_state.buttons[BUTTON_RIGHT] = false;
        //on_wiimote_button(Wiimote_Button_Event(false, wiimote_id, BUTTON_RIGHT));
        mutex->lock();
        queue_wiimote_button_event.push(Wiimote_Button_Event(false, wiimote_id, BUTTON_RIGHT));
        mutex->unlock();
    }

    // nunchuck
    if(wiimote->exp.type == EXP_NUNCHUK){
        nunchuk_t *nunchuk = (nunchuk_t*) &wiimote->exp.nunchuk;

        // buttons
        if(!wiimote_state.buttons[BUTTON_C] && IS_PRESSED(nunchuk, NUNCHUK_BUTTON_C)){ 
            wiimote_state.buttons[BUTTON_C] = true;
            //on_wiimote_button(Wiimote_Button_Event(true, wiimote_id, BUTTON_C));
            mutex->lock();
            queue_wiimote_button_event.push(Wiimote_Button_Event(true, wiimote_id, BUTTON_C));
            mutex->unlock();
        }else if(wiimote_state.buttons[BUTTON_C] && IS_RELEASED(nunchuk, NUNCHUK_BUTTON_C)){ 
            wiimote_state.buttons[BUTTON_C] = false;
            //on_wiimote_button(Wiimote_Button_Event(false, wiimote_id, BUTTON_C));
            mutex->lock();
            queue_wiimote_button_event.push(Wiimote_Button_Event(false, wiimote_id, BUTTON_C));
            mutex->unlock();
        }

        if(!wiimote_state.buttons[BUTTON_Z] && IS_PRESSED(nunchuk, NUNCHUK_BUTTON_Z)){ 
            wiimote_state.buttons[BUTTON_Z] = true;
            //on_wiimote_button(Wiimote_Button_Event(true, wiimote_id, BUTTON_Z));
            mutex->lock();
            queue_wiimote_button_event.push(Wiimote_Button_Event(true, wiimote_id, BUTTON_Z));
            mutex->unlock();
        }else if(wiimote_state.buttons[BUTTON_Z] && IS_RELEASED(nunchuk, NUNCHUK_BUTTON_Z)){ 
            wiimote_state.buttons[BUTTON_Z] = false;
            //on_wiimote_button(Wiimote_Button_Event(false, wiimote_id, BUTTON_Z));
            mutex->lock();
            queue_wiimote_button_event.push(Wiimote_Button_Event(false, wiimote_id, BUTTON_Z));
            mutex->unlock();
        }

        // joystick
        float mag = nunchuk->js.mag;
        if(mag < JOYSTICK_THRESHOLD){
            mag = 0.0f;
        }else if(mag > 1.0f){
            mag = 1.0f;
        }

        if(nunchuk->js.ang >= 0.0f && nunchuk->js.ang <= 360.0f){
            if((wiimote_state.ang != nunchuk->js.ang) || (wiimote_state.mag != mag)){
                wiimote_state.ang = (nunchuk->js.ang * pi) / 180.0f;
                wiimote_state.mag = mag;
                Wiimote_Nunchuk_Event event(wiimote_state.ang, wiimote_state.mag, wiimote_id);
                //on_wiimote_nunchuk(event);
                mutex->lock();
                queue_wiimote_nunchuk_event.push(event);
                mutex->unlock();
            }
        }
    }
    
    // ir information
    if(WIIUSE_USING_IR(wiimote)){
        Wiimote_IR_Event event(wiimote_id, wiimote->ir.x, wiimote->ir.y, wiimote->ir.x - wiimote_state.ir_x, wiimote->ir.y - wiimote_state.ir_y);
        if(event.x_rel != 0 || event.y_rel != 0){
            wiimote_state.ir_x = wiimote->ir.x;
            wiimote_state.ir_y = wiimote->ir.y;
            //on_wiimote_ir(event);
            mutex->lock();
            queue_wiimote_ir_event.push(event);
            mutex->unlock();
        }
    }
}

bool poll_wiimote(Wiimote_Game_State::Wiimote_State &state, wiimote** wiimotes, int index){
    int res = wiiuse_poll(wiimotes + index, 1);
    if(res){
        switch(wiimotes[index]->event){
            case WIIUSE_EVENT:
						    /* a generic event occured */
                Wiimote_Game_State::process_event(state, wiimotes[index], index);
						    break;
					      default:
						    break;
        }
    }
    return res > 0;
}

int wiimote_loop(void *host){
    //Wiimote_Game_State *state = (Wiimote_Game_State*) host;
    bool *ready = (bool *) host;
    Wiimote_Game_State::Wiimote_State wiimote_state[MAX_WIIMOTES];
    
    wiimote** wiimotes;
    int found, connected;

    wiimotes = wiiuse_init(MAX_WIIMOTES);
    wiiuse_set_timeout(wiimotes, MAX_WIIMOTES, 10, 255);
    found = wiiuse_find(wiimotes, MAX_WIIMOTES, 5);
    if(!found){
        cout << "No wiimotes." << endl;
        *ready = true;
        return 1;
    }

    connected = wiiuse_connect(wiimotes, MAX_WIIMOTES);
    if(!connected){
        cout << "Failed to connect" << endl;
        *ready = true;
        return 1;
    }
    

    for(int i = 0; i < connected; i++){
        wiiuse_resync(wiimotes[i]);   
        wiiuse_set_ir(wiimotes[i], 1);
        wiiuse_motion_sensing(wiimotes[i], 0);

        wiiuse_set_aspect_ratio(wiimotes[i], WIIUSE_ASPECT_16_9);
        wiiuse_set_ir_position(wiimotes[i], WIIUSE_IR_ABOVE);
        wiiuse_set_ir_vres(wiimotes[i], get_Video().get_screen_width(), get_Video().get_screen_height());
    }

    wiiuse_set_leds(wiimotes[0], WIIMOTE_LED_1);
    wiiuse_set_leds(wiimotes[1], WIIMOTE_LED_2);
    wiiuse_set_leds(wiimotes[2], WIIMOTE_LED_3);
    wiiuse_set_leds(wiimotes[3], WIIMOTE_LED_4);

    bool rumbling[MAX_WIIMOTES];
    for(int i = 0; i < MAX_WIIMOTES; i++){
        rumbling[i] = false;
    }

    *ready = true;
    while(Wiimote_Game_State::running){
        for(int i = 0; i < MAX_WIIMOTES; i++){
            //rumble_mutex.lock();
            if(rumbling[i] != rumble_state[i]){
                wiiuse_rumble(wiimotes[i], rumble_state[i]);
                rumbling[i] = rumble_state[i];
            }
            //rumble_mutex.unlock();
        }
        for(int i = 0; i < connected; i++){
            wiiuse_set_ir(wiimotes[i], 1);
            while(poll_wiimote(wiimote_state[i], wiimotes, i));
        }
    }
    wiiuse_set_leds(wiimotes[0], WIIMOTE_LED_NONE);
    wiiuse_set_leds(wiimotes[1], WIIMOTE_LED_NONE);
    wiiuse_set_leds(wiimotes[2], WIIMOTE_LED_NONE);
    wiiuse_set_leds(wiimotes[3], WIIMOTE_LED_NONE);
    wiiuse_cleanup(wiimotes, MAX_WIIMOTES);
    return 0;
}

Wiimote_Game_State::Wiimote_Game_State() { 
    if(thread == NULL){
        mutex = new Mutex();
        bool ready = false;
        thread = new Thread(wiimote_loop, &ready);
        while(!ready){
            Sleep(50);
        }
    }
}

Wiimote_Game_State::~Wiimote_Game_State(){ }

void Wiimote_Game_State::on_wiimote_button(const Wiimote_Button_Event &event){ event.button; }

void Wiimote_Game_State::on_wiimote_ir(const Wiimote_IR_Event &event){ event.x; }

void Wiimote_Game_State::on_wiimote_nunchuk(const Wiimote_Nunchuk_Event &event){ event.angle; }
