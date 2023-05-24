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
    bool debug=false;
    if (*eventInputCode_ & URA_INPUTBIT){
        // Serial.println("in if");
        if (debug){
            Serial.println("in FSM_executeFunction, URA");
            Serial.print("event Input Code before update: ");
            Serial.print(*eventInputCode_, BIN);
            Serial.print(", ");
            Serial.println(*eventInputCode_);
        }

        Serial.print("milis after interrupt wakup: ");
        Serial.println(millis());

        FSM_f_URA();
        Tool_setBitOff(eventInputCode_, URA_INPUTBIT); // because eventInputCode_ is already the address of the pointer
                                                    // I am now passing the correct pointer (uint8_t*) to the Tool_setBitOff
        if (debug){

            Serial.print("event Input Code after update: ");
            Serial.print(*eventInputCode_, BIN);
            Serial.print(", ");
            Serial.println(*eventInputCode_);
        }
    }
    if (*eventInputCode_ & WM_INPUTBIT){
        // Serial.println("in if");
        if (debug){
            Serial.println("in FSM_executeFunction, WM");
            Serial.print("event Input Code before update: ");
            Serial.print(*eventInputCode_, BIN);
            Serial.print(", ");
            Serial.println(*eventInputCode_);

        }

        // FSM_waterMonitoring(ORPData_);
        FSM_f_WaterMonitoring_EZO(EZO_ORP, currentState);



        Tool_setBitOff(eventInputCode_, WM_INPUTBIT); // because eventInputCode_ is already the address of the pointer
                                                    // I am now passing the correct pointer (uint8_t*) to the Tool_setBitOff
        if (debug){
            Serial.println("");
            Serial.print("event Input Code after update: ");
            Serial.print(*eventInputCode_, BIN);
            Serial.print(", ");
            Serial.println(*eventInputCode_);
        }
    }
    if (*eventInputCode_ & BUP_INPUTBIT){
        // Serial.println("in if");
        if (debug){
            Serial.println("in FSM_executeFunction, BUP");
            Serial.print("event Input Code before update: ");
            Serial.print(*eventInputCode_, BIN);
            Serial.print(", ");
            Serial.println(*eventInputCode_);
        }

        // FSM_BUPMONITORINGFUNCTION


        Tool_setBitOff(eventInputCode_, BUP_INPUTBIT); // because eventInputCode_ is already the address of the pointer
                                                    // I am now passing the correct pointer (uint8_t*) to the Tool_setBitOff
        if (debug){

            Serial.print("event Input Code after update: ");
            Serial.print(*eventInputCode_, BIN);
            Serial.print(", ");
            Serial.println(*eventInputCode_);
        }
    }

    if (debug){
        // Serial.println("");
    }

    


}

void FSM_f_WaterMonitoring_EZO(Ezo_board* classArg, RMSState* currentState){
    FSM_getEzoWaterReading(classArg);
    // classArg->send_read_cmd();
    // delay(815); // delay required for reading command
    // // TODO: eventually, will need to create my own fuction that only reads the values and that can then be used to store
    // receive_and_print_reading(*classArg);
    float ORPValue = classArg->get_last_received_reading();
    if (ORPValue > 400) {
          *currentState = SWQ;
      }
      else 
      // TODO: need to deal with faulty reading as well
      {
          *currentState = UWQ;
      }
}

void FSM_getEzoWaterReading(Ezo_board* classArg){
    classArg->send_read_cmd();
    delay(815); // delay required for reading command
    // TODO: eventually, will need to create my own fuction that only reads the values and that can then be used to store
    receive_and_print_reading(*classArg);
}

void FSM_f_URA(){
    ToggleLED(YELLOWLED_PIN);

}

