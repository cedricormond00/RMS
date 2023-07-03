#include <Arduino.h>
#include "LED.h"
#include "Constant.h"
// #include "template.h"

#define LED_DELAY 100


void LED_runInitBeginSignal(){
  digitalWrite(BLUELED_PIN, LOW);
  digitalWrite(REDLED_PIN, LOW);
  digitalWrite(GREENLED_PIN, LOW);
  digitalWrite(YELLOWLED_PIN, LOW);
  digitalWrite(ORANGELED_PIN, LOW);
  
  delay(LED_DELAY);
  
  digitalWrite(BLUELED_PIN, HIGH);
  digitalWrite(REDLED_PIN, LOW);
  digitalWrite(GREENLED_PIN, LOW);
  digitalWrite(YELLOWLED_PIN, LOW);
  digitalWrite(ORANGELED_PIN, LOW);

  delay(LED_DELAY);

  digitalWrite(BLUELED_PIN, HIGH);
  digitalWrite(REDLED_PIN, HIGH);
  digitalWrite(GREENLED_PIN, LOW);
  digitalWrite(YELLOWLED_PIN, LOW);
  digitalWrite(ORANGELED_PIN, LOW);

  delay(LED_DELAY);

  digitalWrite(BLUELED_PIN, HIGH);
  digitalWrite(REDLED_PIN, HIGH);
  digitalWrite(GREENLED_PIN, HIGH);
  digitalWrite(YELLOWLED_PIN, LOW);
  digitalWrite(ORANGELED_PIN, LOW);

  delay(LED_DELAY);

  digitalWrite(BLUELED_PIN, HIGH);
  digitalWrite(REDLED_PIN, HIGH);
  digitalWrite(GREENLED_PIN, HIGH);
  digitalWrite(YELLOWLED_PIN, HIGH);
  digitalWrite(ORANGELED_PIN, LOW);

  delay(LED_DELAY);
  
  digitalWrite(BLUELED_PIN, HIGH);
  digitalWrite(REDLED_PIN, HIGH);
  digitalWrite(GREENLED_PIN, HIGH);
  digitalWrite(YELLOWLED_PIN, HIGH);
  digitalWrite(ORANGELED_PIN, HIGH);

  delay(LED_DELAY);
  
  digitalWrite(BLUELED_PIN, LOW);
  digitalWrite(REDLED_PIN, LOW);
  digitalWrite(GREENLED_PIN, LOW);
  digitalWrite(YELLOWLED_PIN, LOW);
  digitalWrite(ORANGELED_PIN, LOW);

}

void LED_runInitCompleteSignal(){
  for (int i=0; i<10; i++){ 
    ToggleLED(BLUELED_PIN);
    delay(100);
  }
  digitalWrite(BLUELED_PIN, LOW);
}


void ToggleLED(unsigned int pin){
  digitalWrite(pin, !digitalRead(pin));
  bool debug = false;
  if (debug){
    Serial.println("TOGGLING LED");
    Serial.print("LED status: ");
    Serial.println(digitalRead(pin));
  }
}



void multipin_HL(unsigned int pin_arr[], unsigned int arraySize, unsigned int status)
{
  // unsigned int n_arraySize = sizeof(n_pinAllLEDs) / sizeof(n_pinAllLEDs[0]);
  // Serial.println(n_pinAllLEDs[2]);
  //char c_arraySize[100];
  //Serial.print(sprintf(c_arraySize, "array size %u", n_arraySize));
  // Serial.print("array size");
  // Serial.println(n_arraySize);
  for (unsigned int i = 0; i < arraySize; i++) 
  {
      digitalWrite(pin_arr[i], status);
      Serial.print("Toggle pin ");
      Serial.println(pin_arr[i]);
  }
}

void multipin_pinMode(unsigned int pin_array[], unsigned int arraySize, unsigned int mode)
{
  for (unsigned int currPin = 0; currPin < arraySize; currPin++) 
  {
    pinMode(currPin, mode);
  }
}



