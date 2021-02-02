/*
  Joystick Class
*/
#include <Arduino.h>

#include "Screen.h"

#ifndef JOYSTICK_H_
#define JOYSTICK_H_

class Joystick {
 public:
  Joystick(Screen *screen);
  bool init();
  void read();

 private:
  Screen *_screen;
  int joyX = 0;
  int joyY = 0;
  unsigned long interrupt_time1 = 0;
  unsigned long interrupt_time2 = 0;
  unsigned long last_interrupt_time1 = 0;
  unsigned long last_interrupt_time2 = 0;
  volatile boolean buttonFlag = false;
  boolean xValid = true;
  boolean yValid = false;
};

#endif  // JOYSTICK_H_
