#include <VitconBrokerComm.h>
#include <U8g2lib.h>
#include "DHT.h"
#include <SoftPWM.h>
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
using namespace vitcon;
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

bool timeset = false;
bool Interval_Mup_status;
bool Interval_Hup_status;
bool Interval_Sup_status;
bool fan_out_status;
bool PWMBtn1_out_status;
bool PWMBtn2_out_status;
bool pump_out_status;
bool pumpIntevalBtn_out_status;
bool pumpInterval_out_status;
bool LED_out_status;
bool Fantimer_start_status;
bool Pumptimer_start_status;
bool LEDtimer_start_status;
bool LED_value;
bool Pump_value;
bool setting1_out_status;
bool setting2_out_status;
bool setting3_out_status;
bool setting4_out_status;
bool AllRST_out_status = false;

int Hour = 0;
int Minute = 0;
int Second = 0;
int PWM = 0;
int PWM1 = 60;
int PWM2 = 60;
int fan_time_out_value = 0;
int pump_time_out_value = 0;
int LED_time_out_value = 0;
int temp1_out_value = 0;
int temp2_out_value = 0;
int humi1_out_value = 0;
int humi2_out_value = 0;
int Pump_TimerStartTime = 0;
int IntervalSecond = 0;
int Fantmp = 0;
int Pumptmp = 0;
int LEDtmp = 0;
int tmp = 0;
int tmp2 = 0;
int tmp3 = 0;
int timeSetTmp = 0;
int ResetTmp = 0;
int OLEDFlow = 0;


uint32_t timer_status = 0;
uint32_t TimeSum = 0;
uint32_t TimeSum2 = 0;
uint32_t TimeCompare;
uint32_t TimeCompare2;

uint32_t TimePushDelay = 0;
uint32_t TimerStartTime = 0;
uint32_t TimerStartTime2 = 0;

uint32_t DataCapture_ST = 0;
uint32_t OLED_ST = 0;
uint32_t OLEDDelay = 1000;
uint32_t Fan_ST = 0;
uint32_t Pump_ST = 0;
uint32_t LED_ST = 0;

void timeset_out(bool val){
    timeset = val;
}

void Interval_Hup(bool val){
    Interval_Hup_status = val;
}
void Interval_Mup(bool val){
    Interval_Mup_status = val;
}
void Interval_Sup(bool val){
    Interval_Sup_status = val;
}

void fan_time_out(int32_t val) {
  fan_time_out_value = val;
}
void fan_out(bool val) {
  fan_out_status = val;
}
void temp1_out(int32_t val) {
  temp1_out_value = val;
}
void temp2_out(int32_t val) {
  temp2_out_value = val;
}
void PWMBtn1_out(bool val) {
  PWMBtn1_out_status = val;
}
void PWMBtn2_out(bool val) {
  PWMBtn2_out_status = val;
}

void PWMReset(bool val){
    if(val){
        PWM1 = 60;
        PWM2 = 60;
    }
}

void pump_time_out(int32_t val) {
  pump_time_out_value = val;
}
void pump_out(bool val) {
  pump_out_status = val;
}
void humi1_out(int32_t val) {
  humi1_out_value = val;
}
void humi2_out(int32_t val) {
  humi2_out_value = val;
}
void pumpIntevalBtn_out(bool val) {
  pumpIntevalBtn_out_status = val;
}
void pumpInterval_out(bool val) {
  pumpInterval_out_status = val;
}

void LED_time_out(int32_t val) {
  LED_time_out_value = val;
}

void LED_out(bool val) {
  LED_out_status = val;
}

void IntervalReset(bool val){
    if(val){
        Hour = 0;
        Minute = 0;
        Second = 0;
        timeSetTmp = 0;
    }
}
void Fantimer_start(bool val) {
 Fantimer_start_status = val;
}
void Pumptimer_start(bool val) {
 Pumptimer_start_status = val;
}
void LEDtimer_start(bool val) {
 LEDtimer_start_status = val;
}

