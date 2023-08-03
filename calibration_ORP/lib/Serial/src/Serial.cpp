#include <Arduino.h>

#include "Constant.h"
#include "Tool.h"

#include "Serial.h"
#include "Global.h" // this probably allows me to not have to include each gloab var individually

//Global variables
// extern volatile bool serialPCFlag;

void TextDescription(char computerData_[], uint8_t nCharReceivedFromComputer_){
  Serial.print("nCharReceivedFromComputer_: ");
  Serial.println(nCharReceivedFromComputer_);
  Serial.print("computerData_: ");
  Serial.println(computerData_);        
  Serial.print("computerData_[0]: ");
  Serial.println(computerData_[0]);
  Serial.print("computerData_[0], ASCII code: ");
  Serial.println((int)computerData_[0]);
  Serial.print("computerData_[nCharReceivedFromComputer-1]: ");
  Serial.println(computerData_[nCharReceivedFromComputer_-1]);
  Serial.print("computerData_[nCharReceivedFromComputer-1], ASCII code: ");
  Serial.println((int)computerData_[nCharReceivedFromComputer_-1]);
  Serial.print("computerData_[nCharReceivedFromComputer]: ");
  Serial.println(computerData_[nCharReceivedFromComputer_]);
  Serial.print("computerData_[nCharReceivedFromComputer], ASCII code: ");
  Serial.println((int)computerData_[nCharReceivedFromComputer_]);
  Serial.println("");
}

void serialEvent(char computerData_[], uint8_t computerDataLength_) 
{                                                             //this interrupt will trigger when the data coming from the serial monitor(pc/mac/other) is received. 
    const bool text = 0;         // If we want the full text description

    byte nCharReceivedFromComputer = 0; //we need to know how many characters have been received.
    
    nCharReceivedFromComputer = Serial.readBytesUntil(10, computerData_, computerDataLength_); //10 is Line Feed        //we read the data sent from the serial monitor(pc/mac/other) until we see a <LF>. We also count how many characters have been received.
    if (text){
        Serial.println("1");
        TextDescription(computerData_, nCharReceivedFromComputer);
    }
    if (nCharReceivedFromComputer){
        computerData_[nCharReceivedFromComputer] = 0;                                       //stop the buffer from transmitting leftovers or garbage.
                                                                                        // at the end of the buffer, write a 0 to indicateit the end of data to be trasnmitted
        serialPCFlag = true;                                                            //set the serial event flag.
        if (computerData_[nCharReceivedFromComputer-1] == 13)
        {                              // deal with the CRLF exception
            computerData_[nCharReceivedFromComputer-1] = {'\0'};
        }

        if (text){
            Serial.println("2");
            TextDescription(computerData_, nCharReceivedFromComputer);
        }
            
    }
}
