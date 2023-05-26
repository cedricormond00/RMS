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
        Serial.println("");
        Serial.println("Going to sleep");
        // Serial.println("");
        // // find the time awake from wakeupe
        // uint32_t awakePeriod = (rtcClassArg.getY2kEpoch()- rmsClassArg.get_wakeUpEPochTime())*1000;
        // Serial.print("awakePeriod: ");
        // Serial.println(awakePeriod);
        // uint32_t nextWakeUpPeriod = intendedWakePeriod-awakePeriod;
        // Serial.print("nextWakeUpPeriod: ");
        // Serial.println(nextWakeUpPeriod);

        // // milis = (seconds - seconds) *1000
        // uint32_t lastAtSleepPeriod = (rmsClassArg.get_wakeUpEPochTime() - rmsClassArg.get_toSleepEPochTime())*1000;
        // Serial.print("lastAtSleepPeriod");
        // Serial.println(lastAtSleepPeriod);
        // // intendedWakePeriod in milis
        // // lastAtSleepPeriod in milis
        // if (lastAtSleepPeriod < intendedWakePeriod){
        //     Serial.println("updating the wakeup period");
        //     nextWakeUpPeriod = nextWakeUpPeriod - lastAtSleepPeriod;
        //     Serial.print("nextWakeUpPeriod; ");
        //     Serial.println(nextWakeUpPeriod);
        // }
        // Serial.print("nextWakeUpPeriod used: ");
        // Serial.println(nextWakeUpPeriod);
        


        // // update the time at which the board goes to sleep
        // rmsClassArg.set_toSleepEPochTime(rtcClassArg.getY2kEpoch());

        //LowPower.sleep(nextWakeUpPeriod);
        //v2 
        // Serial.print("rmsClassArg.get_toSleepEPochTime(): ");
        // Serial.println(rmsClassArg.get_toSleepEPochTime());
        // Serial.print("rmsClassArg.get_sleepPeriod(rmsClassArg.get_previousRMSState()): ");
        // Serial.println(rmsClassArg.get_sleepPeriod(rmsClassArg.get_previousRMSState()));
        // Serial.print("rmsClassArg.get_sleepPeriod(): ");
        // Serial.println(rmsClassArg.get_sleepPeriod());
        // uint32_t nextEPochWakeUpTime = rmsClassArg.get_toSleepEPochTime()
        //                             + rmsClassArg.get_sleepPeriod(rmsClassArg.get_previousRMSState())
        //                             + rmsClassArg.get_sleepPeriod();
        // Serial.print("nextEPochWakeUpTime: ");
        // Serial.println(nextEPochWakeUpTime);
        // rtcClassArg.setAlarmEpoch(nextEPochWakeUpTime);
        // if (Tool_isBitOn(triggeredInputEvent, URA_INPUTBIT)){
        //     // reset the triggered input event tracker
        //     Tool_setBitOff(&triggeredInputEvent, URA_INPUTBIT);
        // }
        // else{
        //     // update the time at which the board goes to sleep
        //     rmsClassArg.set_toSleepEPochTime(rmsClassArg.get_toSleepEPochTime()
        //                             + rmsClassArg.get_sleepPeriod(rmsClassArg.get_previousRMSState()));
        //     // rmsClassArg.set_toSleepEPochTime(rtcClassArg.getEpoch());
        // }
        

        // Serial.print("Eopoch time at sleep: ");
        // Serial.println(rmsClassArg.get_toSleepEPochTime())

        // LowPower.sleep();
        // uint32_t wakeUpEpochTime = rtcClassArg.getEpoch();
        // if (Tool_isBitOn(triggeredInputEvent, URA_INPUTBIT)){
        //     // update the time at which the board woke up
        //     rmsClassArg.set_wakeUpEPochTime(wakeUpEpochTime);

        // }
        // else{

        //     rmsClassArg.set_wakeUpEPochTime(rtcClassArg.getEpoch());
        //     // inform that RMS just woke up
        //     triggeredInputEvent |= WM_INPUTBIT; //global variable

        // }

        // Serial.print("Eopoch time at wakup: ");
        // Serial.println(rtcClassArg.getEpoch());

        // Serial.println("Woke up");

        //v3
        uint32_t nextEPochWakeUpTime = DEFAULT_EPOCHTIME + 10;
        if (Tool_isBitOn(triggeredInputEvent, URA_INPUTBIT)){
            // reset the triggered input event tracker
            Tool_setBitOff(&triggeredInputEvent, URA_INPUTBIT);
        }
        else{
            nextEPochWakeUpTime = rmsClassArg.get_wakeUpEPochTime()+
                                            rmsClassArg.get_sleepPeriod();
            rtcClassArg.setAlarmEpoch(nextEPochWakeUpTime);
        }
        Serial.print("nextEPochWakeUpTime: ");
        Serial.println(nextEPochWakeUpTime);
        Serial.print("Unix time = ");
        Serial.println(rtcClassArg.getEpoch());
        rmsClassArg.set_toSleepEPochTime(rtcClassArg.getEpoch());
        LowPower.sleep();

        if (Tool_isBitOn(triggeredInputEvent, URA_INPUTBIT)){
            // //just to do something
            // Tool_setBitOn(&triggeredInputEvent, URA_INPUTBIT);
        }
        else{
            //TODO: this section needs to be placed elesewhere: if the user presses the button when the alarm is matched, the new wakeup time will not be updated this will mean the device will never wakeup
            // update the time at which the board woke up
            rmsClassArg.set_wakeUpEPochTime(rtcClassArg.getEpoch());
            // // inform that RMS just woke up --> no nooed to: done in RTC_matchAlarmISR()
            // triggeredInputEvent |= WM_INPUTBIT; //global variable

        }
        Serial.println("");
        Serial.println("Woke up");
        Serial.print("rmsClassArg.get_wakeUpEPochTime()");
        Serial.println(rmsClassArg.get_wakeUpEPochTime());
        Serial.print("Unix time = ");
        Serial.println(rtcClassArg.getEpoch());



        

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