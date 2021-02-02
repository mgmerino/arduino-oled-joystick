#include "Screen.h"

Screen::Screen(int screenId, U8G2_SSD1306_128X64_NONAME_1_HW_I2C *u8g2,
               HttpClient *httpClient, RTC_DS3231 *RTC) {
  _screenId = screenId;
  _u8g2 = u8g2;
  _rtc = RTC;
  _httpClient = httpClient;
  grabFlag = true;
}

void Screen::nextScreen() {
  int currentScreen = _screenId;
  if (currentScreen == Screen::MAX_SCREENS - 1) {
    _screenId = 0;
  } else {
    _screenId++;
  }
}

void Screen::prevScreen() {
  int currentScreen = _screenId;
  if (currentScreen == 0) {
    _screenId = Screen::MAX_SCREENS - 1;
  } else {
    _screenId--;
  }
}

void Screen::renderScreen() {
  switch (_screenId) {
    case 0:
      Screen::renderMain();
      break;
    case 1:
      Screen::renderClock();
      break;
    case 2:
      Screen::renderTemp();
      break;
    case 3:
      Screen::renderPressure();
      break;
    case 4:
      Screen::renderCurrency();
      break;
  }
}

void Screen::renderMain() {
  DateTime now = _rtc->now();
  volatile int readingAge = now.minute();
  const char DEGREE_SYMBOL[] = {0xB0, '\0'};

  if (static_cast<int>((now.second() / 10) * 600) == now.second() &&
      grabFlag == true) {
    String response = _httpClient->getCurrentValues();
    currentTemp = Screen::getValue(response, ',', 0);
    currentTemp = 
        currentTemp.substring(0, currentTemp.length() - 1) + " " +
                  DEGREE_SYMBOL + "C"; 
    currentHumidity =
        Screen::getValue(response, ',', 1).substring(0, 2) + " %";
    grabFlag = false;
  }

  if (static_cast<int>(((now.second() - 1) / 10) * 10) == (now.second() - 1) &&
      grabFlag == false) {
    grabFlag = true;
  }

  _u8g2->setFont(u8g2_font_6x12_t_symbols);
  _u8g2->drawStr(10, 10, "T");
  _u8g2->drawStr(20, 10, currentTemp.c_str());
  _u8g2->drawStr(10, 25, "H");
  _u8g2->drawStr(20, 25, currentHumidity.c_str());
  _u8g2->drawStr(10, 40, "P");
  _u8g2->drawStr(20, 40, "1020 mb");

  Screen::renderWeatherCondition(80, 40);

  _u8g2->setFont(u8g2_font_6x12_t_symbols);
  Screen::renderTime(10, 60, now);
  Screen::renderDate(80, 60, now);
}

void Screen::renderDate(int x, int y, DateTime now) {
  _u8g2->setCursor(x, y);
  _u8g2->print(now.month(), DEC);
  _u8g2->print('/');
  _u8g2->print(now.day(), DEC);
  _u8g2->print('/');
  _u8g2->print(now.year(), DEC);
}

void Screen::renderTime(int x, int y, DateTime now) {
  _u8g2->setCursor(x, y);
  _u8g2->print(now.hour(), DEC);
  _u8g2->print(':');
  _u8g2->print(now.minute(), DEC);
  _u8g2->print(':');
  _u8g2->print(now.second(), DEC);
}
void Screen::renderCurrency() {
  _u8g2->setFont(u8g2_font_6x12_t_symbols);
  _u8g2->drawStr(10, 10, "1 BTC =");
  _u8g2->setFont(u8g2_font_osr21_tn);
  _u8g2->drawStr(0, 40, "28015.58");
  _u8g2->setFont(u8g2_font_osr21_tr);
  _u8g2->drawStr(65, 64, "EUR");

  _u8g2->setFont(u8g2_font_open_iconic_mime_2x_t);
  _u8g2->drawGlyph(10, 64, 74);
  _u8g2->setFont(u8g2_font_open_iconic_arrow_2x_t);
  int trendGlyph = 67; // 64 down 66 constant 67 up
  _u8g2->drawGlyph(30, 64, trendGlyph);
}