void FSM_updateInputEventCode(uint8_t *eventInputCode, volatile uint8_t* triggeredInputEvent)//input arguments: relevant function counter
{
    bool debug=false;
    if (debug){
        Serial.println("in FSM_updateInputEventCode, function");
        Serial.print("eventInputCode before update: ");
        Serial.print(*eventInputCode, BIN);
        Serial.print(", ");
        Serial.println(*eventInputCode);
        Serial.print("triggeredInputEvent before update: ");
        Serial.print(*triggeredInputEvent, BIN);
        Serial.print(", ");
        Serial.println(*triggeredInputEvent);

    }

    if (Tool_isBitOn(*triggeredInputEvent, URA_INPUTBIT)) 
    {
        if (debug){
            Serial.println("in FSM_updateInputEventCode, URA");
            Serial.print("eventInputCode before update: ");
            Serial.print(*eventInputCode, BIN);
            Serial.print(", ");
            Serial.println(*eventInputCode);
            Serial.print("triggeredInputEvent before update: ");
            Serial.print(*triggeredInputEvent, BIN);
            Serial.print(", ");
            Serial.println(*triggeredInputEvent);

        }
        // ToggleLED(YELLOWLED_PIN);
        //because we haven't yet reached the 3 seconds
        Tool_setBitOn(eventInputCode, URA_WAIT_INPUTBIT);// This prevents to go back to sleep
        // Because we have come out of sleep becuase of URA, we don't necessarily want to perform a WM
        Tool_setBitOff(eventInputCode, WM_INPUTBIT);// code for WM function
        // reset the triggered input event tracker
        Tool_setBitOff(triggeredInputEvent, WM_INPUTBIT); // because we have already told the FSm to do a WM
        
        // check the button is still pressed 
        if (digitalRead(BUTTON_PIN)==LOW){
            if (millis()>=(millisOnExternalWakeUp + 3000)){
                //inform the RMS the URA function may be performed
                Tool_setBitOn(eventInputCode, URA_INPUTBIT);// code for UserraisedAlarm function
                Tool_setBitOff(eventInputCode, URA_WAIT_INPUTBIT);

                // reset the triggered input event tracker
                Tool_setBitOff(triggeredInputEvent, URA_INPUTBIT);

            }
        }
        else { // the button has been released
            // no more URA alarm
            Tool_setBitOff(triggeredInputEvent, URA_INPUTBIT);
            // no need to inform that we shall wait for 3 seconds
            Tool_setBitOff(eventInputCode, URA_WAIT_INPUTBIT);
        }
        
        


        
        if (debug){
            Serial.print("eventInputCode after update: ");
            Serial.print(*eventInputCode, BIN);
            Serial.print(", ");
            Serial.println(*eventInputCode);
            Serial.print("triggeredInputEvent after update: ");
            Serial.print(*triggeredInputEvent, BIN);
            Serial.print(", ");
            Serial.println(*triggeredInputEvent);
        }
    }
    if (Tool_isBitOn(*triggeredInputEvent, WM_INPUTBIT)) 
    {
        if (debug){
            Serial.println("in FSM_updateInputEventCode, WM");
            Serial.print("eventInputCode before update: ");
            Serial.print(*eventInputCode, BIN);
            Serial.print(", ");
            Serial.println(*eventInputCode);
            Serial.print("triggeredInputEvent before update: ");
            Serial.print(*triggeredInputEvent, BIN);
            Serial.print(", ");
            Serial.println(*triggeredInputEvent);
        }
        // ToggleLED(YELLOWLED_PIN);
        Tool_setBitOn(eventInputCode, WM_INPUTBIT);// code for WM function

        // reset the triggered input event tracker
        Tool_setBitOff(triggeredInputEvent, WM_INPUTBIT);

         if (debug){
            Serial.print("eventInputCode after update: ");
            Serial.print(*eventInputCode, BIN);
            Serial.print(", ");
            Serial.println(*eventInputCode);
            Serial.print("triggeredInputEvent after update: ");
            Serial.print(*triggeredInputEvent, BIN);
            Serial.print(", ");
            Serial.println(*triggeredInputEvent);
        }

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
