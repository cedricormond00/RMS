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

bool irq4 = false;
bool irq5 = false;
void setup()                    
{
    while (!Serial);
    I2c_init();
    pinMode(YELLOWLED_PIN, OUTPUT);
    digitalWrite(YELLOWLED_PIN, LOW);
    pinMode(ORANGELED_PIN, OUTPUT);
    digitalWrite(ORANGELED_PIN, LOW);
    delay(2000); // to check on board

    
    Timer_tc4_init16bit(readInterval, readIntervalType); //intialise timer

    pinMode(REDLED_PIN, OUTPUT);

    digitalWrite(REDLED_PIN, HIGH);



}
void loop()
{   
    delay(500);
    
    serialEvent(PCData, PCDataLength);
    if (serialPCFlag){
        I2c_sendCommandToSensor(ORP_data, PCData, PCDataLength);
        serialPCFlag = false;
        ToggleLED(ORANGELED_PIN);

    }
    
    if (timerFlag) {
        char command[1] = {'r'};
        I2c_sendCommandToSensor(ORP_data, command, 1);
        timerFlag = false;
        ToggleLED(YELLOWLED_PIN);
    }

}
