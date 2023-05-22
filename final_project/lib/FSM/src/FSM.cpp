#include <Arduino.h>
#include <ArduinoLowPower.h>


#include <iot_cmd.h>
#include <Ezo_i2c_util.h>  

#include "FSM.h"
#include "Constant.h"
#include "Global.h"
#include "LED.h"
#include "I2c.h"
#include "Tool.h"

// make it niceer with * and &
// void FSM_executeFunction(uint8_t* eventInputCode_, char ORPData[]){
void FSM_executeFunction(uint8_t* eventInputCode_, Ezo_board* EZO_ORP, RMSState* currentState){
    bool debug=true;
    if (*eventInputCode_ & WM_INPUTBIT){
        // Serial.println("in if");
        if (debug){
            Serial.println("in if");
            Serial.print("event Input Code before update: ");
            Serial.println(*eventInputCode_);
        }

        // FSM_waterMonitoring(ORPData_);
        FSM_waterMonitoring_EZO(EZO_ORP, currentState);



        Tool_setBitOff(eventInputCode_, WM_INPUTBIT); // because eventInputCode_ is already the address of the pointer
                                                    // I am now passing the correct pointer (uint8_t*) to the Tool_setBitOff
        if (debug){

            Serial.print("event Input Code after update: ");
            Serial.println(*eventInputCode_);
        }
    }

}

void FSM_waterMonitoring_EZO(Ezo_board* classArg, RMSState* currentState){
    classArg->send_read_cmd();
    delay(815); // delay required for reading command
    // TODO: eventually, will need to create my own fuction that only reads the values and that can then be used to store
    receive_and_print_reading(*classArg);
    float ORPValue = classArg->get_last_received_reading();
    if (ORPValue > 230) {
          *currentState = SWQ;
      }
      else 
      // TODO: need to deal with faulty reading as well
      {
          *currentState = UWQ;
      }
}

void FSM_goToLowPowerConsumption(uint8_t eventInputCode){
    if (eventInputCode == 0){
        // LowPower.idle();
    }
}

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




// void FSM_updateState(Ezo_board* classArg, RMSState* currentState){
//     float ORPValue = classArg->get_last_received_reading();
//     if (ORPValue > 230) {
//         digitalWrite(REDLED_PIN, LOW);
//         digitalWrite(GREENLED_PIN, HIGH);
//         rmsState = SWQ;
//     }
//     else 
//     {
//         digitalWrite(REDLED_PIN, HIGH);
//         digitalWrite(GREENLED_PIN, LOW);
//     }
// }

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
