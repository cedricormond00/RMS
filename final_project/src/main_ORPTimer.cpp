#include <Arduino.h>

#include "constant.h"
#include "Global.h"

#include "Serial.h"
#include "Timer.h"
#include "I2c.h"
#include "LED.h"


// //Global variables
// extern volatile bool serialPCFlag;

//"local" variables
// timer parameters
uint32_t readInterval = 1000; //in ms
char readIntervalType = {'t'};

// serial parameters
const uint8_t PCDataLength = 20;
char PCData[PCDataLength] = {'\0'}; //we make a 20 byte character array to hold incoming data from a pc/mac/other.

char ORP_data[32];               //we make a 32 byte character array to hold incoming data from the ORP circuit.


void setup()                    
{
    //wait for serial monitor
    while (!Serial);
    // initialise 
    // i2c
    I2c_init();

    // LED pins
    pinMode(YELLOWLED_PIN, OUTPUT);
    digitalWrite(YELLOWLED_PIN, LOW);
    pinMode(ORANGELED_PIN, OUTPUT);
    digitalWrite(ORANGELED_PIN, HIGH);
    pinMode(REDLED_PIN, OUTPUT);
    delay(2000); // to have time to check on board
    digitalWrite(ORANGELED_PIN, LOW);
    delay(1000); // to have time to check on board

    // timer
    // TC4 as 16 bit
    Timer_tc4_init16bit(readInterval, readIntervalType);

    // for debug
    digitalWrite(REDLED_PIN, HIGH);
}

void loop()
{   
    // check for any serial event 
    // also updates the array with the data sent and the data sent's length
    serialEvent(PCData, PCDataLength);
    // if data sent over the serial
    if (serialPCFlag){
        // I2c_sendCommandToSensor(ORP_data, PCData, PCDataLength);
        I2c_sendReceiveORP(ORP_data, PCData, PCDataLength);
        serialPCFlag = false; // set the boolFlag to false
        ToggleLED(ORANGELED_PIN);
    }
    
    // if a timer interrupt occured, execute the command in the normal routine, when it is its turn
    if (timerFlag) {
        char command[1] = {'r'};
        // I2c_sendCommandToSensor(ORP_data, command, 1);
        I2c_sendReceiveORP(ORP_data, command, 1);

        timerFlag = false; // set the boolFlag to false
        ToggleLED(YELLOWLED_PIN);
    }
}

