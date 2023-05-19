#include <Arduino.h>
#include <assert.h>

#include "Constant.h"
#include "Global.h"
#include "Tool.h"
#include "Timer.h"
#include "LED.h"



    // // Enable the OSC32K oscillatoR
    // SYSCTRL->OSC32K.bit.EN32K = 1;
    // // Configure the OSC32K
    // SYSCTRL->OSC32K.bit.EN32K = 0;         // Disable the oscillator
    // while (SYSCTRL->OSC32K.bit.EN32K);     // Wait for the oscillator to be disabled
    // SYSCTRL->OSC32K.bit.STARTUP = 0x4;     // Set the oscillator startup time (recommended value)
    // SYSCTRL->OSC32K.bit.ONDEMAND = 1;      // Enable on-demand mode

void Timer_genericClockGeneratorTC4TC5_init(uint8_t genClockGeneratorID_){
  // Set up the generic clock (GCLK-genClockID) used to clock timers
  // set the division on the generic clock generator 4
  // REG_GCLK_GENDIV = GCLK -> GENDIV.reg
  REG_GCLK_GENDIV = GCLK_GENDIV_DIV(1) |          // Divide the 32kHz clock source by divisor 1: 48MHz/1=48MHz
                    GCLK_GENDIV_ID(genClockGeneratorID_);   // Select Generic Clock (GCLK)
  // Serial.println("test1");
  while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization
  // Serial.println("test2");
  
  // Set the source for the generic clock generator 4
  REG_GCLK_GENCTRL = GCLK_GENCTRL_IDC |           // Set the duty cycle to 50/50 HIGH/LOW
                      GCLK_GENCTRL_GENEN |        // Enable relevant GCLK
                      GCLK_GENCTRL_SRC_XOSC32K|   // Set the 32kHz clock source
                      GCLK_GENCTRL_ID(genClockGeneratorID_);// Select GCLK-genClockID
  // Serial.println("test3");
  while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization
  // Serial.println("test4");
  
  // Feed GCLK4 to TC4 and TC5
  // Enable the clock for timer 4
  REG_GCLK_CLKCTRL = GCLK_CLKCTRL_CLKEN |         // Enable GCLK4 to TC4 and TC5
                      GCLK_CLKCTRL_GEN(genClockGeneratorID_) | //GCLK_CLKCTRL_GEN_GCLK5 |     // Select GCLK4
                      GCLK_CLKCTRL_ID_TC4_TC5;     // Feed the GCLK4 to TC4 and TC5
  // Serial.println("test5");
}

