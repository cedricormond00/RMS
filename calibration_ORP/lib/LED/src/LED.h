// LED.h
#ifndef LED_H // include guard
#define LED_H

/** \brief Visual feedback that the LED initialisation has been performed
 * This Signal also tests each LED
*/
void LED_runInitBeginSignal();






void multipin_pinMode(unsigned int pin_array[], unsigned int arraySize, unsigned int mode);
//void multipin_pinMode_s(unsigned int (&pin_array)[], unsigned int mode);

void multipin_HL(unsigned int pin_array[], unsigned int arraySize, unsigned int status);

void ToggleLED(unsigned int pin);




/** \brief function to show the LED initialisation has completed*/
void LED_runInitCompleteSignal();



void LED_showBatteryLowSignal();
void LED_showBatteryNotConnectedSignal();
void LED_showSDCardNokSignal();
void LED_showSDCardFileNameNOkSignal();
void LED_showRTCFailedInitSignal();
void LED_showRTCFailedHBSetSignal();
void LED_showUnknownErrorSignal();
void LED_showSMSnbnok();




#endif