void setting1_out(bool val) {
  setting1_out_status = val;
}
void setting2_out(bool val) {
  setting2_out_status = val;
}
void setting3_out(bool val) {
  setting3_out_status = val;
}
void setting4_out(bool val) {
  setting4_out_status = val;
}

void AllRST_out(bool val) {
  AllRST_out_status = val;
}

IOTItemFlo dht22_temp;
IOTItemFlo dht22_humi;
IOTItemInt soilhumi;

IOTItemBin FanLight;
IOTItemBin FanTimer(Fantimer_start);
IOTItemInt barFan;
IOTItemInt barFanW(fan_time_out);
IOTItemBin FanStatus;
IOTItemBin Fan(fan_out);
IOTItemInt autoTemp1;
IOTItemInt Temp1(temp1_out);
IOTItemInt autoTemp2;
IOTItemInt Temp2(temp2_out);
IOTItemInt FanPWM1;
IOTItemBin PWMBtn1(PWMBtn1_out);
IOTItemInt FanPWM2;
IOTItemBin PWMBtn2(PWMBtn2_out);
IOTItemBin PWMRST(PWMReset);

IOTItemBin PumpLight;
IOTItemBin PumpTimer(Pumptimer_start);
IOTItemInt barPump;
IOTItemInt barPumpW(pump_time_out);
IOTItemBin PumpStatus;
IOTItemBin Pump(pump_out);
IOTItemInt autoHumi1;
IOTItemInt Humi1(humi1_out);
IOTItemInt autoHumi2;
IOTItemInt Humi2(humi2_out);
IOTItemBin PumpIntervalBtn(pumpIntevalBtn_out);
IOTItemInt intPumpInterval;
IOTItemBin PumpIntervalStatus;
IOTItemBin PumpInterval(pumpInterval_out);

IOTItemBin LEDLight;
IOTItemBin LEDTimer(LEDtimer_start);
IOTItemInt barLED;
IOTItemInt barLEDW(LED_time_out);
IOTItemBin LEDStatus;
IOTItemBin LED(LED_out);

IOTItemInt label_Hinterval;
IOTItemInt label_Minterval;
IOTItemInt label_Sinterval;

IOTItemBin IntervalHUP(Interval_Hup);
IOTItemBin IntervalMUP(Interval_Mup);
IOTItemBin IntervalSUP(Interval_Sup);
IOTItemBin SetBtn(timeset_out);
IOTItemBin RSTBtn(IntervalReset);

IOTItemBin Setting1_light;
IOTItemBin Setting1_Status;
IOTItemBin Setting1(setting1_out);

IOTItemBin Setting2_light;
IOTItemBin Setting2_Status;
IOTItemBin Setting2(setting2_out);

IOTItemBin Setting3_light;
IOTItemBin Setting3_Status;
IOTItemBin Setting3(setting3_out);

IOTItemBin Setting4_light;
IOTItemBin Setting4_Status;
IOTItemBin Setting4(setting4_out);

IOTItemBin AllRST(AllRST_out);

#define ITEM_COUNT 59

IOTItem *items[ITEM_COUNT] = {&dht22_temp, &dht22_humi, &soilhumi,
                              &FanLight, &FanTimer, &barFan, &barFanW,
                              &FanStatus, &Fan, &autoTemp1, &Temp1, &autoTemp2, &Temp2,
                              &FanPWM1, &PWMBtn1, &FanPWM2, &PWMBtn2, &PWMRST, 
                              &PumpLight, &PumpTimer, &barPump, &barPumpW,
                              &PumpStatus, &Pump, &autoHumi1, &Humi1, &autoHumi2, &Humi2,
                              &PumpIntervalBtn, &intPumpInterval, &PumpIntervalStatus, &PumpInterval,
                              &LEDLight, &LEDTimer, &barLED, &barLEDW, &LEDStatus, &LED, 
                              &label_Hinterval, &label_Minterval, &label_Sinterval,
                              &IntervalHUP, &IntervalMUP, &IntervalSUP, &SetBtn, &RSTBtn,
                              &Setting1_light, &Setting1_Status, &Setting1, 
                              &Setting2_light, &Setting2_Status, &Setting2, 
                              &Setting3_light, &Setting3_Status, &Setting3, 
                              &Setting4_light, &Setting4_Status, &Setting4, 
                              &AllRST
                              };

