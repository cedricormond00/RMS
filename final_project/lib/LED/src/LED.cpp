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

//functions to provide some visual feedback about the errors enountered during setup

// about battery -> orangeLED
// (Yellow)
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

// (Red)
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

// about SDCard -> green LED
// (Red)
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
// (Yellow)
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



// about SMS -> yellow
//(red)
void LED_showSMSnbnok(){
  digitalWrite(YELLOWLED_PIN, HIGH);
  digitalWrite(REDLED_PIN, LOW);

  delay(100);

  for (int i = 0; i<4; i++){
    ToggleLED(REDLED_PIN);
    delay(100);
  }

  digitalWrite(YELLOWLED_PIN, LOW);
  digitalWrite(REDLED_PIN, LOW);
}


// about RTC -> green + orange LED
// (red)
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

// (yellow)
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

// about unknown_Error -> red + (blue) LED
void LED_showUnknownErrorSignal(){
  digitalWrite(REDLED_PIN, HIGH);
  digitalWrite(BLUELED_PIN, LOW);

  delay(100);

  for (int i = 0; i<4; i++){
    ToggleLED(BLUELED_PIN);
    delay(100);
  }

  digitalWrite(REG_DAC_DATA, LOW);
  digitalWrite(BLUELED_PIN, HIGH);
}





// void multipin_HL(unsigned int pin_arr[], unsigned int arraySize, unsigned int status)
// {

//   for (unsigned int i = 0; i < arraySize; i++) 
//   {
//       digitalWrite(pin_arr[i], status);
//       Serial.print("Toggle pin ");
//       Serial.println(pin_arr[i]);
//   }
// }

// void multipin_pinMode(unsigned int pin_array[], unsigned int arraySize, unsigned int mode)
// {
//   for (unsigned int currPin = 0; currPin < arraySize; currPin++) 
//   {
//     pinMode(currPin, mode);
//   }
// }
