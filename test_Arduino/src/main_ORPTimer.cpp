#include <Arduino.h>

#include "constant.h"
#include "Global.h"

#include "Serial.h"
#include "Timer.h"
#include "I2c.h"
#include "LED.h"

//Global variables
extern volatile bool serialPCFlag;

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
    while (!Serial);
    pinMode(YELLOWLED_PIN, OUTPUT);
    digitalWrite(YELLOWLED_PIN, LOW);
    delay(2000); // to check on board

    
    Timer_tc4_init16bit(readInterval, readIntervalType); //intialise timer

    pinMode(REDLED_PIN, OUTPUT);

    digitalWrite(REDLED_PIN, HIGH);



}
void loop()
{
    Serial.println(timerFlag);
    if (timerFlag) {
        I2c_sendCommandToSensor(ORP_data, {"r"}, 1);
        timerFlag = false;
        ToggleLED(YELLOWLED_PIN);

    }

}
