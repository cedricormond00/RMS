#include <Arduino.h>
#include <ArduinoLowPower.h>
#include <RTCZero.h>

#include "States.h"
#include "Configuration.h"

#include "LowPower.h"
#include "RTC.h"
#include "SMS.h"

#include "Constant.h"
// requires the global triggeredInputEvent
#include "Global.h"

#include "Tool.h"

#include "LED.h"

bool debug_LowPower = false;

void LP_goToLowPowerConsumption(rmsClass& rmsClassArg, RTCZero& rtcClassArg, volatile uint8_t* triggeredInputEvent){
    
    bool debug = false;
    char message[160]; // buf to hold text to be displayed
    char dateTime[21]; // holds the string time
    
     /*ensure
      - all functions have been completed
      - no external interrupt triggered in the meantime
     */
    if (rmsClassArg.get_inputEventCode() == 0 && *triggeredInputEvent == 0){
        uint32_t wakeUpEpochTime = 0;
        if (debug_LowPower && debug){
            Serial.println("");
            Serial.println("inputEventCode and trigger equal to zero");
            Serial.print("rmsClassArg.get_inputEventCode(): ");
            Serial.println(rmsClassArg.get_inputEventCode());
            Serial.print("triggeredInputEvent: ");
            Serial.println(*triggeredInputEvent);
            Serial.print("rmsClassArg.get_nextWakeUpEPochTime(): ");
            Serial.println(rmsClassArg.get_nextWakeUpEPochTime());
            Serial.print("Unix time = ");
            Serial.println(rtcClassArg.getEpoch());
        }
        
        
        // ensure that the next wake up time is in the future
        while( rmsClassArg.get_nextWakeUpEPochTime() <= rtcClassArg.getEpoch()){
        /* ToDO: add check to ensure the next wakeup time is in future compared to current time. 
        Else, either:
        - set next wakeup time in 1 sec
        - check why it is not appropriate
        - raise an error
        */
            rmsClassArg.set_nextWakeUpEPochTime(rmsClassArg.get_nextWakeUpEPochTime()+10);
        }
        
        

        // one final check that no wm occured in the last few ms
        if (*triggeredInputEvent == 0){
            rtcClassArg.setAlarmEpoch(rmsClassArg.get_nextWakeUpEPochTime());
            
            rmsClassArg.set_toSleepEPochTime(rtcClassArg.getEpoch());
            Serial.println("to sleep now");
            LowPower.sleep();
            // LowPower.deepSleep();

        }
    
        // store time at which device woke up
        wakeUpEpochTime = rtcClassArg.getEpoch();
        rmsClassArg.set_wakeUpEPochTime(wakeUpEpochTime);
        Tool_stringTime(wakeUpEpochTime, dateTime);

        Serial.println("");
        Serial.println("Woke up");
        Serial.print("triggeredInputEvent: ");
        Serial.println(*triggeredInputEvent, BIN);
        Serial.print("Current unix time = ");
        Serial.println(wakeUpEpochTime);
        Serial.print("Readable time = ");
        Serial.println(dateTime);

    }
}

void LP_setupURAInterrupt(){
    //TODO: also attach the itnerrupt for wakeup, when the device is not in sleep mode

    LowPower.attachInterruptWakeup(digitalPinToInterrupt(BUTTON_PIN), LP_callbackURA, FALLING);
}

void LP_callbackURA(){
    //triggeredInputEvent -> global variable
    Tool_setBitOn(&triggeredInputEvent, URA_INPUTBIT);
    // Tool_setBitOn(&triggeredInputEvent, URA_WAIT_INPUTBIT); // inform that the 3 sec has not yet elapsed
    //record when the URA occured
    millisOnExternalWakeUp = millis();
}


void LP_goToDeepSleep(rmsClass& rmsClassArg, ConfigurationStruct& cfgStructArg){
    
    //this should wake the device up from deepsleep
    LowPower.attachInterruptWakeup(digitalPinToInterrupt(PMIC_IRQ_PIN), LP_callbackDeepSleep, RISING);

    Serial.println("Going to deepSleep");

    LowPower.deepSleep();

    Serial.println("woke up from deepSleep");
    
    // Inform the operator that the device has woken up from deepsleep
    SMS_deepSleepWakeUp(rmsClassArg, cfgStructArg);

    // reset to a known states some variables for proper reboot, after an unknown period of sleep
    rmsClassArg.set_wakeUpEPochTime(rtc.getEpoch());
    detachInterrupt(digitalPinToInterrupt(PMIC_IRQ_PIN));

    rmsClassArg.set_rmsState(INIT);
    RTC_init();
    RTC_setUpHB(cfgStructArg);
    LP_setupURAInterrupt();
}

void LP_callbackDeepSleep(){
    Serial.println("in LP_callBackDeepSleep");

}