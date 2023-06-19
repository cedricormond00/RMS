#include <Arduino.h>
#include <ArduinoLowPower.h>
#include <RTCZero.h>

#include "States.h"

#include "LowPower.h"
#include "RTC.h"

#include "Constant.h"
// requires the global triggeredInputEvent
#include "Global.h"

#include "Tool.h"

#include "LED.h"


void LP_goToLowPowerConsumption(rmsClass& rmsClassArg, RTCZero& rtcClassArg, volatile uint8_t* triggeredInputEvent){
    
    
     /*ensure
      - all functions have been completed
      - no external interrupt triggered in the meantime
     */
    if (rmsClassArg.get_inputEventCode() == 0 && *triggeredInputEvent == 0){
        uint32_t wakeUpEpochTime = 0;
        uint32_t toSleepEPochTime = 0;

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

        toSleepEPochTime = rtcClassArg.getEpoch();

        // if (rtcClassArg.getEpoch() < rmsClassArg.get_nextWakeUpEPochTime()){
        //     Serial.println("really going to sleep");
        //     rmsClassArg.set_toSleepEPochTime(toSleepEPochTime);
        //     LowPower.sleep();
        // }
        
        // one final check that no wm occured in the last few ms
        if (*triggeredInputEvent == 0){
        /* ToDO: add check to ensure the next wakeup time is in future compared to current time. 
        Else, either:
        - set next wakeup time in 1 sec
        - check why it is not appropriate
        - raise an error
        */

            rmsClassArg.set_toSleepEPochTime(toSleepEPochTime);
            Serial.println("to sleep now");
            LowPower.sleep();
        }

        // store time at which device woke up
        wakeUpEpochTime = rtcClassArg.getEpoch();
        rmsClassArg.set_wakeUpEPochTime(wakeUpEpochTime);

        Serial.println("");
        Serial.println("Woke up");
        Serial.print("triggeredInputEvent: ");
        Serial.println(*triggeredInputEvent, BIN);
        Serial.print("Current unix time = ");
        Serial.println(rtcClassArg.getEpoch());
        Serial.print("triggeredInputEvent: ");
        Serial.println(*triggeredInputEvent, BIN);
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


void LP_goToDeepSleep(rmsClass& rmsClassArg){
    
    LowPower.attachInterruptWakeup(digitalPinToInterrupt(PMIC_IRQ_PIN), LP_callbackDeepSleep, RISING);

    Serial.println("Going to deepSleep");

    LowPower.deepSleep();

    Serial.println("woke up from deepSleep");
    detachInterrupt(digitalPinToInterrupt(PMIC_IRQ_PIN));

    rmsClassArg.set_rmsState(INIT);
    //TODO: send SM wokeup from deeplsleep: 
    RTC_init();
    LP_setupURAInterrupt();
}

void LP_callbackDeepSleep(){
    Serial.println("in LP_callBackDeepSleep");

}