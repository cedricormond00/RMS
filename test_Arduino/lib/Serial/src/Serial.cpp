#include <Arduino.h>

#include "Constant.h"
#include "Tool.h"

#include "Serial.h"
#include "Global.h" // this probably allows me to not have to include each gloab var individually

//Global variables
// extern volatile bool serialPCFlag;

void serialEvent(char computerData_[], uint8_t computerDataLength_) 
{                                                             //this interrupt will trigger when the data coming from the serial monitor(pc/mac/other) is received. 
    const bool text = false;         // If we want the full text description

    byte nCharReceivedFromComputer = 0; //we need to know how many characters have been received.
    
    nCharReceivedFromComputer = Serial.readBytesUntil(10, computerData_, computerDataLength_); //10 is Line Feed        //we read the data sent from the serial monitor(pc/mac/other) until we see a <LF>. We also count how many characters have been received.

    if (nCharReceivedFromComputer){
        computerData_[nCharReceivedFromComputer] = 0;                                       //stop the buffer from transmitting leftovers or garbage.
                                                                                        // at the end of the buffer, write a 0 to indicateit the end of data to be trasnmitted
        serialPCFlag = true;                                                            //set the serial event flag.
        if (computerData_[nCharReceivedFromComputer-1] == 13)
        {                              // deal with the CRLF exception
            computerData_[nCharReceivedFromComputer-1] = {'\0'};
        }

        if (text){
            Serial.print("nCharReceivedFromComputer: ");
            Serial.println(nCharReceivedFromComputer);
            Serial.print("computerData_: ");
            Serial.println(computerData_);        
            Serial.print("computerData_[0]: ");
            Serial.println(computerData_[0]);
            Serial.print("computerData_[0], ASCII code: ");
            Serial.println((int)computerData_[0]);
            Serial.print("computerData_[nCharReceivedFromComputer-1]: ");
            Serial.println(computerData_[nCharReceivedFromComputer-1]);
            Serial.print("computerData_[nCharReceivedFromComputer-1], ASCII code: ");
            Serial.println((int)computerData_[nCharReceivedFromComputer-1]);
            Serial.print("computerData_[nCharReceivedFromComputer]: ");
            Serial.println(computerData_[nCharReceivedFromComputer]);
            Serial.print("computerData_[nCharReceivedFromComputer], ASCII code: ");
            Serial.println((int)computerData_[nCharReceivedFromComputer]);
        }
            
    }
}


// void TextDescription(){
//   Serial.print("nCharReceivedFromComputer: ");
//   Serial.println(nCharReceivedFromComputer);
//   Serial.print("computerData: ");
//   Serial.println(computerData);        
//   Serial.print("computerData[0]: ");
//   Serial.println(computerData[0]);
//   Serial.print("computerData[0], ASCII code: ");
//   Serial.println((int)computerData[0]);
//   Serial.print("computerData[nCharReceivedFromComputer-1]: ");
//   Serial.println(computerData[nCharReceivedFromComputer-1]);
//   Serial.print("computerData[nCharReceivedFromComputer-1], ASCII code: ");
//   Serial.println((int)computerData[nCharReceivedFromComputer-1]);
//   Serial.print("computerData[nCharReceivedFromComputer]: ");
//   Serial.println(computerData[nCharReceivedFromComputer]);
//   Serial.print("computerData[nCharReceivedFromComputer], ASCII code: ");
//   Serial.println((int)computerData[nCharReceivedFromComputer]);
// }