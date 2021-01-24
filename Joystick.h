/*
  Joystick Class
*/
#include <Arduino.h>
#include "Screen.h"

#ifndef Joystick_h
#define Joystick_h

class Joystick
{
  public:
    Joystick(Screen *screen);
    bool init();
    void read();
  private:
    Screen *_screen;
    unsigned long interrupt_time1;
    unsigned long interrupt_time2;  
    unsigned long last_interrupt_time1;
    unsigned long last_interrupt_time2;
    volatile boolean buttonFlag;
    int joyX;
    int joyY;
    boolean xValid;
    boolean yValid;
    void resetFlags();
};

#endif
