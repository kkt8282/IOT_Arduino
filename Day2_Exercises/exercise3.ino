#include <VitconBrokerComm.h>
using namespace vitcon;
#include <SoftPWM.h>

#define PUMP 16
SOFTPWM_DEFINE_CHANNEL(A3);

bool timeset = false;
bool Interval_Mup_status;
bool Interval_Hup_status;
bool fan_out_status;

int Hour = 0;
int Minute = 1;
int pwm_value = 0;
uint32_t TimeSum = 0;
uint32_t TimeCompare;

uint32_t TimePushDelay = 0;
uint32_t TimerStartTime = 0;

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
void pwm_out(int32_t val) { 
  pwm_value = val; 
}

void fan_out(bool val) {
  fan_out_status = val;
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


IOTItemInt pwm_status;
IOTItemInt pwm(pwm_out);

#define ITEM_COUNT 11

IOTItem *items[ITEM_COUNT] = {&StopStatus, &Stop,
                            &IntervalHUP, &IntervalMUP, &IntervalRST,
                            &label_Hinterval, &label_Minterval, 
                            &pwm_status, &pwm, &FanStatus, &Fan};


const char device_id[]="d83e80a1406318a6bcb4f5403cbdae89";
BrokerComm comm(&Serial, device_id, items, ITEM_COUNT);


void setup(){
    Serial.begin(250000);
    comm.SetInterval(200);

    SoftPWM.begin(490);
}

void IntervalSet(bool timeset){
    if(!timeset){
        TimeSum = (uint32_t)(Hour * 60 + Minute) * 60 * 1000;
        TimerStartTime = millis();

        if(millis() > TimePushDelay + 500){
            Hour += Interval_Hup_status;
            if(Hour >= 24) Hour = 0;
            Minute += Interval_Mup_status;
            if(Minute >= 60)Minute = 0;

            TimePushDelay = millis();
        }
    }
    else if(timeset){
        TimeCompare = (millis() - TimerStartTime)/TimeSum;
    }
}

void loop(){
    IntervalSet(timeset);
    if (fan_out_status == true) {
      if(timeset){
        if(TimeCompare % 2 ==0){
            SoftPWM.set(pwm_value);
        }
        else if(TimeCompare % 2 == 1){
            SoftPWM.set(0);
        }
      }
      else {
          SoftPWM.set(pwm_value);
      }
    }
    else {
      SoftPWM.set(0);
    }
    pwm_status.Set(pwm_value);
    StopStatus.Set(timeset);
    FanStatus.Set(fan_out_status);
    label_Hinterval.Set(Hour);
    label_Minterval.Set(Minute);
    comm.Run();
}
