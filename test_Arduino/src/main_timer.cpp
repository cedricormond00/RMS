#include <Arduino.h>

#include "constant.h"
#include "Timers.h"

uint32_t freq = 2;
void setup()                    
{
    pinMode(YELLOWLED_PIN, OUTPUT);
    digitalWrite(YELLOWLED_PIN, LOW);
    Timers_tc4_init8bit(freq);
}
void loop()
{

}
