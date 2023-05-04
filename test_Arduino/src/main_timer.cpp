#include <Arduino.h>

#include "constant.h"
#include "Timer.h"


void setup()                    
{
    while (!Serial);
    pinMode(YELLOWLED_PIN, OUTPUT);
    digitalWrite(YELLOWLED_PIN, LOW);
    delay(2000);
    // uint32_t freq = 10;
    // Timers_tc4_init8bit(freq);

    // Timers_tc4_init8bit(1024, 46875);

    // Timers_tc4_init16bit_t(1100);

    uint32_t value = 1000; //in ms
    char valueType = {'t'};
    Timer_tc4_init16bit(value, valueType);


}
void loop()
{

}
