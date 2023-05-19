#include <Arduino.h>

#include "Constant.h"
#include "Global.h"

#include "Serial.h"
#include "Timer.h"
#include "I2c.h"
#include "LED.h"
#include "FSM.h"


// types
enum RMSState {
   INIT = 0,
   SWQ,
   UWQ,
   FWQ,
   SLEEP
};

// local variables
RMSState rmsState = INIT;


void setup() {
  //wait for serial monitor
  while (!Serial);

  // initialise 
  // i2c
  I2c_init();

  // LED pins
  pinMode(YELLOWLED_PIN, OUTPUT);
  digitalWrite(YELLOWLED_PIN, LOW);
  pinMode(ORANGELED_PIN, OUTPUT);
  digitalWrite(ORANGELED_PIN, HIGH);
  pinMode(REDLED_PIN, OUTPUT);
  digitalWrite(REDLED_PIN, HIGH);
  delay(2000); // to have time to check on board
  digitalWrite(ORANGELED_PIN, LOW);
  delay(1000); // to have time to check on board

  // timer
  //Timer_tc4_init16bit(1900, {'t'});

  Timer_tc4_init16bit(timeIncrement, timeIncrementType);
  Serial.println("init completed");


  // init completed
  // normally go to unsafe mode
  rmsState = SWQ;
  Serial.println("init completed");




}

void loop() {

  //switch statement
  switch(rmsState){
    // go in appropriate state
    case INIT:

    case SWQ: 
      // check if must read (event flag (triggered upon interrutp)
      if (timerFlag) {
        // char command[1] = {'r'};
        // // I2c_sendCommandToSensor(ORP_data, command, 1);
        // I2c_sendReceiveORP(ORP_data, command, 1);

        // timerFlag = false; // set the boolFlag to false
        // ToggleLED(YELLOWLED_PIN);
        // Serial.println("came in switch");
      }
      FSM_updateEventCounters();
      break;
    case UWQ:
      break;
    case FWQ:
      break;
    case SLEEP:
      break;
    
      // check if can go to sleep (if all relevant event flag are off)
    }
}