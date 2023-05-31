#include <Arduino.h>
#include <ArduinoLowPower.h>
#include <RTCZero.h>

#include "States.h"

#include "LowPower.h"
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
        Serial.println("");
        Serial.println("inputEventCode and trigger equal to zero");
        Serial.print("rmsClassArg.get_inputEventCode(): ");
        Serial.println(rmsClassArg.get_inputEventCode());
        Serial.print("triggeredInputEvent: ");
        Serial.println(*triggeredInputEvent);


        // v4
        Serial.print("rmsClassArg.get_nextWakeUpEPochTime(): ");
        Serial.println(rmsClassArg.get_nextWakeUpEPochTime());
        Serial.print("Unix time = ");
        Serial.println(rtcClassArg.getEpoch());
        uint32_t toSleepEPochTime = rtcClassArg.getEpoch();
        rmsClassArg.set_toSleepEPochTime(toSleepEPochTime);

        // if (rtcClassArg.getEpoch() < rmsClassArg.get_nextWakeUpEPochTime()){
        //     Serial.println("really going to sleep");
        //     rmsClassArg.set_toSleepEPochTime(toSleepEPochTime);
        //     LowPower.sleep();
        // }
        
        // one final check that no wm occured in the last few ms
        if (*triggeredInputEvent == 0){
            Serial.println("to sleep now");
            LowPower.sleep();
        }

        // store time at which device woke up
        wakeUpEpochTime = rtcClassArg.getEpoch();
        Serial.println("");
        Serial.println("Woke up");
        Serial.print("triggeredInputEvent: ");
        Serial.println(*triggeredInputEvent, BIN);
        rmsClassArg.set_wakeUpEPochTime(wakeUpEpochTime);
        // if (Tool_isBitOn(*triggeredInputEvent,WM_INPUTBIT)){
        //     rmsClassArg.set_wakeUpEPochTime(wakeUpEpochTime);
        //     rmsClassArg.set_wmWakeUpEPochTime(wakeUpEpochTime);
        //     // Serial.println("Came in here");
        // }
        // else{
        //     //TODO: this section needs to be placed elesewhere: if the user presses the button when the alarm is matched, the new wakeup time will not be updated this will mean the device will never wakeup
        //     // update the time at which the board woke up
        //     rmsClassArg.set_wakeUpEPochTime(wakeUpEpochTime);
        //     // // inform that RMS just woke up --> no nooed to: done in RTC_matchAlarmISR()
        //     // triggeredInputEvent |= WM_INPUTBIT; //global variable

        // }
        
        // Serial.print("rmsClassArg.get_wakeUpEPochTime()");
        // Serial.println(rmsClassArg.get_wakeUpEPochTime());
        // Serial.print("rmsClassArg.get_wmWakeUpEPochTime()");
        // Serial.println(rmsClassArg.get_wmWakeUpEPochTime());
        Serial.print("Current unix time = ");
        Serial.println(rtcClassArg.getEpoch());
        Serial.print("triggeredInputEvent: ");
        Serial.println(*triggeredInputEvent, BIN);
    }
}

void LP_setupURAInterrupt(){

    LowPower.attachInterruptWakeup(digitalPinToInterrupt(BUTTON_PIN), LP_callbackURA, FALLING);
}

void LP_callbackURA(){
    //triggeredInputEvent -> global variable
    Tool_setBitOn(&triggeredInputEvent, URA_INPUTBIT);
    // Tool_setBitOn(&triggeredInputEvent, URA_WAIT_INPUTBIT); // inform that the 3 sec has not yet elapsed
    //record when the URA occured
    millisOnExternalWakeUp = millis();
}

