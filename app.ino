#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>
#include "Screen.h"
#include "Joystick.h"

U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, A5, A4);

RTC_DS1307 RTC;
Screen screen(0, &u8g2, &RTC);
Joystick joystick(&screen);

void setup() {
  Serial.begin(9600);
  Serial.println("Iniciando pantalla OLED");
  u8g2.begin();
  joystick.init();
}

void loop() {
  u8g2.firstPage();
  do {
    screen.renderScreen();
  } while (u8g2.nextPage());

  joystick.read();
}
