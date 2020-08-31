#include "DHT.h"
#include <U8g2lib.h>
#include <SoftPWM.h>
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
SOFTPWM_DEFINE_CHANNEL(A3);
#define SOILHUMI A6
#define DHTPIN A1
#define DHTTYPE DHT22
#define PUMP 16
#define LAMP 17

DHT dht(DHTPIN, DHTTYPE);
uint32_t TimeCompare;
uint32_t StartTime = 0;
uint32_t TimeSum;
uint32_t DataCaptureDelay = 2000; //ms
uint32_t DataCaptureDelay2 = 3000; //ms
uint32_t DataCapture_ST = 0;
uint32_t DataCapture_ST2 = 0;

float Temp;
float Humi;

String Pump;
String Lamp;

int Pwm;
int Soilhumi = 0;
int Hour = 0;
int Minute = 1;

void setup() {
  dht.begin();
  Serial.begin(9600);
  u8g2.begin();
  DataCapture_ST = millis();
  SoftPWM.begin(490);  pinMode(SOILHUMI, INPUT);
  pinMode(PUMP, OUTPUT);
  pinMode(LAMP, OUTPUT);
  TimeSum = (uint32_t)(Hour*60 + Minute)*2*1000;
  StartTime = millis();
}

void loop() {
  TimeCompare = (millis() - StartTime) / TimeSum;
  if (TimeCompare % 2 == 0) {
    digitalWrite(LAMP, LOW);
    Lamp = "OFF";
  }
  else if (TimeCompare % 2 == 1) {
    digitalWrite(LAMP, HIGH);
    Lamp = "ON";
  }
  
  if((millis()-DataCapture_ST) > DataCaptureDelay) {
    Humi = dht.readHumidity();
    Temp = dht.readTemperature();
    if(isnan(Humi) || isnan(Temp)) {
      Serial.println(F("Falied to read from DHT sensor!"));
      return;
    }
    if (Temp >= 29) {
      SoftPWM.set(100);
      Pwm = 100;
    }
    else if (Temp <= 20) {
      SoftPWM.set(0);
      Pwm = 0;
    }
    else {
      SoftPWM.set(65);
      Pwm = 65;
    }
    OLEDdraw();
    DataCapture_ST = millis();
  }
  if((millis()-DataCapture_ST2) > DataCaptureDelay2) {
    Soilhumi = map(analogRead(SOILHUMI), 0, 1023, 100, 0);
    if(isnan(Soilhumi)) {
      Serial.println(F("Falied to read from SOILHUMI sensor!"));
      return;
    }
    if(Soilhumi >= 30 and Soilhumi <= 60) {
      digitalWrite(PUMP, HIGH);
      Pump = "ON";
    }
    else {
      digitalWrite(PUMP, LOW);
      Pump = "OFF";
    }
    OLEDdraw();
    DataCapture_ST2 = millis();
  }
  
}

void OLEDdraw() {
  u8g2.clearBuffer();

  u8g2.setFont(u8g2_font_ncenB08_te);
  u8g2.drawStr(1, 10, "Temp.");
  u8g2.setCursor(85, 10);
  u8g2.print(Temp);
  u8g2.drawStr(114, 10, "\xb0");
  u8g2.drawStr(119, 10, "C");

  u8g2.drawStr(1, 20, "Humidity.");
  u8g2.setCursor(85, 20); u8g2.print(Humi);
  u8g2.drawStr(116, 20, "%");

  u8g2.drawStr(1, 30, "Soil Humi.");
  u8g2.setCursor(85, 30); u8g2.print(Soilhumi);
  u8g2.drawStr(116, 30, "%");

  u8g2.drawStr(1, 40, "Fan PWM.");
  u8g2.setCursor(85, 40); u8g2.print(Pwm);

  u8g2.drawStr(1, 50, "Pump.");
  u8g2.setCursor(85, 50); u8g2.print(Pump);
  
  u8g2.drawStr(1, 60, "Lamp.");
  u8g2.setCursor(85, 60); u8g2.print(Lamp);

  u8g2.sendBuffer();
}