void Screen::renderWeatherCondition(int x, int y) {
  // String response = _httpClient->getWeatherCondition();
  int weatherCond = 801;

  switch (weatherCond) {
    case 800:
      _u8g2->setFont(u8g2_font_open_iconic_weather_4x_t);
      _u8g2->drawGlyph(x, y, 69);
      break;
    case 801:
      _u8g2->setFont(u8g2_font_open_iconic_weather_4x_t);
      _u8g2->drawGlyph(x, y, 65);
      break;
    case 804:
      _u8g2->setFont(u8g2_font_open_iconic_weather_4x_t);
      _u8g2->drawGlyph(x, y, 64);
      break;
    case 500:
      _u8g2->setFont(u8g2_font_open_iconic_weather_4x_t);
      _u8g2->drawGlyph(x, y, 67);
      break;
    case 200:
      _u8g2->setFont(u8g2_font_open_iconic_embedded_4x_t);
      _u8g2->drawGlyph(x, y, 67);
      break;
  }
}

void Screen::renderClock() {
  DateTime now = _rtc->now();
  const char *greetingTime = "";

  // Clock face
  // main outer circle
  _u8g2->drawCircle(64, 32, 20);
  // small inner circle
  _u8g2->drawCircle(64, 32, 2);

  // hour ticks
  for (int z = 0; z < 360; z = z + 30) {
    float angle = z;
    angle = (angle / 57.29577951);
    int x2 = (64 + (sin(angle) * 20));
    int y2 = (32 - (cos(angle) * 20));
    int x3 = (64 + (sin(angle) * (20 - 5)));
    int y3 = (32 - (cos(angle) * (20 - 5)));
    _u8g2->drawLine(x2, y2, x3, y3);
  }

  // display second hand
  float angle = now.second() * 6;
  angle = (angle / 57.29577951);
  int x3 = (64 + (sin(angle) * (20)));
  int y3 = (32 - (cos(angle) * (20)));
  _u8g2->drawLine(64, 32, x3, y3);

  // display minute hand
  angle = now.minute() * 6;
  angle = (angle / 57.29577951);
  x3 = (64 + (sin(angle) * (20 - 3)));
  y3 = (32 - (cos(angle) * (20 - 3)));
  _u8g2->drawLine(64, 32, x3, y3);

  // display hour hand
  angle = now.hour() * 30 + static_cast<int>((now.minute() / 12) * 6);
  angle = (angle / 57.29577951);
  x3 = (64 + (sin(angle) * (20 - 11)));
  y3 = (32 - (cos(angle) * (20 - 11)));
  _u8g2->drawLine(64, 32, x3, y3);

  if (now.hour() >= 6 && now.hour() < 12) {
    greetingTime = "Buenos días";
  }
  if (now.hour() >= 12 && now.hour() < 17) {
    greetingTime = "Buenas tardes";
  }
  if (now.hour() >= 17 && now.hour() <= 22) {
    greetingTime = "Buenas noches";
  }
  if (now.hour() >= 22 && now.hour() <= 5) {
    greetingTime = "Que duermas bien :)";
  }
  _u8g2->setFont(u8g2_font_profont10_mf);
  _u8g2->drawStr(30, 64, greetingTime);
}

void Screen::renderTemp() {
  _u8g2->drawFrame(0, 0, 128, 64);
  _u8g2->drawLine(19, 64, 19, 59);
  _u8g2->drawLine(34, 64, 34, 59);
  _u8g2->drawLine(49, 64, 49, 59);
  // 12 hour mark
  _u8g2->drawLine(64, 64, 64, 54);
  _u8g2->drawLine(79, 64, 79, 59);
  _u8g2->drawLine(94, 64, 94, 59);
  _u8g2->drawLine(109, 64, 109, 59);
  // horizontal lines
  // 10 degrees C
  _u8g2->drawLine(0, 48, 128, 48);
  // 20 degrees C
  _u8g2->drawLine(0, 32, 128, 32);
  // 30 degrees C
  _u8g2->drawLine(0, 16, 128, 16);
}

void Screen::renderPressure() {
  _u8g2->drawFrame(0, 0, 128, 64);
  _u8g2->drawLine(19, 64, 19, 59);
  _u8g2->drawLine(34, 64, 34, 59);
  _u8g2->drawLine(49, 64, 49, 59);
  // 12 hour mark
  _u8g2->drawLine(64, 64, 64, 54);
  _u8g2->drawLine(79, 64, 79, 59);
  _u8g2->drawLine(94, 64, 94, 59);
  _u8g2->drawLine(109, 64, 109, 59);

  // the baseline is 900mb
  // the top line is 1020mb
  // 1000mb
  _u8g2->drawLine(0, 42, 128, 42);
  // 1010mb
  _u8g2->drawLine(0, 21, 128, 21);
}

String Screen::getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}