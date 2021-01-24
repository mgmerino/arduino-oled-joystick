/*
  Screen Class
*/
#include <Arduino.h>
#include <Time.h>
#include <U8g2lib.h>
#include <RTClib.h>

#ifndef Screen_h
#define Screen_h

class Screen
{
  public:
    Screen(int screenId, U8G2_SSD1306_128X64_NONAME_1_SW_I2C *u8g2, RTC_DS1307 *RTC);
    const static int MAX_SCREENS = 4;
    void renderScreen();
    void prevScreen();
    void nextScreen();
  private:
    int _screenId;
    U8G2_SSD1306_128X64_NONAME_1_SW_I2C *_u8g2;
    RTC_DS1307 *_rtc;
    void renderMenu();
    void renderClock();
    void renderTemp();
    void renderPressure();
    bool getPCTime(); 
};

#endif
