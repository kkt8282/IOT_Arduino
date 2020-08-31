#define SOILHUMI A6
#define PUMP 16
#include <U8g2lib.h>
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

int Soilhumi = 0;

void setup() {
     Serial.begin(9600);
     pinMode(PUMP, OUTPUT);
     pinMode(SOILHUMI, INPUT);
     u8g2.begin();
}

void loop() {
    Soilhumi = map(analogRead(SOILHUMI), 0, 1023, 100, 0);

    if(isnan(Soilhumi)) {
      Serial.println(F("Falied to read from SOILHUMI sensor!"));
      return;
    }
    if (Soilhumi < 30) {
      digitalWrite(PUMP, HIGH);
    }
    else if (Soilhumi > 60) {
      digitalWrite(PUMP, LOW);
    }
    OLEDdraw();
  }

  void OLEDdraw() {
  u8g2.clearBuffer();

  u8g2.setFont(u8g2_font_ncenB08_te);
  u8g2.drawStr(1, 15, "DC PUMP");

  u8g2.drawStr(1, 47, "Soilhumi");
  u8g2.setCursor(85, 47); u8g2.print(Soilhumi);
  u8g2.drawStr(116, 47, "%");

  u8g2.sendBuffer();
}
