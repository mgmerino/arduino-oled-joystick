#include "Joystick.h"
#include <Arduino.h>

Joystick::Joystick(Screen *screen) {
  _screen = screen;
}

bool Joystick::init() {
  pinMode(2, INPUT_PULLUP);
  joyX = analogRead(A0);
  joyY = analogRead(A1);

  return true;
}

void Joystick::read() {
  int currentX = analogRead(A0);
  int SW_state = digitalRead(2);

  if (currentX > (joyX - 100) && currentX < (joyX + 100)) {
    interrupt_time1 = millis();
    if (interrupt_time1 - last_interrupt_time1 > 200) {  // debounce delay
      xValid = true;
    }
    last_interrupt_time1 = interrupt_time1;
  }

  // <- LEFT
  if (xValid == true) {
    if (currentX < (joyX - 100)) {
      xValid = false;
      _screen->prevScreen();
    }
  }

  // -> RIGHT
  if (xValid == true) {
    if (currentX > (joyX + 100)) {
      xValid = false;
      _screen->nextScreen();
    }
  }

  // BUTTON
  if (!SW_state) {
    _screen->nextScreen();
  }
}
