/*
  Read battery voltage on MKR WIFI 1010 and log values to the serial monitor


                            ┌─────── VBatt
                            │
                            /
                            \  330k
  ┌────────────┐             /
  │   SAMD21   │             \
  │            │             │
  │       PB09 ├────┬────────┤
  │            │    │        │
  │            │    │ 100n   /
  │            │  ──┴───     \  1M
  │            │  ──┬───     /
  │            │    |        \
  │            │    │        |
  │            │    │        │
  └─────┬──────┘    |        |
        │           |        |
        ▼           ▼        ▼ 



  Author: Ali Jahangiri & Karl Soderby

  Last Edit: 9th August 2022
*/

#include <Arduino.h>

// Include the library for the BQ24195 IC
#include <Arduino_PMIC.h>
#include <ArduinoLowPower.h>

#include "Battery.h"

int usb_mode = UNKNOWN_MODE;



//define the resistor values in the voltage divider
  
const uint32_t R1 =  330000;       // resistor between battery terminal and SAMD pin PB09
const uint32_t R2 = 1000000;       // resistor between SAMD pin PB09 and ground

// define voltage at which battery is full/empty
const float batteryFullVoltage = 4.2;   //upper voltage limit for battery
const float batteryEmptyVoltage = 3.5;  //lower voltage limit for battery

const float batteryCapacity = 2.9;            //set battery capacity in Ah
        



float Battery_getBatteryVoltage(void){
    analogReference(AR_DEFAULT);      // the upper value of ADC is set to 3.3V
    analogReadResolution(12);         // this will give us 4096 (2^12) levels on the ADC
    
    // The formula for calculating the output of a voltage divider is
    // Vout = (Vsource x R2)/(R1 + R2)
    // If we consider that 3.3V is the maximum that can be applied to Vout then the maximum source voltage is calculated as
    // upper source voltage for the battery
    uint8_t maxSourceVoltage = (3.3 * (R1 + R2))/R2;

    //unprocessed ADC value
    float rawADC = analogRead(ADC_BATTERY);                     //the value obtained directly at the PB09 input pin
    //ADC converted into voltage
    float voltADC = rawADC * (3.3/4095.0);                      //convert ADC value to the voltage read at the pin
    //calculated voltage on battery
    float voltBat = voltADC * (maxSourceVoltage/3.3);         //we cannot use map since it requires int inputs/outputs
    // //report information over Serial
    // Serial.begin(9600);               // start Serial port with a baudrate of 9600bps
    // Serial.print("The ADC on PB09 reads a value of ");
    // Serial.print(rawADC);
    // Serial.print(" which is equivialent to ");
    // Serial.print(voltADC);
    // Serial.print("V. This means the battery voltage is ");
    // Serial.print(voltBat);
    // Serial.print("V. Which is equivalent to a charge level of ");
    // Serial.print(Battery_getBatteryPerc(voltBat));
    // Serial.println("%.");
    return voltBat;
}

uint8_t Battery_getBatteryPercentage(uint8_t batteryVoltage){
    uint8_t batteryPercentage = (batteryVoltage - batteryEmptyVoltage) * (100) / (batteryFullVoltage - batteryEmptyVoltage);    //custom float friendly map function
    return batteryPercentage;
}

bool Battery_getIsStablePowerSupply(){
    uint8_t vBusStatus = PMIC.isPowerGood();
    return vBusStatus;
}

uint8_t Battery_getChargeStatus(){
    uint8_t vBusStatus = PMIC.chargeStatus();
    return vBusStatus;
}