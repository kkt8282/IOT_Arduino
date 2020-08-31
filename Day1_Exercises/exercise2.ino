#include <SoftPWM.h>
SOFTPWM_DEFINE_CHANNEL(A3);
#include <U8g2lib.h>
#include "DHT.h"
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

#define DHTPIN A1
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

float Temp;
int PWM;

void setup() {
     SoftPWM.begin(490);
     dht.begin();
     u8g2.begin();
}

void loop() {
  Temp = dht.readTemperature();
  if(isnan(Temp)) {
    Serial.println(F("Falied to read from DHT sensor!"));
    return;
  }
  if (Temp >= 30) {
    SoftPWM.set(100);
    PWM = 100;
  }
  else if (Temp <= 25) {
    SoftPWM.set(0);
    PWM = 0;
  }
  else {
    SoftPWM.set(65);
    PWM = 65;
  }
  OLEDdraw();
}
void OLEDdraw() {
  u8g2.clearBuffer();

  u8g2.setFont(u8g2_font_ncenB08_te);
  u8g2.drawStr(1, 15, "DC FAN");

  u8g2.drawStr(1, 36, "Temp.");
  u8g2.setCursor(85, 36);
  u8g2.print(Temp);
  u8g2.drawStr(114, 36, "\xb0");
  u8g2.drawStr(119, 36, "C");

  u8g2.drawStr(1, 47, "DC FAN PWM");
  u8g2.setCursor(85, 47); u8g2.print(PWM);
  u8g2.drawStr(116, 47, "%");

  u8g2.sendBuffer();
}