const char device_id[]="81b543deb3c5cd0a82861c471d44e2f4";
BrokerComm comm(&Serial, device_id, items, ITEM_COUNT);

void FanOnTime() {
  if(Fantimer_start_status == true) {
    Fantmp = 1;
    Fan_ST = millis();
    Fantimer_start_status = false;
  }
  if (Fantmp == 1) {
    if ((millis() - Fan_ST) < fan_time_out_value*1000) {
      PWM = 65;
      FanLight.Set(HIGH);
    }
    else {
      Fantmp = 0;
      Fan_ST = millis();
      PWM = 0;
      FanLight.Set(LOW);
    }  
  }
}
void AutoControlFan() {
  if(fan_out_status == true) {
    if (Temp >= temp1_out_value and Temp <= temp2_out_value) {
      PWM = PWM1;
    }
    else if(Temp >  temp2_out_value){
      PWM = PWM2;
    }
    else{
      PWM = 0;
    }
    tmp = 1;
  }
  else {
    if (tmp == 1) {
      PWM = 0;
      tmp = 0;
    }
  }
  if (millis() > TimePushDelay + 500) {

    TimePushDelay = millis();
  }
}

void PumpOnTime() {
  if(Pumptimer_start_status == true) {
    Pumptmp = 1;
    Pump_ST = millis();
    Pumptimer_start_status = false;
  }
  if (Pumptmp == 1) {
    if ((millis() - Pump_ST) < pump_time_out_value*1000) {
      Pump_value = true;
      PumpLight.Set(HIGH);
    }
    else {
      Pumptmp = 0;
      Pump_ST = millis();
      Pump_value = false;
      PumpLight.Set(LOW);
    }
  }
}

void AutoControlpump() {
  if(pump_out_status == true and pumpInterval_out_status != true) {
    if (Soilhumi >= humi1_out_value and Soilhumi <= humi2_out_value) {
      Pump_value = true;
    }
    else{
      Pump_value = false;
    }
    tmp2 = 1;
  }
  else if(pump_out_status == true and pumpInterval_out_status == true) {
    if (TimeCompare2 % 2 == 0) {
      Pump_value = false;
    }
    else if (TimeCompare2 % 2 == 1) {
      Pump_value = true;
    }
  }
  else {
    if (tmp2 == 1) {
      Pump_value = false;
      tmp2 = 0;
    }
  }
}
void LEDOnTime() {
  if(LEDtimer_start_status == true) {
    LEDtmp = 1;
    LED_ST = millis();
    LEDtimer_start_status = false;
  }
  if (LEDtmp == 1) {
    if ((millis() - LED_ST) < LED_time_out_value*1000) {
      LED_value = true;
      LEDLight.Set(HIGH);
    }
    else {
      LEDtmp = 0;
      LED_ST = millis();
      LED_value = false;
      LEDLight.Set(LOW);
    }
  }
}
void AutoControlLED() {
  if (LED_out_status) {
    if (TimeCompare % 2 == 0) {
      LED_value = false;
    }
    else if(TimeCompare % 2 == 1) {
      LED_value = true;
    }
    tmp3 = 1;
  }
  else {
    if (tmp3 == 1) {
      LED_value = false;
      tmp3 = 0;
    }
  }
}

