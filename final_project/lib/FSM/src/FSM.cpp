#include <Arduino.h>
#include <FSM.h>

#include "Constant.h"
#include "Global.h"
#include "LED.h"
#include "I2c.h"
#include "Tool.h"

void FSM_updateEventInputCode(uint8_t *eventInputCode, uint16_t waterMonitoringPeriod)//input arguments: relevant function counter
{
    if (timerFlag){
        timerFlag = 0;


        if (WMTC >= waterMonitoringPeriod) // if reach the watermonitoringPeriod, 
        {
            ToggleLED(ORANGELED_PIN);
            *eventInputCode |= 0b1; // code for WM function
            // reset the water monitoring timer count to zero
            WMTC = 0;
        }


    }
}

// make it niceer with * and &
void FSM_executeFunction(uint8_t* eventInputCode_, char ORPData_[]){
    if (*eventInputCode_ & WM_INPUTBIT){

        FSM_waterMonitoring(ORPData_);


        Serial.println(*eventInputCode_);
        Tool_setBitOff(eventInputCode_, WM_INPUTBIT); // because eventInputCode_ is already the address of the pointer
                                                    // I am now passing the correct pointer (uint8_t*) to the Tool_setBitOff
        Serial.println(*eventInputCode_);

    }

}

void FSM_waterMonitoring(char ORPData_[]){
    uint8_t commandLength = 1;
    char ORPCommand[commandLength] = {'r'};
    

    I2c_sendReceiveORP(ORPData_, ORPCommand, commandLength);
    Serial.print("ORP value: ");
    Serial.println(ORPData_);


}
