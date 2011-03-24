#ifndef WIIMOTE_EVENTS_H
#define WIIMOTE_EVENTS_H


enum Wiimote_Button { BUTTON_A, BUTTON_B, BUTTON_ONE, BUTTON_TWO, BUTTON_MINUS, BUTTON_PLUS, BUTTON_HOME, BUTTON_UP, BUTTON_DOWN, BUTTON_LEFT, BUTTON_RIGHT, 
    BUTTON_C, BUTTON_Z, NUMBER_OF_BUTTONS};

struct Wiimote_Button_Event{
    Wiimote_Button_Event() { }
    Wiimote_Button_Event(bool pressed_, int wiimote_, Wiimote_Button button_) : pressed(pressed_), wiimote(wiimote_), button(button_) { }

    bool pressed;
    int wiimote;
    Wiimote_Button button;
};

struct Wiimote_IR_Event{
    Wiimote_IR_Event() { }
    Wiimote_IR_Event(int wiimote_, int x_, int y_, int x_rel_, int y_rel_) : wiimote(wiimote_), x(x_), y(y_), x_rel(x_rel_), y_rel(y_rel_) { }
    int wiimote;
    int x, y, x_rel, y_rel;
};

struct Wiimote_Nunchuk_Event{
    Wiimote_Nunchuk_Event() { }
    Wiimote_Nunchuk_Event(float angle_, float magnitude_, int wiimote_) : angle(angle_), magnitude(magnitude_), wiimote(wiimote_) {}
    float angle; // radians
	  float magnitude;	// 0.0f - 1.0f
    int wiimote;
};

#endif