void IntervalSet(bool timeset) {
  if (timeset) {
    timeSetTmp = 1;
  }
  if (LED_out_status and timeSetTmp == 0) {
    TimeSum = 3000;
    TimerStartTime = millis();
  }
  else if (LED_out_status and timeSetTmp == 1) {
    TimeSum = (Hour*3600 + Minute*60 + Second)*1000;
    TimerStartTime = millis();
  }
  TimeSum2 = IntervalSecond*1000;
  TimeCompare2 = (DataCapture_ST - TimerStartTime2)/TimeSum2;
  TimerStartTime2 = millis();
  TimeCompare = (DataCapture_ST - TimerStartTime)/TimeSum;
  if(millis() > TimePushDelay + 500){
  Hour += Interval_Hup_status;
  if(Hour >= 24) Hour = 0;
  Minute += Interval_Mup_status;
  if(Minute >= 60) Minute = 0;
  Second += Interval_Sup_status;
  if(Second >= 60) Second = 0;
  IntervalSecond += pumpIntevalBtn_out_status;
  if (IntervalSecond >= 101) IntervalSecond = 0;
  PWM1 += PWMBtn1_out_status*5;
  if (PWM1 >= 101) PWM1 = 0;
  PWM2 += PWMBtn2_out_status*5;
  if (PWM2 >= 101) PWM2 = 0; 
  TimePushDelay = millis();
  }
}

void AllReset() {
  autoTemp1.Set(0);
  autoTemp2.Set(0);
  PWM1 = 60;
  PWM2 = 60;
  autoHumi1.Set(0);
  autoHumi2.Set(0);
  IntervalSecond = 0;
  pumpInterval_out_status = false;
  Hour = 0;
  Minute = 0;
  Second = 0;
  setting1_out_status = false;
  setting2_out_status = false;
  setting3_out_status = false;
  setting4_out_status = false;
}
void setSetting1() {
  autoTemp1.Set(20);
  autoTemp2.Set(25);
  PWM1 = 70;
  PWM2 = 80;
  autoHumi1.Set(30);
  autoHumi2.Set(60);
  IntervalSecond = 60;
  pumpInterval_out_status = true;
  Hour = 0;
  Minute = 5;
  Second = 0;
  Setting1_light.Set(true);
  setting2_out_status = false;
  setting3_out_status = false;
  setting4_out_status = false;
}
void setSetting2() {
  autoTemp1.Set(24);
  autoTemp2.Set(29);
  PWM1 = 80;
  PWM2 = 100;
  autoHumi1.Set(0);
  autoHumi2.Set(50);
  IntervalSecond = 10;
  pumpInterval_out_status = true;
  Hour = 0;
  Minute = 0;
  Second = 5;
  Setting2_light.Set(true);
  setting1_out_status = false;
  setting3_out_status = false;
  setting4_out_status = false;
}
void setSetting3() {
  autoTemp1.Set(26);
  autoTemp2.Set(32);
  PWM1 = 80;
  PWM2 = 90;
  autoHumi1.Set(25);
  autoHumi2.Set(55);
  IntervalSecond = 12;
  pumpInterval_out_status = true;
  Hour = 1;
  Minute = 30;
  Second = 0;
  Setting3_light.Set(true);
  setting1_out_status = false;
  setting2_out_status = false;
  setting4_out_status = false;
}
void setSetting4() {
  autoTemp1.Set(22);
  autoTemp2.Set(30);
  PWM1 = 65;
  PWM2 = 95;
  autoHumi1.Set(10);
  autoHumi2.Set(40);
  IntervalSecond = 30;
  pumpInterval_out_status = true;
  Hour = 0;
  Minute = 1;
  Second = 30;
  Setting4_light.Set(true);
  setting1_out_status = false;
  setting2_out_status = false;
  setting3_out_status = false;
}

void setup(){
  Serial.begin(250000);
  comm.SetInterval(200);

  pinMode(LAMP, OUTPUT);
  pinMode(PUMP, OUTPUT);
  pinMode(SOILHUMI, INPUT);
  dht.begin();
  u8g2.begin();
  
  digitalWrite(LAMP, LOW);
  SoftPWM.begin(490);

  DataCapture_ST = millis();
  OLED_ST = millis();
}

