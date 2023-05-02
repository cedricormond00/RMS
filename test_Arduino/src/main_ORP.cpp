//This code will work on an Arduino Uno and Mega
//This code was written to be easy to understand.
//Modify this code as you see fit.
//This code will output data to the Arduino serial monitor.
//Type commands into the Arduino serial monitor to control the ORP circuit.
//This code was written in the Arduino 2.0 IDE
//This code was last tested 10/2022

#include <Arduino.h>
#include <Wire.h>                //enable I2C.
#include "Ezo_i2c.h"
#define address 98               //default I2C ID number for EZO ORP Circuit.

const bool text = false;         // If we want the full text description
const unsigned int computerData_length = 20;
char computerdata[computerData_length] = {'\0'}; //= [0];           //we make a 20 byte character array to hold incoming data from a pc/mac/other.
byte received_from_computer = 0; //we need to know how many characters have been received.
byte serial_event = 0;           //a flag to signal when data has been received from the pc/mac/other.
byte code = 0;                   //used to hold the I2C response code.
char ORP_data[32];               //we make a 32 byte character array to hold incoming data from the ORP circuit.
byte in_char = 0;                //used as a 1 byte buffer to store inbound bytes from the ORP Circuit.
byte i = 0;                      //counter used for ORP_data array.
int time_ = 815;                 //used to change the delay needed depending on the command sent to the EZO Class ORP Circuit.
float ORP_float;                 //float var used to hold the float value of the ORP.


void setup()                    //hardware initialization.
{
  Serial.begin(9600);           //enable serial port.
  Serial.setTimeout(100);
  Wire.begin();                 //enable I2C port.
}

void TextDescription(){
  Serial.print("received_from_computer: ");
  Serial.println(received_from_computer);
  Serial.print("computerdata: ");
  Serial.println(computerdata);        
  Serial.print("computerdata[0]: ");
  Serial.println(computerdata[0]);
  Serial.print("computerdata[0], ASCII code: ");
  Serial.println((int)computerdata[0]);
  Serial.print("computerdata[received_from_computer-1]: ");
  Serial.println(computerdata[received_from_computer-1]);
  Serial.print("computerdata[received_from_computer-1], ASCII code: ");
  Serial.println((int)computerdata[received_from_computer-1]);
  Serial.print("computerdata[received_from_computer]: ");
  Serial.println(computerdata[received_from_computer]);
  Serial.print("computerdata[received_from_computer], ASCII code: ");
  Serial.println((int)computerdata[received_from_computer]);
}

void serialEvent() {                                                              //this interrupt will trigger when the data coming from the serial monitor(pc/mac/other) is received.
  received_from_computer = Serial.readBytesUntil(10, computerdata, computerData_length); //10 is Line Feed        //we read the data sent from the serial monitor(pc/mac/other) until we see a <LF>. We also count how many characters have been received.

  if (received_from_computer){
    computerdata[received_from_computer] = 0;                                       //stop the buffer from transmitting leftovers or garbage.
                                                                                    // at the end of the buffer, write a 0 to indicateit the end of data to be trasnmitted
    serial_event = true;                                                          //set the serial event flag.
    if (computerdata[received_from_computer-1] == 13){                              // deal with the CRLF exception
      computerdata[received_from_computer-1] = {'\0'};
    }

    if (text){
      TextDescription();
    }
        
  }
}


void loop() {   
  serialEvent();                                                                  //the main loop.
  // 2 other solutions:
  // 1-modify directly the buffer that gets populated from Serial
  // 2-modify the terminal settings such that it doesn't directly send ans ASCII code of 10 at the end
  // +investigate the serial.h
  // if ((int)computerdata[0]==10){
  //   Serial.println("came in here");
  //   serial_event = false;
  //   computerdata[0] = 0;
  // }
  if (serial_event == true) {                                                     //if a command was sent to the EZO device.
    for (i = 0; i <= received_from_computer; i++) {                               //set all char to lower case, this is just so this exact sample code can recognize the "sleep" command.
                                                                                  // <= to include the final zero (terminator)
      computerdata[i] = tolower(computerdata[i]);                                 //"Sleep" ≠ "sleep"
    }
    i=0;                                                                          //reset i, we will need it later 
    if (computerdata[0] == 'c' || computerdata[0] == 'r')time_ = 815;             //if a command has been sent to calibrate or take a reading we wait 815ms so that the circuit has time to take the reading.
    else time_ = 250;                                                             //if any other command has been sent we wait only 250ms.

    // Here we say what we will want
    Wire.beginTransmission(address);                                              //call the circuit by its ID number.
    Wire.write(computerdata);                                                     //transmit the command that was sent through the serial port.
    Wire.endTransmission();                                                       //end the I2C data transmission.

    // Here we wait + Ask + get what we want
    if (strcmp(computerdata, "sleep") != 0) {  	                                  //if the command that has been sent is NOT the sleep command, wait the correct amount of time and request data.
                                                                                  //if it is the sleep command, we do nothing. Issuing a sleep command and then requesting data will wake the circuit.
      
      delay(time_);								                                                //wait the correct amount of time for the circuit to complete its instruction.

      Wire.requestFrom(address, 32, 1); 		                                      //call the circuit and request 32 bytes (this may be more than we need)
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
        ORP_data[i] = in_char;					    //load this byte into our array.
        i += 1;                          		//incur the counter for the array element.
        if (in_char == 0) {              		//if we see that we have been sent a null command.
          i = 0;                         		//reset the counter i to 0.
          Serial.println("Received all data");
          serial_event = false;                   //reset the serial event flag.
          break;                         		//exit the while loop.
        }
      }

      Serial.println(ORP_data);          		//print the data.
    }
    
  }
  //Uncomment this section if you want to take the ORP value and convert it into floating point number.
  //ORP_float=atof(ORP_data);
}