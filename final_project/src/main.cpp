#include <Arduino.h>
// #include <RTCZero.h>
#include <Ezo_i2c.h> //include the EZO I2C library from https://github.com/Atlas-Scientific/Ezo_I2c_lib
#include <Arduino_PMIC.h>

#include "Constant.h"

#include "Configuration.h"
#include "States.h"

#include "FSM.h"

#include "SMS.h"

#include "LowPower.h"
#include "RTC.h"
#include "Data.h"
#include "Battery.h"
#include "LED.h"

#include "Tool.h"

#include "Global.h"

// local variables

// define state machine
rmsClass rms;

// Define configuration struct
ConfigurationStruct cfg;

// define EZO peripheral
Ezo_board EZO_ORP = Ezo_board(EZO_ADDRESS, "ORP_EZO");       //create an ORP circuit object, who's address is 98 and name is "ORP_EZO"

// filename that will store the data
char dataFileName[13] = "df.csv";


void setup() {
  
  // initialise serial comm.
  Serial.begin(9600);
  //wait for serial monitor
  // while (!Serial){
  //   delay(1);
  // }
  pinMode(BLUELED_PIN, OUTPUT);

  // visual output
  digitalWrite(BLUELED_PIN, HIGH);
  delay(2000);
  digitalWrite(BLUELED_PIN, LOW);


  // Utils intialisation
  // init I2C
  Wire.begin();
  
  // PMIC
  if (!PMIC.begin()) {
    Serial.println("Failed to initialize PMIC!");
    while (1);
  }

  //nb timeout
  SMS_init(5000);//5 secs timeout
  
  // obtain the charge volatge in the register
  // Serial.print(PMIC.getChargeVoltage());

  Serial.println("------");

  Serial.println("****RMS****");

  Serial.print("RMS ID: ");
  Serial.println(RMS_ID);

  Serial.println("***Setup Start***");

  Serial.println("**LEDs initialisation**");

  // LED pins
  pinMode(BLUELED_PIN, OUTPUT);
  pinMode(REDLED_PIN, OUTPUT);
  pinMode(GREENLED_PIN, OUTPUT);
  pinMode(YELLOWLED_PIN, OUTPUT);
  pinMode(ORANGELED_PIN, OUTPUT);
  LED_runInitBeginSignal();
  Serial.println("DONE.");

  

  Serial.println("**Button initialisation**");

  pinMode(BUTTON_PIN, INPUT);
  Serial.println("DONE.");

  
  Serial.println("**PowerSupply check**");
  if(Battery_runInitSequence(rms)){
    Serial.println("DONE.");
  }
  else{
    Serial.println("NOK. Problem with power supply, please follow instructions");
    return;
  }

  // RTC  
  Serial.println("**Internal RTC initialisation**");
  //TODO: use the nb.getTime: to update the time external RTC ansd internal RTC
  if(RTC_init()){
    char buf[20];
    Serial.print("Current time: ");
    Tool_stringTime(rtc.getEpoch(), buf);
    Serial.println("DONE.");
  }
  else{
    rms.set_rmsState(RTC_FAILEDINIT);
    Serial.print("NOK. Failed to initialise RTC");
    return;
  }

  Serial.println("**SD card initialisation**");
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
  Serial.println("**Configuration parameters setting**");
  Serial.println("*Setting RMS configuration as default*");
  Config_setConfigurationDefault(cfg);
  // Serial.println("Default settings: ");
  // Config_printConfiguration(cfg);
  Serial.println("DONE.");

  Serial.println("*Setting RMS configuration from file*");
  Config_setConfigurationFromFile(cfg);
  // Serial.println("File settings: ");
  // Config_printConfiguration(cfg);
  Serial.println("DONE.");


  Serial.println("*Setting up next HB time*");
  if (RTC_setUpHB(cfg)){
    Serial.println("DONE.");
  }
  else{
    rms.set_rmsState(RTC_FAILEDHBSET);
    Serial.println("NOK. Failed to setup HB time");
    return;
  }


  Serial.println("**Attaching low power external wakeup function**");
  /* 
  Setup LowPower: tell what to do in case of external interrupt. 
  According to source cdoe, this also works when the device is awake
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
  /* execute FSM functions.
  Based on inputs, tracked through the global variable triggeredInputEvent, 
  the corresponding functions will execute */
  FSM_executeFunction(EZO_ORP, rms, rtc, cfg, dataFileName);
  // delay(500);

  switch(rms.get_rmsState()){
    // go in appropriate state
    case INIT:
      Serial.println("-in INIT State");
      digitalWrite(BLUELED_PIN, HIGH);
      digitalWrite(REDLED_PIN, HIGH);
      FSM_initRMS(rms, cfg);
      Serial.println("-end INIT State");

      /*TODO: ensure to check the next hb is checked to be in the future here:
      update the HB time, if again it fails,
      else:return an error and show the rror signal code: send the device into unknown error state
      */
      break;

    case SWQ: 
      Serial.println("-in SWQ State");
      digitalWrite(REDLED_PIN, LOW);
      digitalWrite(GREENLED_PIN, HIGH);

      

     
      //function: determine wakeup period (cf input trigger event)

      LP_goToLowPowerConsumption(rms, rtc, &triggeredInputEvent);

      FSM_updateInputEventCode(rms, rtc, cfg, &triggeredInputEvent);

      
      break;

    case UWQ:
      Serial.println("-in UWQ State");
      digitalWrite(REDLED_PIN, HIGH);
      digitalWrite(GREENLED_PIN, LOW);
      // wakePeriod = 10000; // 20 sec
      //wake every 2 minutes (2*60*1000ms = 120000)


      LP_goToLowPowerConsumption(rms, rtc, &triggeredInputEvent);
      FSM_updateInputEventCode(rms, rtc, cfg, &triggeredInputEvent);


      Serial.println("-end UWQ State");

      break;

    case FWQ:
      Serial.println("-in FWQ State");
      digitalWrite(REDLED_PIN, HIGH);
      digitalWrite(GREENLED_PIN, LOW);
      // wakePeriod = 10000; // 20 sec
      //wake every 2 minutes (2*60*1000ms = 120000)


      LP_goToLowPowerConsumption(rms, rtc, &triggeredInputEvent);
      FSM_updateInputEventCode(rms, rtc, cfg, &triggeredInputEvent);
      Serial.println("-end FWQ State");

      break;

    case SLEEP:
      Serial.println("-in SLEEP State");
      digitalWrite(REDLED_PIN, HIGH);
      digitalWrite(GREENLED_PIN, LOW);
      digitalWrite(BLUELED_PIN, LOW);
      
      LP_goToDeepSleep(rms);
      Serial.println("-end SLEEP State");

      break;
//TODO: remove the in /end for the follwoing states
    case BATTERY_LOW:
      Serial.println("-in BATTERY_LOW State");
      LED_showBatteryLowSignal();
      delay(2000);
      Serial.println("-end BATTERY_LOW State");

      break;

    case BATTERY_NOTCONNECTED:
      Serial.println("-in BATTERY_NOTCONNECTED State");
      LED_showBatteryNotConnectedSignal();
      delay(2000);
      Serial.println("-end BATTERY_NOTCONNECTED State");

      break;

    case SDCARD_NOK:
      Serial.println("-in SDCARD_NOK State");
      LED_showSDCardNokSignal();
      delay(2000);
      Serial.println("-end SDCARD_NOK State");

      break;
    
    case SDCARD_FILENAMENOK:
      Serial.println("-in SDCARD_FILENAMENOK State");
      LED_showSDCardFileNameNOkSignal();
      delay(2000);
      Serial.println("-end SDCARD_FILENAMENOK State");

      break;
    
    case RTC_FAILEDINIT:
      Serial.println("-in RTC_FAILEDINIT State");
      LED_showRTCFailedInitSignal();
      delay(2000);
      Serial.println("-end RTC_FAILEDINIT State");

      break;
    
    case RTC_FAILEDHBSET:
      Serial.println("-in RTC_FAILEDHBSET State");
      LED_showRTCFailedHBSetSignal();
      delay(2000);
      Serial.println("-end RTC_FAILEDHBSET State");

      break;
    
    case UNKNOWN_ERROR:
      Serial.println("-in UNKNOWN_ERROR State");
      LED_showUnknownErrorSignal();
      delay(2000);
      Serial.println("-end UNKNOWN_ERROR State");
      break;

    
      // check if can go to sleep (if all relevant event flag are off)
    }
}