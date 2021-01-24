#include "Screen.h"

Screen::Screen(int screenId, U8G2_SSD1306_128X64_NONAME_1_SW_I2C *u8g2, RTC_DS1307 *RTC) 
{
  _screenId = screenId;
  _u8g2 = u8g2;
  _rtc = RTC;
}

void Screen::nextScreen()
{
  int currentScreen = _screenId;
  if (currentScreen == Screen::MAX_SCREENS - 1) {
    _screenId = 0;
  } else {
    _screenId++;
  }
}

void Screen::prevScreen()
{
  int currentScreen = _screenId;
  if (currentScreen == 0) {
    _screenId = Screen::MAX_SCREENS - 1;
  } else {
    _screenId--;
  }
}

void Screen::renderScreen()
{
  switch (_screenId){
    case 0:
      renderMenu();
      break;
    case 1:
      renderClock();
      break;
    case 2:
      renderTemp();
      break;
    case 3:
      renderPressure();
      break;
  }
}

void Screen::renderMenu()
{
  _u8g2->setFont(u8g2_font_bitcasual_tu);
  _u8g2->drawStr(30, 45, "1 MAIN");
}

void Screen::renderClock()
{
  DateTime now = DateTime(19,28,50,20,10,2020);
  const char* greetingTime = "";

  // Clock face
  _u8g2->drawCircle(64, 32, 20); // main outer circle
  _u8g2->drawCircle(64, 32, 2);  // small inner circle
  //
  //hour ticks
  for( int z=0; z < 360;z= z + 30 ){
  //Begin at 0° and stop at 360°
    float angle = z ;
    angle = (angle/57.29577951) ; //Convert degrees to radians
    int x2 = (64 + (sin(angle) * 20));
    int y2 = (32 - (cos(angle) * 20));
    int x3 = (64 + (sin(angle) * (20 - 5)));
    int y3 = (32 - (cos(angle) * (20 - 5)));
    _u8g2->drawLine(x2,y2,x3,y3);
  }
  // display second hand
  float angle = now.second()*6 ;
  angle=(angle/57.29577951) ; //Convert degrees to radians  
  int x3=(64+(sin(angle)*(20)));
  int y3=(32-(cos(angle)*(20)));
  _u8g2->drawLine(64,32,x3,y3);
  //
  // display minute hand
  angle = now.minute() * 6 ;
  angle=(angle/57.29577951) ; //Convert degrees to radians  
  x3=(64+(sin(angle)*(20-3)));
  y3=(32-(cos(angle)*(20-3)));
  _u8g2->drawLine(64,32,x3,y3);
  //
  // display hour hand
  angle = now.hour() * 30 + int((now.minute() / 12) * 6 )   ;
  angle=(angle/57.29577951) ; //Convert degrees to radians  
  x3=(64+(sin(angle)*(20-11)));
  y3=(32-(cos(angle)*(20-11)));
  _u8g2->drawLine(64,32,x3,y3);
 
  if (now.hour() >= 6 && now.hour() < 12){greetingTime = " Good Morning";}
  if (now.hour() >= 12 && now.hour() < 17){greetingTime = "Good Afternoon";}
  if (now.hour() >= 17 && now.hour() <= 23){greetingTime = " Good Evening";}
  if (now.hour() >= 0 && now.hour() <= 5){greetingTime = "  Sleep well";}
  
  _u8g2->setFont(u8g_font_profont15);
  _u8g2->drawStr(20,10, greetingTime);
}

void Screen::renderTemp() {
  _u8g2->drawFrame(0,0,128,64);
  _u8g2->drawLine(19,64,19,59);
  _u8g2->drawLine(34,64,34,59);
  _u8g2->drawLine(49,64,49,59);   
  _u8g2->drawLine(64,64,64,54); // 12 hour mark
  _u8g2->drawLine(79,64,79,59);    
  _u8g2->drawLine(94,64,94,59);
  _u8g2->drawLine(109,64,109,59);
  // horizontal lines
  _u8g2->drawLine(0,48,128,48); // 10 degrees C  
  _u8g2->drawLine(0,32,128,32); // 20 degrees C 
  _u8g2->drawLine(0,16,128,16); // 30 degrees C 
}

void Screen::renderPressure() {
  _u8g2->drawFrame(0,0,128,64);
  _u8g2->drawLine(19,64,19,59);
  _u8g2->drawLine(34,64,34,59);
  _u8g2->drawLine(49,64,49,59);   
  _u8g2->drawLine(64,64,64,54); // 12 hour mark
  _u8g2->drawLine(79,64,79,59);    
  _u8g2->drawLine(94,64,94,59);
  _u8g2->drawLine(109,64,109,59);
// horizontal lines
  // the baseline is 900mb
  // the top line is 1020mb
  _u8g2->drawLine(0,42,128,42); // 1000mb  
  _u8g2->drawLine(0,21,128,21); // 1010mb 
}
