/*
  Screen Class
*/
#include <Arduino.h>
#include <RTClib.h>
#include <Time.h>
#include <U8g2lib.h>

#include "HttpClient.h"

#ifndef SCREEN_H_
#define SCREEN_H_

class Screen {
 public:
  Screen(int screenId, U8G2_SSD1306_128X64_NONAME_1_HW_I2C *u8g2,
         HttpClient *httpClient, RTC_DS3231 *RTC);
  static const int MAX_SCREENS = 5;
  void renderScreen();
  void prevScreen();
  void nextScreen();

 private:
  int _screenId;
  U8G2_SSD1306_128X64_NONAME_1_HW_I2C *_u8g2;
  HttpClient *_httpClient;
  RTC_DS3231 *_rtc;
  void renderMain();
  void renderClock();
  void renderTemp();
  void renderPressure();
  void renderCurrency();
  void renderWeatherCondition(int x, int y);
  void renderDate(int x, int y, DateTime now);
  void renderTime(int x, int y, DateTime now);
  String getValue(String data, char separator, int index);
  bool grabFlag;
  String currentTemp = "...";
  String currentHumidity = "...";
};

#endif  // SCREEN_H_
