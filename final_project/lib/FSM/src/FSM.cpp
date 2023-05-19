#include <Arduino.h>
#include <FSM.h>

#include "Constant.h"
#include "Global.h"
#include "LED.h"

void FSM_updateEventCounters()//input arguments: relevant function counter
{
    if (timerFlag){
        timerFlag = 0;
        WMTC += timeIncrementS;

        if (WMTC >= WMTC_limit){
            ToggleLED(ORANGELED_PIN);
            WMTC = 0;
        }


    }
}