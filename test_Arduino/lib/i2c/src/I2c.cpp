#include <Arduino.h>
#include <Wire.h>
//
#include "Constant.h"
#include "Global.h"

//Global variables
// extern volatile bool serialPCFlag;

void I2c_sendCommandToSensor(char ORPData_[], char command_[], uint8_t commandLength_ ){                                                  //if a command was sent to the EZO device.
    for (int i = 0; i <= commandLength_; i++) {                               //set all char to lower case, this is just so this exact sample code can recognize the "sleep" command.
                                                                                  // <= to include the final zero (terminator)
      command_[i] = tolower(command_[i]);                                 //"Sleep" ≠ "sleep"
    }
    i=0;                                                                          //reset i, we will need it later 
    if (command_[0] == 'c' || command_[0] == 'r')time_ = 815;             //if a command has been sent to calibrate or take a reading we wait 815ms so that the circuit has time to take the reading.
    else time_ = 250;                                                             //if any other command has been sent we wait only 250ms.

    // Here we say what we will want
    // I2c_sendCommand();
    Wire.beginTransmission(EZO_ADDRESS);                                              //call the circuit by its ID number.
    Wire.write(command_);                                                     //transmit the command that was sent through the serial port.
    Wire.endTransmission();     
    
    // Here we wait + Ask + get what we want
    if (strcmp(command_, "sleep") != 0) {  	                                  //if the command that has been sent is NOT the sleep command, wait the correct amount of time and request data.
                                                                                  //if it is the sleep command, we do nothing. Issuing a sleep command and then requesting data will wake the circuit.
      
        delay(time_);								                                                //wait the correct amount of time for the circuit to complete its instruction.

        Wire.requestFrom(EZOaddress, 32, 1); 		                                      //call the circuit and request 32 bytes (this may be more than we need)
        // Here we read the first byte to be sent: this contains the communicaiton code between the EZO board and the arduino
        code = Wire.read();               		                                      //the first byte is the response code, we read this separately.

        switch (code) {							          //switch case based on what the response code is.
        case 1:                         		//decimal 1.
            Serial.println("Success");        //means the command was successful.
            break;                        		//exits the switch case.

        case 2:                         		//decimal 2.
            Serial.println("Failed");     		//means the command has failed.
            break;                        		//exits the switch case.

        case 254:                       		//decimal 254.
            Serial.println("Pending");    		//means the command has not yet been finished calculating.
            break;                        		//exits the switch case.

        case 255:                       		//decimal 255.
            Serial.println("No Data");    		//means there is no further data to send.
            break;                        		//exits the switch case.
        }





      while (Wire.available()) {            //are there bytes to receive.
        in_char = Wire.read();           		//receive a byte.
        ORPData_[i] = in_char;					    //load this byte into our array.
        i += 1;                          		//incur the counter for the array element.
        if (in_char == 0) {              		//if we see that we have been sent a null command.
          i = 0;                         		//reset the counter i to 0.
          Serial.println("Received all data");
          serial_event = false;                   //reset the serial event flag.
          break;                         		//exit the while loop.
        }
      }

      Serial.println(ORPData_);          		//print the data.
    }
    
  //Uncomment this section if you want to take the ORP value and convert it into floating point number.
  //ORP_float=atof(ORPData_);
  }

//   void I2c_sendCommand(uint8_t i2cAddress_, char textSent_, uint8_t textLength_){
//     Wire.beginTransmission(i2cAddress_);                                              //call the circuit by its ID number.
//     Wire.write(command_);                                                     //transmit the command that was sent through the serial port.
//     Wire.endTransmission();                                                       //end the I2C data transmission.
//   }