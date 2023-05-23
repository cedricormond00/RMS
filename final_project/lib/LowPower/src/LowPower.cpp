#include <Arduino.h>
#include <ArduinoLowPower.h>

#include "LowPower.h"
#include "Constant.h"
// requires the global triggeredInputEvent
#include "Global.h"

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

    LowPower.attachInterruptWakeup(BUTTON_PIN, LP_callbackURA, RISING);
}

void LP_callbackURA(){
    triggeredInputEvent |= URA_INPUTBIT; //global variable

}