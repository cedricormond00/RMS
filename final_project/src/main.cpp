#include <Arduino.h>
#include <RTCZero.h>

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
#include "RTC.h"
#include "Data.h"




// // types
// enum RMSState {
//    INIT = 0,
//    SWQ,
//    UWQ,
//    FWQ,
//    SLEEP
// };


// instance of Real Time Clock
// RTCZero rtc;

// define state machine
rmsClass rms;


// local variables
// define EZO peripheral
Ezo_board EZO_ORP = Ezo_board(EZO_ADDRESS, "ORP_EZO");       //create an ORP circuit object, who's address is 98 and name is "ORP_EZO"

RMSState rmsState = INIT;

uint32_t initWakeUpTime = 0; //TODO: find a better way to define the first time interval for intialisation

// uint16_t WMTC_limit = 5; // Upper timer limit for action to be triggered
// uint8_t inputEventCode = 0b0;
// const int nbLastORPValuesStored = 5; // Size of the ring buffer for ORP values
// float ORPRingBuffer[nbLastORPValuesStored];  // Array to store the floating-point values


// uint32_t wakePeriod = 0;
// char ORPData[32];               //we make a 32 byte character array to hold incoming data from the ORP circuit.

void setup() {
  Serial.begin(9600);
  //wait for serial monitor
  while (!Serial);

  //print my class info
  Serial.print("rmsState class instance, stae: ");
  Serial.println(rms.get_rmsState());

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

  pinMode(BUTTON_PIN, INPUT);

  // // timer
  // Timer_tc4_init16bit(timeIncrement, timeIncrementType);
  // Serial.println("TC4 init completed");

  // configure LowPower: tell what to do in case of ecxternal interrupt
  LP_setupURAInterrupt();


  
  
  // RTC  
  RTC_init();


  //SD
  bool SDIsInit = Data_SDCard_init();
  Serial.print("is cSD Card initialised? ");
  Serial.println(SDIsInit);

  // Sd card
  if (SDIsInit){
    // remove preexisting file
    Data_removeFile(dataFileName);

    //initialise first row
    uint8_t numberOfCols = 7;
    String colNames[numberOfCols] = {"readEPochTimeStamp", "readTimeStamp", "orpValue", "State", "inputEvent", "triggeredInputEvent", "BatteryVoltage"};
    Data_populateHeaderRowToFile(colNames, numberOfCols, dataFileName);
  }
  
    
    
  // rms
  // set RMS on same time as RTC on initialisation
  rms.set_wakeUpEPochTime(DEFAULT_EPOCHTIME+0);
  rms.set_toSleepEPochTime(DEFAULT_EPOCHTIME);
  
  for (int i=0; i<10; i++){ 
    ToggleLED(BLUELED_PIN);
    delay(100);
  }
  digitalWrite(BLUELED_PIN, LOW);
  // init completed
}

void loop() {
  // RTC_printTime(rtc);
  // delay(1000);
  // Serial.println(digitalRead(BUTTON_PIN));
  //   FSM_executeFunction(&inputEventCode, &EZO_ORP, &rmsState);
  // if (digitalRead(BUTTON_PIN) == HIGH){
  //   Serial.print("triggeredInputEvent: ");
  //   Serial.println(triggeredInputEvent, BIN);
  // }
  
  FSM_executeFunction(EZO_ORP, rms, rtc);
  // Serial.print(rms.get_rmsState());
  // Serial.print("test");

  switch(rms.get_rmsState()){
    

    
    // go in appropriate state
    case INIT:
      digitalWrite(BLUELED_PIN, HIGH);
      digitalWrite(REDLED_PIN, HIGH);
      // let the mahchine know it must perform the Water Monitoring function right now

      // as if at initialisation the device has just woken up
      initWakeUpTime = rtc.getEpoch();
      rms.set_wakeUpEPochTime(initWakeUpTime);
      rms.set_wmWakeUpEPochTime(initWakeUpTime);

      Serial.print("rms.get_wakeUpEPochTime(): ");
      Serial.println(rms.get_wakeUpEPochTime());
      rtc.setAlarmEpoch(rms.get_wakeUpEPochTime());

      // Serial.print("rms.get_wmWakeUpEPochTime(): ");
      // Serial.println(rms.get_wmWakeUpEPochTime());
      // rtc.setAlarmEpoch(rms.get_wmWakeUpEPochTime());

      rms.set_inputEventCode(0b1);

      

    case SWQ: 
      digitalWrite(REDLED_PIN, LOW);
      digitalWrite(GREENLED_PIN, HIGH);
      // wakePeriod = 10000; //every 30 sec
      // wake evry 5 minutes (5*60*1000ms=300000)
      

     
      //function: determine wakeup period (cf input trigger event)

      LP_goToLowPowerConsumption(rms, rtc, &triggeredInputEvent);

      FSM_updateInputEventCode(rms, rtc, &triggeredInputEvent);

      
      break;

    case UWQ:
      digitalWrite(REDLED_PIN, HIGH);
      digitalWrite(GREENLED_PIN, LOW);
      // wakePeriod = 10000; // 20 sec
      //wake every 2 minutes (2*60*1000ms = 120000)


      LP_goToLowPowerConsumption(rms, rtc, &triggeredInputEvent);
      FSM_updateInputEventCode(rms, rtc, &triggeredInputEvent);



      break;

    case FWQ:
      // rmsState = SWQ;
      digitalWrite(REDLED_PIN, HIGH);
      digitalWrite(GREENLED_PIN, LOW);
      // wakePeriod = 10000; // 20 sec
      //wake every 2 minutes (2*60*1000ms = 120000)


      LP_goToLowPowerConsumption(rms, rtc, &triggeredInputEvent);
      FSM_updateInputEventCode(rms, rtc, &triggeredInputEvent);
      break;

    case SLEEP:
      rmsState = SWQ;
      break;

    
      // check if can go to sleep (if all relevant event flag are off)
    }
}