// void multipin_pinMode_s(unsigned int (&pin_array)[], unsigned int mode)
// {
//   unsigned int *arraypx0; //pointer to the first eleemnt of the array
//   arraypx0 = &allLEDpin_array[0]; // adress of the first array element
//   Serial.print("First array element : ");
//   Serial.println(*arraypx0); // print the content at address pointed to by arraypx0
//   unsigned int *arraypx = arraypx0 + 1;//sizeof(*arraypx0); //pointer to the second element in the address
//   Serial.print("Second array element : ");
//   Serial.println(*arraypx);
//   unsigned int arraySize = sizeof(pin_array) / sizeof(pin_array[0]);
//   //size_t arraySize = array_length(pin_array);

//   for (unsigned int currPin = 0; currPin < arraySize; currPin++) 
//   {
//     pinMode(currPin, mode);
//   }
// }



// about battery -> orangeLED
void LED_showBatteryLowSignal(){
  digitalWrite(ORANGELED_PIN, HIGH);
  digitalWrite(YELLOWLED_PIN, LOW);

  delay(100);

  for (int i = 0; i<4; i++){
    ToggleLED(YELLOWLED_PIN);
    delay(100);
  }

  digitalWrite(ORANGELED_PIN, LOW);
  digitalWrite(YELLOWLED_PIN, LOW);
}

void LED_showBatteryNotConnectedSignal(){
  digitalWrite(ORANGELED_PIN, HIGH);
  digitalWrite(REDLED_PIN, LOW);

  delay(100);

  for (int i = 0; i<4; i++){
    ToggleLED(REDLED_PIN);
    delay(100);
  }

  digitalWrite(ORANGELED_PIN, LOW);
  digitalWrite(REDLED_PIN, LOW);
}

// about SDCard -> greenLED

void LED_showSDCardNokSignal(){
  digitalWrite(GREENLED_PIN, HIGH);
  digitalWrite(REDLED_PIN, LOW);

  delay(100);

  for (int i = 0; i<4; i++){
    ToggleLED(REDLED_PIN);
    delay(100);
  }

  digitalWrite(GREENLED_PIN, LOW);
  digitalWrite(REDLED_PIN, LOW);
}

void LED_showSDCardFileNameNOkSignal(){
  digitalWrite(GREENLED_PIN, HIGH);
  digitalWrite(YELLOWLED_PIN, LOW);

  delay(100);

  for (int i = 0; i<4; i++){
    ToggleLED(YELLOWLED_PIN);
    delay(100);
  }

  digitalWrite(GREENLED_PIN, LOW);
  digitalWrite(YELLOWLED_PIN, LOW);
}


// about RTC -> green + orange LED

void LED_showRTCFailedInitSignal(){
  digitalWrite(GREENLED_PIN, HIGH);
  digitalWrite(ORANGELED_PIN, HIGH);

  digitalWrite(REDLED_PIN, LOW);

  delay(100);

  for (int i = 0; i<4; i++){
    ToggleLED(REDLED_PIN);
    delay(100);
  }

  digitalWrite(GREENLED_PIN, LOW);
  digitalWrite(ORANGELED_PIN, LOW);
  digitalWrite(REDLED_PIN, LOW);
}

void LED_showRTCFailedHBSetSignal(){
  digitalWrite(GREENLED_PIN, HIGH);
  digitalWrite(ORANGELED_PIN, HIGH);

  digitalWrite(YELLOWLED_PIN, LOW);

  delay(100);

  for (int i = 0; i<4; i++){
    ToggleLED(YELLOWLED_PIN);
    delay(100);
  }

  digitalWrite(GREENLED_PIN, LOW);
  digitalWrite(ORANGELED_PIN, LOW);

  digitalWrite(YELLOWLED_PIN, LOW);
}


