#include <VitconBrokerComm.h>
using namespace vitcon;
#include "DHT.h"
#include <U8g2lib.h>
#include <SoftPWM.h>
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

#define SOILHUMI A6
#define DHTPIN A1
#define DHTTYPE DHT22
#define LAMP 17
#define PUMP 16
SOFTPWM_DEFINE_CHANNEL(A3);

DHT dht(DHTPIN, DHTTYPE);

int Soilhumi = 0;
float Temp;
float Humi;

IOTItemFlo dht22_temp;
IOTItemFlo dht22_humi;
IOTItemInt soilhumi;

bool fan_out_status;
bool pump_out_status;
bool lamp_out_status;

bool auto_fan_status;
bool auto_pump_status;

void fan_out(bool val) {
  fan_out_status = val;
}

void pump_out(bool val) {
  pump_out_status = val;
}

void auto_fan(bool val) {
  auto_fan_status = val;
}

void auto_pump(bool val) {
  auto_pump_status = val;
}

void lamp_out(bool val) {
  lamp_out_status = val;
}

IOTItemBin FanStatus;
IOTItemBin Fan(fan_out);

IOTItemBin autoFanStatus;
IOTItemBin AutoFan(auto_fan);

IOTItemBin PumpStatus;
IOTItemBin Pump(pump_out);

IOTItemBin autoPumpStatus;
IOTItemBin AutoPump(auto_pump);

IOTItemBin LampStatus;
IOTItemBin Lamp(lamp_out);

#define ITEM_COUNT 13

IOTItem *items[ITEM_COUNT] = {&dht22_temp, &dht22_humi, &soilhumi ,&FanStatus, &Fan, &LampStatus, &Lamp, &PumpStatus, &Pump, &autoFanStatus, &AutoFan, &autoPumpStatus, &AutoPump};

const char device_id[] = "d83e80a1406318a6bcb4f5403cbdae89";
BrokerComm comm(&Serial, device_id, items, ITEM_COUNT);

void setup() {
  dht.begin();
  u8g2.begin();
  Serial.begin(250000);
  comm.SetInterval(200);

  pinMode(SOILHUMI, INPUT);
  SoftPWM.begin(490);
  pinMode(LAMP, OUTPUT);
  pinMode(PUMP, OUTPUT);
}

void loop() {
  Soilhumi = map(analogRead(SOILHUMI), 0, 1023, 100, 0);
  Humi = dht.readHumidity();
  Temp = dht.readTemperature();

  dht22_temp.Set(Temp);
  dht22_humi.Set(Humi);
  soilhumi.Set(Soilhumi);
  
  if(auto_fan_status == true){
    if (Temp >= 29) {
      SoftPWM.set(100);
      fan_out_status = true;
      FanStatus.Set(fan_out_status);
    }
    else if(Temp <=10){
      SoftPWM.set(0);
      fan_out_status = false;
      FanStatus.Set(fan_out_status);      
    }
    else{
      SoftPWM.set(65);
      fan_out_status = true;
      FanStatus.Set(fan_out_status);
    }
  }
  else if ( auto_fan_status == false and fan_out_status == true) SoftPWM.set(100); 
  else SoftPWM.set(0);

  if(auto_pump_status == true){
    if(Soilhumi >= 30 and Soilhumi <= 60) {
      digitalWrite(PUMP, HIGH);
      pump_out_status = true;
      PumpStatus.Set(digitalRead(PUMP));
    }
    else {
      digitalWrite(PUMP, LOW);    
      pump_out_status = false;
      PumpStatus.Set(digitalRead(PUMP));
    }    
  }
  else if (auto_pump_status == false and pump_out_status == true) digitalWrite(PUMP, HIGH);
  else digitalWrite(PUMP, LOW);

  digitalWrite(PUMP, pump_out_status);
  digitalWrite(LAMP, lamp_out_status);

  dht22_temp.Set(Temp);
  dht22_humi.Set(Humi);
  soilhumi.Set(Soilhumi);
  
  FanStatus.Set(fan_out_status);
  PumpStatus.Set(digitalRead(PUMP));
  LampStatus.Set(digitalRead(LAMP));
  autoFanStatus.Set(auto_fan_status);
  autoPumpStatus.Set(auto_pump_status);
  comm.Run();
}
