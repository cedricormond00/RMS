#include <Arduino.h>
#include <FSM.h>

#include <iot_cmd.h>
#include <Ezo_i2c_util.h>  


#include "Constant.h"
#include "Global.h"
#include "LED.h"
#include "I2c.h"
#include "Tool.h"

void FSM_updateEventInputCode(uint8_t *eventInputCode, uint16_t waterMonitoringPeriod)//input arguments: relevant function counter
{
    if (WMTC >= waterMonitoringPeriod) // if reach the watermonitoringPeriod, 
    {
        ToggleLED(ORANGELED_PIN);
        *eventInputCode |= 0b1; // code for WM function
        // reset the water monitoring timer count to zero
        WMTC = 0;

    }
}

// make it niceer with * and &
void FSM_executeFunction(uint8_t* eventInputCode_, char ORPData_[], Ezo_board* EZO_ORP){
    bool debug=false;
    if (*eventInputCode_ & WM_INPUTBIT){
        // Serial.println("in if");
        if (debug){
            Serial.println("in if");
            Serial.print("event Input Code before update: ");
            Serial.println(*eventInputCode_);
        }

        // FSM_waterMonitoring(ORPData_);
        FSM_waterMonitoring_EZO(EZO_ORP);



        Tool_setBitOff(eventInputCode_, WM_INPUTBIT); // because eventInputCode_ is already the address of the pointer
                                                    // I am now passing the correct pointer (uint8_t*) to the Tool_setBitOff
        if (debug){

            Serial.print("event Input Code after update: ");
            Serial.println(*eventInputCode_);
        }
    }

}

void FSM_waterMonitoring_EZO(Ezo_board* classArg){
    classArg->send_read_cmd();
    delay(1000);
    receive_and_print_reading(*classArg);




}



void FSM_waterMonitoring(char ORPData_[]){
    bool debug = false;
    uint8_t commandLength = 1;
    
    char ORPCommand[commandLength] = {'r'};
    

    I2c_sendReceiveORP(ORPData_, ORPCommand, commandLength);
    if (debug){
        Serial.print("ORP value: ");
        Serial.println(ORPData_);


    }
    


}

    // if (eventInputCode & WM_INPUTBIT) {
    //     FSM_waterMonitoring(ORPData);
    //     Tool_setBitOff(&eventInputCode, WM_INPUTBIT);
    //   }