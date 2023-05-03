#include <Arduino.h>

#include "constant.h"
#include "Timers.h"


void setup()                    
{
    pinMode(YELLOWLED_PIN, OUTPUT);
    digitalWrite(YELLOWLED_PIN, LOW);
    delay(2000);
    // uint32_t freq = 10;
    // Timers_tc4_init8bit(freq);

    // Timers_tc4_init8bit(1024, 46875);

    Timers_tc4_init16bit_t(1100);

}
void loop()
{

}