void Timer_tc4_init16bit(uint16_t counterPrescaler_, uint16_t counterCompare_){
  bool debug = 1;
  
  // Set up the generic clock generator to use with TC4
  uint8_t genClockGeneratorID = 4;
  Timer_genericClockGeneratorTC4TC5_init(genClockGeneratorID);
  while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization
  // Serial.println("test6");

  // Set up the timer
  // Select Timer 4 (TC4) as a 16-bit timer
  TC4->COUNT16.CTRLA.reg |= TC_CTRLA_MODE_COUNT16;
  // REG_TC4_CTRLA |= TC_CTRLA_MODE_COUNT16;          // Set the counter to 16-bit mode
  while (TC4->COUNT16.STATUS.bit.SYNCBUSY);        // Wait for synchronization
  if (debug) {Serial.println("test7");}

  // Set the compare value
  TC4->COUNT16.CC[0].reg = counterCompare_;
  // while (TC4->COUNT16.STATUS.bit.SYNCBUSY);        // Wait for synchronization
  if (debug){Serial.println("test8");}

  // COnfigure the NVIC
  NVIC_DisableIRQ(TC4_IRQn);        // Disable the TC4 Interrupt Request Line 
  NVIC_ClearPendingIRQ(TC4_IRQn);
  NVIC_SetPriority(TC4_IRQn, 0);    // Set the Nested Vector Interrupt Controller (NVIC) priority for TC4 to 0 (highest)
  NVIC_EnableIRQ(TC4_IRQn);         // Connect TC4 to Nested Vector Interrupt Controller (NVIC)
      

  if (debug){
    Serial.print("REG_TC4_INTFLAG bef clear: ");
    Serial.println(REG_TC4_INTFLAG, BIN);
    Serial.print("REG_TC4_INTENSET bef clear: ");
    Serial.println(REG_TC4_INTENSET, BIN);
    Serial.print("REG_TC4_INTENCLR bef clear: ");
    Serial.println(REG_TC4_INTENCLR, BIN);
  }
  
  REG_TC4_INTFLAG |= TC_INTFLAG_OVF;  // Clear the interrupt flags
  REG_TC4_INTENSET = TC_INTENSET_OVF; // Enable TC4 interrupts
  
  if (debug){
    Serial.println("REG_TC4_INTFLAG |= TC_INTFLAG_OVF;");
    Serial.print("REG_TC4_INTFLAG bef reset: ");
    Serial.println(REG_TC4_INTFLAG, BIN);
    Serial.print("REG_TC4_INTENSET bef reset: ");
    Serial.println(REG_TC4_INTENSET, BIN);
    Serial.print("REG_TC4_INTENCLR bef clear: ");
    Serial.println(REG_TC4_INTENCLR, BIN);
  }
  // based on the optimal counterPrescaler, find the correct corresponding byte
  uint16_t prescale=0;
  switch(counterPrescaler_)
  {
      case 1:    prescale=TC_CTRLA_PRESCALER(0); break;
      case 2:    prescale=TC_CTRLA_PRESCALER(1); break;
      case 4:    prescale=TC_CTRLA_PRESCALER(2); break;
      case 8:    prescale=TC_CTRLA_PRESCALER(3); break;
      case 16:   prescale=TC_CTRLA_PRESCALER(4); break;
      case 64:   prescale=TC_CTRLA_PRESCALER(5); break;
      case 256:  prescale=TC_CTRLA_PRESCALER(6); break;
      case 1024: prescale=TC_CTRLA_PRESCALER(7); break;
  }
  TC4->COUNT16.CTRLA.reg |= prescale |                  //set by how many ticks the clock generator must go before incrementing by one the counter
                              TC_CTRLA_WAVEGEN_MFRQ |   // Set that when the counter reaches the counterCompare, an overflow occurs
                              TC_CTRLA_ENABLE;          // Enable TC4
  while (TC4->COUNT16.STATUS.bit.SYNCBUSY);             // Wait for synchronization
  
  if (debug){
    Serial.print("REG_TC4_INTFLAG aft enable: ");
    Serial.println(REG_TC4_INTFLAG, BIN);
    Serial.print("REG_TC4_INTENSET aft enable: ");
    Serial.println(REG_TC4_INTENSET, BIN);
  }

  attachInterrupt(TC4_IRQn, TC4_Handler, RISING);       // (optional) ensure the TC4_Handler is used upon interrupt request 
}



//----

uint16_t Timer_16bit32kHz_getCounterPrescaler(float freq_)
// Valid for 16 bit and using XOSC32K as a clock source
{
  float idealCounterPrescaler=32768.0f/(65536.0f*freq_); //this gives the widest range possible (at cost of reosution)
  
  uint16_t counterPrescaler=Tool_16bitNextPow2(uint16_t(ceil(idealCounterPrescaler)));
  switch(counterPrescaler) // to account for those prescalers that do not exist
  {
    case 32: counterPrescaler=64; break;
    case 128: counterPrescaler=256; break;
    case 512: counterPrescaler=1024; break;
  }
  return counterPrescaler;
}

