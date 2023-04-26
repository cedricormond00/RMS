#include <Arduino.h>

#include "interrupts.h"
#include "constant.h"
#include "LED.h"

// Here you may use the bp variable, whic was declared in main.cpp

void ISR_buttonPress (void){
    bp = true;
    Serial.println();
    Serial.println("in ISR");
    Serial.print("bp:");
    Serial.println(bp);
    Serial.println();


    //ToggleLED(ORANGELED_PIN);

}
void Interrupts_init(void){
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), ISR_buttonPress, RISING);
    
}

