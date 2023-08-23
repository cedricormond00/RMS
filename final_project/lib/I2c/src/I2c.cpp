#include <Arduino.h>
#include <Wire.h>
//
#include "Constant.h"
#include "Global.h"
#include "I2c.h"

//Global variables
// extern volatile bool serialPCFlag;
void I2c_init(){
  Wire.begin();
}



// int I2c_checkResponseDelay(char command_[], uint8_t commandLength_){
//   int responseDelay = 815;      //used to change the delay needed depending on the command sent to the EZO Class ORP Circuit.

//   for (int i = 0; i <= commandLength_; i++) {                               //set all char to lower case, this is just so this exact sample orpResponseCode can recognize the "sleep" command.
//                                                                                 // <= to include the final zero (terminator)
//     command_[i] = tolower(command_[i]);                                 //"Sleep" ≠ "sleep"
//   }

//   if (command_[0] == 'c' || command_[0] == 'r'){
//     responseDelay = 815;
//     return responseDelay;             //if a command has been sent to calibrate or take a reading we wait 815ms so that the circuit has time to take the reading.
//   }
//   else {
//     responseDelay = 250;   
//     return responseDelay;
//   }
// } 

// void I2c_sendCommand(uint8_t i2cAddress, const char *command_){
//   // noInterrupts();
//   Wire.beginTransmission(i2cAddress);                                              //call the circuit by its ID number.
//   Wire.write(command_);                                                     //transmit the command that was sent through the serial port.
//   Wire.endTransmission();  
//   // interrupts();
// }




// void I2c_sendCommandORP(char command_[]){
//   // byte orpInChar = 0;     //used as a 1 byte buffer to store inbound bytes from the ORP Circuit.
//                                                 //if any other command has been sent we wait only 250ms.
//   bool debug = false;
//   if (debug){
//     Serial.print("I2c_sendCommandORP command_: ");
//     Serial.println(command_);
//   }
//   // Here we say what we will want
//   I2c_sendCommand(EZO_ADDRESS, command_);

// }

// // void I2c_receiveORPData(char ORPData_[], char command_[], int responseDelay){
// // // Here we wait + Ask + get what we want
// //   byte orpResponseCode = 0;   //used to hold the I2C response code, from the ORP.
// //   byte orpInChar = 0;         //used as a 1 byte buffer to store inbound bytes from the ORP Circuit.

// //   if (strcmp(command_, "sleep") != 0) {   //if the command that has been sent is NOT the sleep command, wait the correct amount of time and request data.
// //                                           //if it is the sleep command, we do nothing. Issuing a sleep command and then requesting data will wake the circuit.
      
// //     delay(responseDelay);								                                                //wait the correct amount of time for the circuit to complete its instruction.

// //     Wire.requestFrom(EZO_ADDRESS, 32, 1); 		                                      //call the circuit and request 32 bytes (this may be more than we need)
// //     // Here we read the first byte to be sent: this contains the communicaiton orpResponseCode between the EZO board and the arduino
// //     orpResponseCode = Wire.read();               		                                      //the first byte is the response orpResponseCode, we read this separately.

// //     switch (orpResponseCode) {							          //switch case based on what the response orpResponseCode is.
// //       case 1:                         		//decimal 1.
// //           Serial.println("Success");        //means the command was successful.
// //           break;                        		//exits the switch case.

// //       case 2:                         		//decimal 2.
// //           Serial.println("Failed");     		//means the command has failed.
// //           break;                        		//exits the switch case.

// //       case 254:                       		//decimal 254.
// //           Serial.println("Pending");    		//means the command has not yet been finished calculating.
// //           break;                        		//exits the switch case.

// //       case 255:                       		//decimal 255.
// //           Serial.println("No Data");    		//means there is no further data to send.
// //           break;                        		//exits the switch case.
// //       }



// //     int i = 0;
// //     while (Wire.available()) {            //are there bytes to receive.
// //       orpInChar = Wire.read();           		//receive a byte.
// //       ORPData_[i] = orpInChar;					    //load this byte into our array.
// //       i += 1;                          		//incur the counter for the array element.
// //       if (orpInChar == 0) {              		//if we see that we have been sent a null command.
// //         i = 0;                         		//reset the counter i to 0.
// //         Serial.println("Received all data");
// //         serialPCFlag = false;                   //reset the serial event flag.
// //         break;                         		//exit the while loop.
// //       }
// //     }

// //     Serial.println(ORPData_);          		//print the data.
// //     Serial.println("");
// //     }
    
// //   //Uncomment this section if you want to take the ORP value and convert it into floating point number.
// //   //ORP_float=atof(ORPData_);
// //   }

// void I2c_sendReceiveORP(char ORPData_[], char command_[], uint8_t commandLength_){
//   int responseDelay = 815;      //used to change the delay needed depending on the command sent to the EZO Class ORP Circuit.
//   bool debug = false;
//   if(debug){
//     Serial.print("command_");
//     Serial.println(command_);
//   }
//   I2c_sendCommandORP(command_);
//   if(debug){
//     Serial.print("responseDelay");
//     Serial.println(responseDelay);
//   }
//   responseDelay = I2c_checkResponseDelay(command_, commandLength_);
//   if(debug){
//     Serial.print("responseDelay");
//     Serial.println(responseDelay); 
//   }
//   I2c_receiveORPData(ORPData_, command_, responseDelay);
// }




















