#include <VitconBrokerComm.h>
#include "DHT.h"
using namespace vitcon;
#include <SoftPWM.h>

#define SOILHUMI A6
#define DHTPIN A1
#define DHTTYPE DHT22
#define PUMP 16
SOFTPWM_DEFINE_CHANNEL(A3);
DHT dht(DHTPIN, DHTTYPE);

bool fan_out_status;
bool pump_out_status;

float Temp;
float Humi;
int Soilhumi = 0;

void fan_out(bool val) {
  fan_out_status = val;
}

void pump_out(bool val) {
  pump_out_status = val;
}

IOTItemFlo dht22_temp;
IOTItemInt soilhumi; 

IOTItemBin FanStatus;
IOTItemBin Fan(fan_out);

IOTItemBin PumpStatus;
IOTItemBin Pump(pump_out);

#define ITEM_COUNT 6

IOTItem *items[ITEM_COUNT] = {&dht22_temp, &soilhumi, &FanStatus, &Fan, &PumpStatus, &Pump};

const char device_id[] = "d83e80a1406318a6bcb4f5403cbdae89";
BrokerComm comm(&Serial, device_id, items, ITEM_COUNT);

void setup() {
  dht.begin();
  Serial.begin(250000);
  comm.SetInterval(200);

  SoftPWM.begin(490); pinMode(SOILHUMI, INPUT);
  pinMode(PUMP, OUTPUT);
}

void loop() {
  Humi = dht.readHumidity();
  Temp = dht.readTemperature();
  Soilhumi = map(analogRead(SOILHUMI), 0, 1023, 100, 0);
  if (fan_out_status == true) {
    if (Temp >= 29) {
      SoftPWM.set(100); 
    }
    else if (Temp <= 20) {
      SoftPWM.set(0);
    }
    else {
      SoftPWM.set(65);
    }
  }
  else {
    SoftPWM.set(0);
  }
  if (pump_out_status == true) {
    if (Soilhumi >= 30 and Soilhumi <= 60) {
      digitalWrite(PUMP, HIGH);
    }
    else {
      digitalWrite(PUMP, LOW);
    }
  }
  else {
    digitalWrite(PUMP, LOW);
  }

  dht22_temp.Set(Temp);
  soilhumi.Set(Soilhumi);
  FanStatus.Set(fan_out_status);
  PumpStatus.Set(digitalRead(PUMP));
  comm.Run();
}
