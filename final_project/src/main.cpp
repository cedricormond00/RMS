#include <Arduino.h>
#include <RTCZero.h>

#include <Ezo_i2c.h> //include the EZO I2C library from https://github.com/Atlas-Scientific/Ezo_I2c_lib
#include <Arduino_PMIC.h>

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
#include "Battery.h"
#include"Configuration.h"




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


// local variables

// define state machine
rmsClass rms;

// Define configuration struct
Configuration cfg;

// define EZO peripheral
Ezo_board EZO_ORP = Ezo_board(EZO_ADDRESS, "ORP_EZO");       //create an ORP circuit object, who's address is 98 and name is "ORP_EZO"

// file to store the data
char dataFileName[13] = "df.csv";


uint32_t initWakeUpTime = 0; 
//TODO: find a better way to define the first time interval for intialisation


// uint16_t WMTC_limit = 5; // Upper timer limit for action to be triggered
// uint8_t inputEventCode = 0b0;
// const int nbLastORPValuesStored = 5; // Size of the ring buffer for ORP values
// float ORPRingBuffer[nbLastORPValuesStored];  // Array to store the floating-point values


// uint32_t wakePeriod = 0;
// char ORPData[32];               //we make a 32 byte character array to hold incoming data from the ORP circuit.

void setup() {
  
  // init serial comm.

  Serial.begin(9600);
  //wait for serial monitor
  while (!Serial){
    delay(1);
  }

  // Utils intialisation
  // init I2C
  Wire.begin();
  
  // PMIC
  if (!PMIC.begin()) {
    Serial.println("Failed to initialize PMIC!");
    while (1);
  }
  
  Serial.print(PMIC.getChargeVoltage());

  // delay(2000);
  Serial.println("------");


  Serial.println("****RMS****");

  Serial.println("***Setup Start***");


  Serial.println("**Initializing LEDs**");

  // LED pins
  pinMode(BLUELED_PIN, OUTPUT);
  pinMode(REDLED_PIN, OUTPUT);
  pinMode(GREENLED_PIN, OUTPUT);
  pinMode(YELLOWLED_PIN, OUTPUT);
  pinMode(ORANGELED_PIN, OUTPUT);
  LED_runInitBeginSignal();
  Serial.println("DONE.");

  

  Serial.println("**Initializing button**");

  pinMode(BUTTON_PIN, INPUT);
  Serial.println("DONE.");

  
  Serial.println("**Checking powerSupply**");
  if(Battery_runInitSequence(rms)){
    Serial.println("DONE.");
  }
  else{
    Serial.println("NOK. Problem with power supply, please follow instructions");
    return;
  }

    Serial.println("**Initializing internal RTC**");
  // RTC  
  if(RTC_init()){
    char buf[20];
    Serial.print("Current time: ");
    RTC_getTimeInText(rtc.getEpoch(), buf);
    Serial.println("DONE.");
  }
  else{
    rms.set_rmsState(RTC_FAILEDINIT);
    Serial.print("NOK. Failed to initialise RTC");
    return;
  }

  Serial.println("**Initialising SD card**");
  if (Data_SDCard_init()){
    Serial.println("DONE.");
    // create a valide filename
  }
  else{
    Serial.println("NOK. SD card not initialised");
    rms.set_rmsState(SDCARD_NOK);
    return;
  }

  Serial.println("*Creating a valid datalogging filename*");
  if(Data_createValidDataFileName(dataFileName)){
    Serial.println("DONE.");
  }
  else{
    Serial.println("NOK. Failed to create a valid filename");
    rms.set_rmsState(SDCARD_FILENAMENOK);
    return;
  }

  Serial.println("*Populating first row of datafile*");
  //initialise first row
  uint8_t numberOfCols = 10;
  String colNames[numberOfCols] = {"readEPochTimeStamp",
                                "readTimeStamp", 
                                "orpValue", 
                                "State", 
                                "inputEvent", 
                                "triggeredInputEvent", 
                                "isPowerSourceStable",
                                "BatteryVoltage",
                                "BatteryELState",
                                "chargeStatus"};
  if(Data_populateHeaderRowToFile(colNames, numberOfCols, dataFileName)){
    Serial.println("DONE.");
  }
  else{
    Serial.println("NOK. Failed to populate first row");
    rms.set_rmsState(SDCARD_FILENAMENOK);
    return;
  }


  /*TODO: 
  - read config file
  - if fail, use default config
  - setup rms with config settings --> done in INIT
  */
  Serial.println("**Setting configuration parameters**");
  Serial.println("*Setting RMS default config*");
  Config_setConfigurationDefault(cfg);
  Serial.println("DONE.");

  Serial.println("*Setting RMS file config*");
  Config_setConfigurationFromFile(cfg);
  Serial.println("DONE.");


  // // rms
  // // set RMS on same time as RTC on initialisation
  // //TODO: check if this is required
  // rms.set_wakeUpEPochTime(DEFAULT_EPOCHTIME+0);
  // rms.set_toSleepEPochTime(DEFAULT_EPOCHTIME);

  Serial.println("*Setting up next HB time*");
  if (RTC_setUpHB()){
    Serial.println("DONE.");
  }
  else{
    rms.set_rmsState(RTC_FAILEDHBSET);
    Serial.println("NOK. Failed to setup HB time");
    return;
  }


  





  Serial.println("**Attaching low power external wakeup function**");
  /* Sertup LowPower: tell what to do in case of external interrupt
  - According to source cdoe, this also works when the device is awake

  */ 
  LP_setupURAInterrupt();
  Serial.println("DONE.");


  Serial.println("**Saving RMS config to SD**");
  Config_saveConfigurationToSD(cfg);
  Serial.println("DONE.");

  

  
  // init completed
  LED_runInitCompleteSignal();

  Serial.println("***Setup complete***");
  Serial.println("------");
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
  
  FSM_executeFunction(EZO_ORP, rms, rtc, dataFileName);
  // Serial.print(rms.get_rmsState());
  // Serial.print("test");
  delay(500);
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
      // rtc.setAlarmEpoch(rms.get_wakeUpEPochTime());

      // Serial.print("rms.get_wmWakeUpEPochTime(): ");
      // Serial.println(rms.get_wmWakeUpEPochTime());
      // rtc.setAlarmEpoch(rms.get_wmWakeUpEPochTime());

      rms.set_inputEventCode(0b1);

      // put rms.set_inputEventCode(0b1); in the setup loop
      // init_smsPowerStruct(initWakeUpTime);

      /*TODO: ensure to check the next hb is checked to be in the future here:
      update the HB time, if again it fails,
      else:return an error and show the rror signal code: send the device into unknown error state
      */
      
      /*TODO: set inside the rms class
      - input event code
      - time
        - wake up
        - wmWakeUp -> for future alarm 
        - 
      - sleep period depending on rmsState
        - value defined in the SD Card
      - alarmStruct -> for both URA + WM
        - allowedIntervalBetweenSMS_initVal); -> use the config file
        (- lastAlarmSMSEPochTime_initVal, -> if equal to 0, set to now)
        (- currentAlarmEPochTime_initVal, -> if equal to 0, set to now)
        - -> setting to now prevents that the alarm gets triggered at start up
        - alarmSituation (wm) -> set to NOANOMALIES
      - stateHistoryStruct -
        - simply call reset
      - the BUP -> power struct 
        - simply call FSM_setPowerSituation
      - smsPowerStruct
        - simply call init_smsPowerStruct (consider that we consider that the operator knows the state at init)
      - 
      */

     //TODO: CONFIG : in rmsCLass: create a config function to set the _SWQSleepPerio
    

      break;

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
      digitalWrite(REDLED_PIN, HIGH);
      digitalWrite(GREENLED_PIN, LOW);
      digitalWrite(BLUELED_PIN, LOW);
      
      LP_goToDeepSleep(rms);
      break;

    case BATTERY_LOW:
      LED_showBatteryLowSignal();
      delay(2000);
      break;

    case BATTERY_NOTCONNECTED:
      LED_showBatteryNotConnectedSignal();
      delay(2000);
      break;

    case SDCARD_NOK:
      LED_showSDCardNokSignal();
      delay(2000);
      break;
    
    case SDCARD_FILENAMENOK:
      LED_showSDCardFileNameNOkSignal();
      delay(2000);
      break;
    
    case RTC_FAILEDINIT:
      LED_showRTCFailedInitSignal();
      delay(2000);
      break;
    
    case RTC_FAILEDHBSET:
      LED_showRTCFailedHBSetSignal();
      delay(2000);
      break;

    
      // check if can go to sleep (if all relevant event flag are off)
    }
}