// void I2c_sendCommandToSensor(char ORPData_[], char command_[], uint8_t commandLength_ )
// {                                                  //if a command was sent to the EZO device.
//     bool debug = false;
    
//     byte orpResponseCode = 0;        //used to hold the I2C response code, from the ORP.
//     int responseDelay = 815;      //used to change the delay needed depending on the command sent to the EZO Class ORP Circuit.
//     byte orpInChar = 0;     //used as a 1 byte buffer to store inbound bytes from the ORP Circuit.
    


//     if (debug){   
//       Serial.println(command_);
//       Serial.println(commandLength_);
//       Serial.println(ORPData_);
//     }


//     int i = 0;
//     for (i = 0; i <= commandLength_; i++) {                               //set all char to lower case, this is just so this exact sample orpResponseCode can recognize the "sleep" command.
//                                                                                   // <= to include the final zero (terminator)
//       command_[i] = tolower(command_[i]);                                 //"Sleep" ≠ "sleep"
//     }
//     i=0;                                                                          //reset i, we will need it later 

//     if (command_[0] == 'c' || command_[0] == 'r')responseDelay = 815;             //if a command has been sent to calibrate or take a reading we wait 815ms so that the circuit has time to take the reading.
//     else responseDelay = 250;                                                             //if any other command has been sent we wait only 250ms.
//     if (debug){   
//       Serial.println(responseDelay);
//       Serial.println("send command");
//       Serial.println(command_[0]);
//     }    // Here we say what we will want
//     // I2c_sendCommand();
//     Wire.beginTransmission(EZO_ADDRESS);                                              //call the circuit by its ID number.
//     if (debug){   
//       Serial.println("address found");
//       Serial.println(EZO_ADDRESS);
//     }
//     Wire.write(command_);                                                     //transmit the command that was sent through the serial port.
//     if (debug){   
//       Serial.println("command sent");
//       Serial.println(command_);
//     }
//     Wire.endTransmission();  
//     if (debug){   
//       Serial.println("transmission end");
//       // Serial.println(err);
//     }
//     // Here we wait + Ask + get what we want
//     if (strcmp(command_, "sleep") != 0) {  	                                  //if the command that has been sent is NOT the sleep command, wait the correct amount of time and request data.
//                                                                                   //if it is the sleep command, we do nothing. Issuing a sleep command and then requesting data will wake the circuit.
      
//         delay(responseDelay);								                                                //wait the correct amount of time for the circuit to complete its instruction.

//         Wire.requestFrom(EZO_ADDRESS, 32, 1); 		                                      //call the circuit and request 32 bytes (this may be more than we need)
//         // Here we read the first byte to be sent: this contains the communicaiton orpResponseCode between the EZO board and the arduino
//         orpResponseCode = Wire.read();               		                                      //the first byte is the response orpResponseCode, we read this separately.

//         switch (orpResponseCode) {							          //switch case based on what the response orpResponseCode is.
//         case 1:                         		//decimal 1.
//             Serial.println("Success");        //means the command was successful.
//             break;                        		//exits the switch case.

//         case 2:                         		//decimal 2.
//             Serial.println("Failed");     		//means the command has failed.
//             break;                        		//exits the switch case.

//         case 254:                       		//decimal 254.
//             Serial.println("Pending");    		//means the command has not yet been finished calculating.
//             break;                        		//exits the switch case.

//         case 255:                       		//decimal 255.
//             Serial.println("No Data");    		//means there is no further data to send.
//             break;                        		//exits the switch case.
//         }




//       while (Wire.available()) {            //are there bytes to receive.
//         orpInChar = Wire.read();           		//receive a byte.
//         ORPData_[i] = orpInChar;					    //load this byte into our array.
//         i += 1;                          		//incur the counter for the array element.
//         if (orpInChar == 0) {              		//if we see that we have been sent a null command.
//           i = 0;                         		//reset the counter i to 0.
//           Serial.println("Received all data");
//           // should be able to delete this line
//           serialPCFlag = false;                   //reset the serial event flag.
//           break;                         		//exit the while loop.
//         }
//       }

//       Serial.println(ORPData_);          		//print the data.
//       Serial.println("");
//     }
    
//   //Uncomment this section if you want to take the ORP value and convert it into floating point number.
//   //ORP_float=atof(ORPData_);
//   }

// // void I2c_sendCommand(uint8_t i2cAddress_, char textSent_)
// // {
// //   // ensure you intialise the I2C communication
// //   Wire.beginTransmission(i2cAddress_);                                              //call the circuit by its ID number.
// //   Wire.write(textSent_);                                                     //transmit the command that was sent through the serial port.
// //   Wire.endTransmission();                                                       //end the I2C data transmission.
// // }