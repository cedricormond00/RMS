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


void LP_goToLowPowerConsumption(rmsClass& rmsClassArg, RTCZero& rtcClassArg, uint8_t inputEventCode, uint32_t intendedWakePeriod){
    
    
     
    if (inputEventCode == 0){
        // find the time awake from wakeupe
        uint32_t awakePeriod = (rtcClassArg.getY2kEpoch()- rmsClassArg.get_wakeUpEPochTime())*1000;
        Serial.print("awakePeriod: ");
        Serial.println(awakePeriod);
        uint32_t nextWakeUpPeriod = intendedWakePeriod-awakePeriod;
        // uint32_t nextWakeUpPeriod = intendedWakePeriod;
        Serial.print("nextWakeUpPeriod: ");
        Serial.println(nextWakeUpPeriod);
        Serial.println("");
        Serial.println("Going to sleep");
        Serial.println("");
        // milis = (seconds - seconds) *1000
        uint32_t lastAtSleepPeriod = (rmsClassArg.get_wakeUpEPochTime() - rmsClassArg.get_toSleepEPochTime())*1000;
        Serial.print("lastAtSleepPeriod");
        Serial.println(lastAtSleepPeriod);
        // intendedWakePeriod in milis
        // lastAtSleepPeriod in milis
        if (lastAtSleepPeriod < intendedWakePeriod){
            Serial.println("updating the wakeup period");
            nextWakeUpPeriod = intendedWakePeriod - lastAtSleepPeriod;
            Serial.print("nextWakeUpPeriod; ");
            Serial.println(nextWakeUpPeriod);
        }
        Serial.print("nextWakeUpPeriod used: ");
        Serial.println(nextWakeUpPeriod);
        


        // update the time at which the board goes to sleep
        rmsClassArg.set_toSleepEPochTime(rtcClassArg.getY2kEpoch());

        LowPower.sleep(nextWakeUpPeriod);

        // update the time at which the board woke up
        rmsClassArg.set_wakeUpEPochTime(rtcClassArg.getY2kEpoch());
        // inform that RMS just woke up
        triggeredInputEvent |= WM_INPUTBIT; //global variable

    }
}

void LP_setupURAInterrupt(){

    LowPower.attachInterruptWakeup(digitalPinToInterrupt(BUTTON_PIN), LP_callbackURA, FALLING);
}

void LP_callbackURA(){
    //triggeredInputEvent -> global variable
    Tool_setBitOn(&triggeredInputEvent, URA_INPUTBIT);
    Tool_setBitOn(&triggeredInputEvent, URA_WAIT_INPUTBIT); // inform that the 3 sec has not yet elapsed
    //record when the URA occured
    millisOnExternalWakeUp = millis();
}