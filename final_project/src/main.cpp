#include <Arduino.h>


#include <Ezo_i2c.h> //include the EZO I2C library from https://github.com/Atlas-Scientific/Ezo_I2c_lib


#include "Constant.h"
#include "Global.h"

#include "Serial.h"
#include "Timer.h"
#include "I2c.h"
#include "LED.h"
#include "FSM.h"
#include "Tool.h"
#include "States.h"
#include "LowPower.h"





// local variables

// define EZO peripheral
Ezo_board EZO_ORP = Ezo_board(EZO_ADDRESS, "ORP_EZO");       //create an ORP circuit object, who's address is 98 and name is "ORP_EZO"

RMSState rmsState = INIT;
uint16_t WMTC_limit = 5; // Upper timer limit for action to be triggered
uint8_t inputEventCode = 0b0;
const int nbLastORPValuesStored = 5; // Size of the ring buffer for ORP values
float ORPRingBuffer[nbLastORPValuesStored];  // Array to store the floating-point values

float ORPValue;

uint32_t wakePeriod = 0;
// char ORPData[32];               //we make a 32 byte character array to hold incoming data from the ORP circuit.

void setup() {
  //wait for serial monitor
  while (!Serial);

  // initialise 
  // i2c
  I2c_init();

  // LED pins
  pinMode(BLUELED_PIN, OUTPUT);
  pinMode(REDLED_PIN, OUTPUT);
  pinMode(GREENLED_PIN, OUTPUT);
  pinMode(YELLOWLED_PIN, OUTPUT);
  pinMode(ORANGELED_PIN, OUTPUT);
  digitalWrite(BLUELED_PIN, LOW);
  digitalWrite(REDLED_PIN, LOW);
  digitalWrite(GREENLED_PIN, LOW);
  digitalWrite(YELLOWLED_PIN, LOW);
  digitalWrite(ORANGELED_PIN, LOW);


  // // timer
  // Timer_tc4_init16bit(timeIncrement, timeIncrementType);
  // Serial.println("TC4 init completed");

  // configure LowPower: tell what to do in case of ecxternal interrupt
  LP_setupURAInterrupt();

  

  // init completed
}

void loop() {
  FSM_executeFunction(&inputEventCode, &EZO_ORP, &rmsState);
  // Serial.print("test");
  //switch statement
  switch(rmsState){

    
    // go in appropriate state
    case INIT:
      digitalWrite(BLUELED_PIN, HIGH);
      digitalWrite(REDLED_PIN, HIGH);
      // let the mahchine know it must perform the Water Monitoring function right now
      inputEventCode = 0b1;
      

    case SWQ: 
      digitalWrite(REDLED_PIN, LOW);
      digitalWrite(GREENLED_PIN, HIGH);
      wakePeriod = 15000; //every 30 sec
      // wake evry 5 minutes (5*60*1000ms=300000)

     

      LP_goToLowPowerConsumption(inputEventCode, wakePeriod);

      FSM_updateInputEventCode(&inputEventCode, &triggeredInputEvent);

      
      break;

    case UWQ:
      digitalWrite(REDLED_PIN, HIGH);
      digitalWrite(GREENLED_PIN, LOW);
      wakePeriod = 10000; // 20 sec
      //wake every 2 minutes (2*60*1000ms = 120000)

      FSM_updateInputEventCode(&inputEventCode, &triggeredInputEvent);

      LP_goToLowPowerConsumption(inputEventCode, wakePeriod);


      break;

    case FWQ:
      rmsState = SWQ;
      break;

    case SLEEP:
      rmsState = SWQ;
      break;

    
      // check if can go to sleep (if all relevant event flag are off)
    }
}