#include <CurieTime.h>


int ch1TimeTrigger;
int ch2TimerTrigger;
int ch1TimeState = 0;
int ch2TimerState = 0;

void setup() {
  while (!Serial);
  Serial.begin(9600);

  // set the current time to 14:27:00, December 14th, 2015
  setTime(12, 59, 00, 1, 1, 2016);

  delay(2000);
}

void loop() {






  Serial.println();

  Serial.println();


  delay(500);
}



void TriggerRelay(int onTime, int offTime, int channel) {

  int timerCurrent;
  int stateCurrent;

  if (channel == 1) {

    //Print on Serial Output
    Serial.print("\n[Relay Pump] ");

    //Get time when relay was last trigger time depending on channel
    timerCurrent = minute() - ch1TimeTrigger;
    stateCurrent = ch1TimeState;

    //Handles 60 minutes reset scenario
    if (timerCurrent < 0) {
      timerCurrent = timerCurrent + 60;
    }
  } else if (channel == 2) {
    Serial.print("\n[RelayLight] ");
    timerCurrent = hour() - ch2TimerTrigger;
    stateCurrent = ch2TimerState;
    if (timerCurrent < 0) {
      timerCurrent = timerCurrent + 24;
    }
  } else {
    Serial.print("\n[RelayLight] FAIL - Invalid channel number. Valid entries are 1 or 2");
    return;
  }

  //breaks out of funticion if not even 1 increment happened yet
  if (timerCurrent == 0) {
    Serial.print("SKIP - Time value has not changed since last triggered");
    return;
  }

  //Print to serial monitor
  Serial.print("Time since last triggered = ");
  Serial.print(timerCurrent);

  //if channel is currently OFF, attempt to run ON
  if (stateCurrent == 0) {
    Serial.print(" | Current State = Low");
    if (timerCurrent % (offTime) == 0) {
      Serial.print(" | Triggering Relay!");
      stateCurrent = 1;
      if (channel == 1) {
        ch1TimeTrigger = minute();
        ch1TimeState = stateCurrent;
      } else {
        ch2TimerTrigger = hour();
        ch2TimerState = stateCurrent;
      }
    }
  }
  //if channel is currently OFF, attempt to run ON
  else if (stateCurrent == 1) {
    Serial.print(" | Current State = High");
    if (timerCurrent % (onTime) == 0) {
      Serial.print(" | Triggering Relay!");
      stateCurrent = 0;
      if (channel == 1) {
        ch1TimeTrigger = minute();
        ch1TimeState = stateCurrent;
      } else {
        ch2TimerTrigger = hour();
        ch2TimerState = stateCurrent;
      }
    }
  }
}


void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.print('0');
  }
  Serial.print(number);
}
