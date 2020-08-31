#include <VitconBrokerComm.h>
using namespace vitcon;
#include "DHT.h"
#include <SoftPWM.h>

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

bool mode = false;
bool timeset = false;
bool Interval_Mup_status;
bool Interval_Hup_status;
bool fan_out_status;
bool pump_out_status;
bool lamp_out_status;
bool autoBool = false;
bool manualBool = false;

int Hour = 0;
int Minute = 1;
uint32_t TimeSum = 0;
uint32_t TimeCompare;

uint32_t TimePushDelay = 0;
uint32_t TimerStartTime = 0;

uint32_t DataCapture_ST = 0;

void mode_out(bool val) {
  mode = val;
}

void timeset_out(bool val){
    timeset = val;
}


void Interval_Hup(bool val){
    Interval_Hup_status = val;
}

void Interval_Mup(bool val){
    Interval_Mup_status = val;
}

void IntervalReset(bool val){
    if(!timeset && val){
        Hour = 0;
        Minute = 0;
    }
}

void pump_out(bool val) {
  pump_out_status = val;
}


void fan_out(bool val) {
  fan_out_status = val;
}

void lamp_out(bool val) {
  lamp_out_status = val;
}

IOTItemBin StopStatus;
IOTItemBin Stop(timeset_out);

IOTItemBin IntervalHUP(Interval_Hup);
IOTItemBin IntervalMUP(Interval_Mup);
IOTItemBin IntervalRST(IntervalReset);

IOTItemInt label_Hinterval;
IOTItemInt label_Minterval;

IOTItemBin FanStatus;
IOTItemBin Fan(fan_out);

IOTItemBin PumpStatus;
IOTItemBin Pump(pump_out);

IOTItemBin LampStatus;
IOTItemBin Lamp(lamp_out);

IOTItemBin ModeStatus;
IOTItemBin Mode(mode_out);


IOTItemFlo dht22_temp;
IOTItemFlo dht22_humi;
IOTItemInt soilhumi;

#define ITEM_COUNT 18

IOTItem *items[ITEM_COUNT] = {&StopStatus, &Stop,
                            &IntervalHUP, &IntervalMUP, &IntervalRST,
                            &label_Hinterval, &label_Minterval, 
                            &dht22_temp, &dht22_humi, &soilhumi,
                            &ModeStatus, &Mode,
                            &FanStatus, &Fan, &PumpStatus, &Pump,
                            &LampStatus, &Lamp};


const char device_id[]="d83e80a1406318a6bcb4f5403cbdae89";
BrokerComm comm(&Serial, device_id, items, ITEM_COUNT);

void manualMode() {
  if (fan_out_status == true) SoftPWM.set(100);
  else SoftPWM.set(0);
  digitalWrite(PUMP, pump_out_status);
  digitalWrite(LAMP, lamp_out_status);
}

void autoMode() {
  if (TimeCompare % 2 == 0) {
    digitalWrite(LAMP, LOW);
  }
  else if(TimeCompare % 2 == 1) {
    digitalWrite(LAMP, HIGH);
  }
  if(Soilhumi >= 30 and Soilhumi <= 60) {
    digitalWrite(PUMP, HIGH);
  }
  else {
    digitalWrite(PUMP, LOW);    
  }
  if (Temp >= 29) {
    SoftPWM.set(100);
    fan_out_status = true;
  }
  else if(Temp <=10){
    SoftPWM.set(0);     
    fan_out_status = false;
  }
  else{
    SoftPWM.set(65);
    fan_out_status = true;
  }
}

void setup(){
  Serial.begin(250000);
  comm.SetInterval(200);

  pinMode(LAMP, OUTPUT);
  pinMode(PUMP, OUTPUT);
  pinMode(SOILHUMI, INPUT);
  dht.begin();

  digitalWrite(LAMP, LOW);
  SoftPWM.begin(490);

  DataCapture_ST = millis();
}



void loop(){
  Soilhumi = map(analogRead(SOILHUMI), 0, 1023, 100, 0);
  Humi = dht.readHumidity();
  Temp = dht.readTemperature();

  dht22_temp.Set(Temp);
  dht22_humi.Set(Humi);
  soilhumi.Set(Soilhumi);
  IntervalSet(timeset);
  
  if (!mode) {
    autoBool = false;
    manualMode();
  }
  else if (mode) {
    manualBool = false;
    autoMode();
  }
  
  PumpStatus.Set(digitalRead(PUMP));
  LampStatus.Set(digitalRead(LAMP));
  ModeStatus.Set(mode);
  StopStatus.Set(timeset);
  FanStatus.Set(fan_out_status);
  label_Hinterval.Set(Hour);
  label_Minterval.Set(Minute);
  comm.Run();
}

void IntervalSet(bool timeset) {
  if (timeset and mode) {
    TimeSum = (uint32_t)(Hour * 60 + Minute) * 1 * 1000;
    TimeCompare = (millis() - TimerStartTime)/TimeSum;
  }
  else if (!timeset and mode) {
    TimeSum = 3000;
    TimerStartTime = millis();
    if(millis() > TimePushDelay + 500){
        Hour += Interval_Hup_status;
        if(Hour >= 24) Hour = 0;
        Minute += Interval_Mup_status;
        if(Minute >= 60) Minute = 0;

        TimePushDelay = millis();
    }
    TimeCompare = (DataCapture_ST - TimerStartTime)/TimeSum;
  }
}
