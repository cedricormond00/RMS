#include <Arduino.h>
#include <ArduinoLowPower.h>

#include "LowPower.h"
#include "Constant.h"
// requires the global triggeredInputEvent
#include "Global.h"

#include "Tool.h"

#include "LED.h"


void LP_goToLowPowerConsumption(uint8_t inputEventCode, uint32_t wakeperiod){
    if (inputEventCode == 0){
        Serial.println("");
        Serial.println("Going to sleep");
        Serial.println("");
        LowPower.sleep(wakeperiod);
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