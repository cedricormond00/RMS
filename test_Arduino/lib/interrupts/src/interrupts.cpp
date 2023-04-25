#include <Arduino.h>

#include "interrupts.h"
#include "constant.h"

// Here you may use the bp variable, whic was declared in main.cpp

void ISR (void){
    bp = true;
}
void Interrupts_init(void){
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), ISR, RISING);
    
}