void loop(){
  Soilhumi = map(analogRead(SOILHUMI), 0, 1023, 100, 0);
  Humi = dht.readHumidity();
  Temp = dht.readTemperature();

  dht22_temp.Set(Temp);
  dht22_humi.Set(Humi);
  soilhumi.Set(Soilhumi);
  IntervalSet(timeset);
  
  FanOnTime();                         
  AutoControlFan();
  SoftPWM.set(PWM);
  if (PWM < 60) FanLight.Set(LOW);
  else FanLight.Set(HIGH);
  
  PumpOnTime();
  AutoControlpump();
  digitalWrite(PUMP, Pump_value);
  if (Pump_value == false) PumpLight.Set(LOW);
  else PumpLight.Set(HIGH);
  
  LEDOnTime();
  AutoControlLED();
  digitalWrite(LAMP, LED_value);
  if (LED_value == false) LEDLight.Set(LOW);
  else LEDLight.Set(HIGH);

  if (AllRST_out_status) {
    AllRST_out_status = false;
    ResetTmp = 1;
    if (ResetTmp == 1) {
      AllReset();
      ResetTmp = 0;
    }
  }
  if (setting1_out_status) {
    setSetting1();
  }
  else Setting1_light.Set(false);
  if (setting2_out_status) {
    setSetting2();
  }
  else Setting2_light.Set(false);
  if (setting3_out_status) {
    setSetting3();
  }
  else Setting3_light.Set(false);
  if (setting4_out_status) {
    setSetting4();
  }
  else Setting4_light.Set(false);
  
  FanPWM1.Set(PWM1);
  FanPWM2.Set(PWM2);
  intPumpInterval.Set(IntervalSecond);
  label_Hinterval.Set(Hour);
  label_Minterval.Set(Minute);
  label_Sinterval.Set(Second);
  
  if ((millis() - OLED_ST) > OLEDDelay) {
    OLEDdraw();
    OLEDFlow += 5;
    if (OLEDFlow >= 130) OLEDFlow = -70;
    OLED_ST = millis();
  }
  comm.Run();
}

void OLEDdraw() {
  u8g2.clearBuffer();
  
  u8g2.drawStr(OLEDFlow, 9, "<TaeWonHo>");
  u8g2.setFont(u8g2_font_ncenB08_te);
  u8g2.drawStr(1, 18, "TEMP");
  u8g2.drawStr(45, 18, "HUMI");
  u8g2.drawStr(90, 18, "SOIL");

  u8g2.setCursor(1, 27);
  u8g2.print(Temp);
  u8g2.drawStr(29, 27, "\xb0");
  u8g2.drawStr(35, 27, "C");
  
  u8g2.setCursor(45, 27);
  u8g2.print(Humi);
  u8g2.drawStr(73, 27, "%");

  u8g2.setCursor(90, 27);
  u8g2.print(Soilhumi);
  u8g2.drawStr(118, 27, "%");

  u8g2.drawStr(45, 37, "Power");
  u8g2.drawStr(90, 37, "Auto");

  u8g2.drawStr(1, 46, "FAN");
  u8g2.setCursor(45, 46);
  u8g2.print(PWM > 0? "ON" : "OFF");
  u8g2.setCursor(90, 46);
  u8g2.print(fan_out_status == true? "ON" : "OFF");
  
  u8g2.drawStr(1, 55, "PUMP");
  u8g2.setCursor(45, 55);
  u8g2.print(Pump_value == true? "ON" : "OFF");
  u8g2.setCursor(90, 55);
  u8g2.print(pump_out_status == true? "ON" : "OFF");
  
  u8g2.drawStr(1, 64, "LAMP");
  u8g2.setCursor(45, 64);
  u8g2.print(LED_value == true? "ON" : "OFF");
  u8g2.setCursor(90, 64);
  u8g2.print(LED_out_status == true? "ON" : "OFF");

  u8g2.sendBuffer();
}
