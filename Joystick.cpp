#include <Arduino.h>

#include "Joystick.h"

Joystick::Joystick(Screen *screen) 
{
  _screen = screen;

  // Debounce
  interrupt_time1 = 0;
  interrupt_time2 = 0;  
  last_interrupt_time1 = 0;
  last_interrupt_time2 = 0;
  
  // Calibrate
  buttonFlag = false; 
  joyX = 0;
  xValid = true;
  joyY = 0;  
  yValid = false;
};

bool Joystick::init()
{
  pinMode(2, INPUT_PULLUP);
  joyX = analogRead(A0); 
  joyY = analogRead(A1);

  return true;
};

void Joystick::read()
{
  int currentX = analogRead(A0);
  int SW_state = digitalRead(2);

   if(currentX > (joyX - 100) && currentX < (joyX + 100)){
    interrupt_time1 = millis();
    if (interrupt_time1 - last_interrupt_time1 > 200) {  // debounce delay   
      xValid = true;
    }
    last_interrupt_time1 = interrupt_time1;           
  }

  // <- LEFT
  if(xValid == true){
    if(currentX < (joyX - 100)) { 
      xValid = false;
      _screen->prevScreen();
      resetFlags();
    }
  }

  // -> RIGHT
  if(xValid == true){
    if(currentX > (joyX + 100)) {
      xValid = false;
      _screen->nextScreen();
      resetFlags();     
    }     
  }

  // BUTTON
  if (!SW_state) {
    _screen->nextScreen();
    resetFlags(); 
  }
};

void Joystick::resetFlags()
{

}