void Timer_tc4_init16bit(uint32_t value_, char valueType_)
// t_ is in miliseconds (1 s = 1000 ms = value_)
{
  // add: assert valueType_ is either f or t
  assert(valueType_ == 't' || valueType_ == 'f');
  
  bool debug = 1;
  float freq;
  
  if (valueType_ == 't' ){
    freq = Tool_periodToFrequency(value_); //convert to a frequency
    Serial.print(freq);
  }
  else {
    freq = float(value_);
  }
  
  // add: a sanity check: ensure freq < GCLK_T4/(2^16*1024)
  
  uint16_t counterPrescaler=Timer_16bit32kHz_getCounterPrescaler(freq);
  uint16_t counterCompare=(32768/counterPrescaler)/freq;
  
  if (debug) {
    Serial.print("freq");
    Serial.println(freq);
    Serial.print("counterPrescaler");
    Serial.println(counterPrescaler);
    Serial.print("counterCompare");
    Serial.println(counterCompare);
    Serial.println("we got here 1");
  }
  
  Timer_tc4_init16bit(counterPrescaler, counterCompare);
}



//---------------------------------------------------------------------------

void TC4_Handler()
{
  // check for the reason of the TC4 IRQ
  if (TC4->COUNT16.INTFLAG.bit.OVF && TC4->COUNT16.INTENSET.bit.OVF)  // overflow           
  {
    bool debug = false;
    
    /*write your interrupt code here*/
    // toggle flag (set to true)
    timerFlag = true; 

    // Best would be to increment here
    WMTC += timeIncrementS;
    
    ToggleLED(YELLOWLED_PIN);
    
    if (debug){
      Serial.println(timerFlag);
      Serial.println("in ISR");
      Serial.print("REG_TC4_INTFLAG bef flag reset: ");
      Serial.println(REG_TC4_INTFLAG, BIN);
    }

    REG_TC4_INTFLAG = TC_INTFLAG_OVF;

    if (debug){
      Serial.print("REG_TC4_INTFLAG aft flag reset: ");
      Serial.println(REG_TC4_INTFLAG, BIN);
    }
  }
}

  // Serial.println(NVIC_GetPendingIRQ(PM_IRQn));              
  // Serial.println(NVIC_GetPendingIRQ(SYSCTRL_IRQn));    
  // Serial.println(NVIC_GetPendingIRQ(WDT_IRQn));  
  // Serial.println(NVIC_GetPendingIRQ(RTC_IRQn));  
  // Serial.println(NVIC_GetPendingIRQ(EIC_IRQn));  
  // Serial.println(NVIC_GetPendingIRQ(NVMCTRL_IRQn));  
  // Serial.println(NVIC_GetPendingIRQ(DMAC_IRQn));  
  // Serial.println(NVIC_GetPendingIRQ(USB_IRQn));    
  // Serial.println(NVIC_GetPendingIRQ(EVSYS_IRQn));  
  // Serial.println(NVIC_GetPendingIRQ(SERCOM0_IRQn));  
  // Serial.println(NVIC_GetPendingIRQ(SERCOM1_IRQn));   
  // Serial.println(NVIC_GetPendingIRQ(SERCOM2_IRQn));   
  // Serial.println(NVIC_GetPendingIRQ(SERCOM3_IRQn));   
  // Serial.println(NVIC_GetPendingIRQ(SERCOM4_IRQn));  
  // Serial.println(NVIC_GetPendingIRQ(SERCOM5_IRQn));      
  // Serial.println(NVIC_GetPendingIRQ(TCC0_IRQn));              
  // Serial.println(NVIC_GetPendingIRQ(TCC1_IRQn));           
  // Serial.println(NVIC_GetPendingIRQ(TCC2_IRQn));  
  // Serial.println(NVIC_GetPendingIRQ(TC3_IRQn)); 
  // Serial.println(NVIC_GetPendingIRQ(TC4_IRQn));    
  // Serial.println(NVIC_GetPendingIRQ(TC5_IRQn));  
  // Serial.println(NVIC_GetPendingIRQ(TC6_IRQn));  
  // Serial.println(NVIC_GetPendingIRQ(TC7_IRQn));  
  // Serial.println(NVIC_GetPendingIRQ(ADC_IRQn));    
  // Serial.println(NVIC_GetPendingIRQ(AC_IRQn));   
  // Serial.println(NVIC_GetPendingIRQ(DAC_IRQn));   
  // Serial.println(NVIC_GetPendingIRQ(PTC_IRQn)); 
  // Serial.println(NVIC_GetPendingIRQ(I2S_IRQn));   