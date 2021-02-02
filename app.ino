#include <SPI.h>
#include <U8g2lib.h>
#include <WiFiEsp.h>
#include <Wire.h>
#include "HttpClient.h"
#include "Joystick.h"
#include "RTClib.h"
#include "Screen.h"
#include "arduino_secrets.h"


/*
 *  Wifi Module Test Wiring
 *  Emulate Serial1 on pins 10, 11 in Arduino MEGA
 *  (not used, only for testing purposes)
 */
#include <SoftwareSerial.h>
SoftwareSerial ESPSerial(10, 11);  // RX | TX

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
int status = WL_IDLE_STATUS;
char host[] = TARGET_HOST;
int port = 80;

RTC_DS3231 RTC;
WiFiEspClient wifi;
HttpClient client(&wifi);
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0);
Screen screen(0, &u8g2, &client, &RTC);
Joystick joystick(&screen);

void setup() {
  Serial.begin(115200);
  Serial.println("Iniciando pantalla OLED...");

  u8g2.begin();
  u8g2.firstPage();
  do {
    splash();
  } while (u8g2.nextPage());

  Serial.println("Iniciando Modulo RTC...");
  RTC.begin();
  if (RTC.lostPower()) {
    RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // rtc.adjust(DateTime(2016, 1, 21, 3, 0, 0));
  }

  Serial.println("Iniciando Red...");
  beginNetwork();
  Serial.println("Iniciando Joystick...");
  joystick.init();
}

void loop() {
  u8g2.firstPage();
  do {
    screen.renderScreen();
  } while (u8g2.nextPage());
  joystick.read();
}

bool beginNetwork() {
  // ESPSerial.begin(9600);
  Serial1.begin(9600);
  WiFi.init(&Serial1);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    Serial.println(WiFi.status());
    while (WiFi.status() == WL_NO_SHIELD)
      ;
  }

  while (status != WL_CONNECTED) {
    Serial.print("Trying to connect to WPA SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("You're connected to the network");
  return true;
}

void splash() {
  u8g2.setFont(u8g2_font_profont12_mf);
  u8g2.drawStr(7, 10, "Joystick Controlled");
  u8g2.drawStr(21, 30, "Multi-Weather Station");
}
