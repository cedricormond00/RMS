#include <Arduino.h>

#include "Constant.h"
#include "Global.h"
#include "Tool.h"
#include "Timer.h"
#include "LED.h"

void Timer_tc4_init16bit(uint16_t counterPrescaler_, uint16_t counterCompare_){
    bool debug = 0;
    // Set up the generic clock (GCLK4) used to clock timers
    // set the division on the generic clock generator 4
    // REG_GCLK_GENDIV = GCLK -> GENDIV.reg
    // Since these are general settings, we would prefer to not modify them too much
    // should probably put the GCLK setting in a different header?
    REG_GCLK_GENDIV = GCLK_GENDIV_DIV(1) |          // Divide the 48MHz clock source by divisor 1: 48MHz/1=48MHz
                    GCLK_GENDIV_ID(4);            // Select Generic Clock (GCLK) 4
  
    while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization

    // Set the source for the generic clock generator 4
    REG_GCLK_GENCTRL = GCLK_GENCTRL_IDC |           // Set the duty cycle to 50/50 HIGH/LOW
                        GCLK_GENCTRL_GENEN |         // Enable GCLK4
                        GCLK_GENCTRL_SRC_DFLL48M |   // Set the 48MHz clock source
                        GCLK_GENCTRL_ID(4);          // Select GCLK4
    while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization

    // Feed GCLK4 to TC4 and TC5
    REG_GCLK_CLKCTRL = GCLK_CLKCTRL_CLKEN |         // Enable GCLK4 to TC4 and TC5
                        GCLK_CLKCTRL_GEN_GCLK4 |     // Select GCLK4
                        GCLK_CLKCTRL_ID_TC4_TC5;     // Feed the GCLK4 to TC4 and TC5

    while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization

    //Set the timer
    
    REG_TC4_CTRLA |= TC_CTRLA_MODE_COUNT16;          // Set the counter to 8-bit mode
    while (TC4->COUNT16.STATUS.bit.SYNCBUSY);        // Wait for synchronization

    REG_TC4_COUNT16_CC0 = counterCompare_;                      // Set the TC4 CC0 register to some arbitary value
    while (TC4->COUNT16.STATUS.bit.SYNCBUSY);        // Wait for synchronization

    NVIC_DisableIRQ(TC4_IRQn);
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
    

    REG_TC4_INTFLAG |= TC_INTFLAG_OVF;              // Clear the interrupt flags
    REG_TC4_INTENSET = TC_INTENSET_OVF;             // Enable TC4 interrupts
    
    
    if (debug){
      Serial.println("REG_TC4_INTFLAG |= TC_INTFLAG_OVF;");
      Serial.print("REG_TC4_INTFLAG bef reset: ");
      Serial.println(REG_TC4_INTFLAG, BIN);
      Serial.print("REG_TC4_INTENSET bef reset: ");
      Serial.println(REG_TC4_INTENSET, BIN);
      Serial.print("REG_TC4_INTENCLR bef clear: ");
      Serial.println(REG_TC4_INTENCLR, BIN);
    }
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
    REG_TC4_CTRLA |= prescale | TC_CTRLA_WAVEGEN_MFRQ | TC_CTRLA_ENABLE;    // Enable TC4
    while (TC4->COUNT16.STATUS.bit.SYNCBUSY);        // Wait for synchronization
    if (debug){
      Serial.print("REG_TC4_INTFLAG aft enable: ");
      Serial.println(REG_TC4_INTFLAG, BIN);
      Serial.print("REG_TC4_INTENSET aft enable: ");
      Serial.println(REG_TC4_INTENSET, BIN);
    }
    Serial.println("we got here 2");
    attachInterrupt(TC4_IRQn, TC4_Handler, RISING);
    Serial.println("we got here 3");

    // Register the ISR for TC4 with the NVIC

}


//----

uint16_t Timer_getCounterPrescaler(float freq_)
// Valid for 16 bit
{
  float idealCounterPrescaler=48000000.0f/(65536.0f*freq_); //this gives the widest range possible (at cost of reosution)
  
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
  bool debug = true;
  float freq;
  // add: assert valueType_ is either f or t
  if (valueType_ == 't' ){
    freq = Tool_periodToFrequency(value_); //convert to a frequency
    Serial.print(freq);
  }
  else {
    freq = float(value_);
  }
  
  // add: a sanity check: ensure freq < GCLK_T4/(2^16*1024)
  
  uint16_t counterPrescaler=Timer_getCounterPrescaler(freq);
  uint16_t counterCompare=(48000000/counterPrescaler)/freq;
  
  if (debug) {
    Serial.print("freq");
    Serial.println(freq);
    Serial.print("counterPrescaler");
    Serial.println(counterPrescaler);
    Serial.print("counterCompare");
    Serial.println(counterCompare);
  }
  Serial.println("we got here 1");
  Timer_tc4_init16bit(counterPrescaler, counterCompare);
}

// uint16_t Timer_getCounterPrescaler(uint32_t freq_)
//  // SUitable for 8 bit configuration
// {
//   float idealCounterPrescaler=48000000.0f/(256.0f*float(freq_));
//   uint16_t counterPrescaler=Tool_16bitNextPow2(uint16_t(ceil(idealCounterPrescaler)));
//   switch(counterPrescaler)
//   {
//     case 32: counterPrescaler=64; break;
//     case 128: counterPrescaler=256; break;
//     case 512: counterPrescaler=1024; break;
//   }
  
//   return counterPrescaler;
// }
//----

// void Timer_tc4_init16bit(uint32_t freq_)
// {
//   uint16_t counterPrescaler=Timer_getCounterPrescaler(freq_);
//   uint16_t counterCompare=(48000000/counterPrescaler)/freq_;
//   Timer_tc4_init16bit(counterPrescaler, counterCompare);
// }




// void Timer_tc4_init16bit_t(uint32_t t_)
// // t_ is in miliseconds (1 s = 1000 ms = t_)
// {
//   bool debug = true;
//   float freq = 1/(float(t_)*float(0.001)); //convert to a frequency
//   // add a sanity check: ensure freq < GCLK_T4/(2^16*1024)
  
//   uint16_t counterPrescaler=Timers_getClkDiv_t(freq);
//   uint16_t counterCompare=(48000000/counterPrescaler)/freq;
  
//   if (debug) {
//     Serial.print("freq");
//     Serial.println(freq);
//     Serial.print("counterPrescaler");
//     Serial.println(counterPrescaler);
//     Serial.print("counterCompare");
//     Serial.println(counterCompare);
//   }

//   Timer_tc4_init16bit(counterPrescaler, counterCompare);
// }




//---------------------------------------------------------------------------

// void TC4_Handler()
// {
//   // Serial.println(NVIC_GetPendingIRQ(PM_IRQn));              
//   // Serial.println(NVIC_GetPendingIRQ(SYSCTRL_IRQn));    
//   // Serial.println(NVIC_GetPendingIRQ(WDT_IRQn));  
//   // Serial.println(NVIC_GetPendingIRQ(RTC_IRQn));  
//   // Serial.println(NVIC_GetPendingIRQ(EIC_IRQn));  
//   // Serial.println(NVIC_GetPendingIRQ(NVMCTRL_IRQn));  
//   // Serial.println(NVIC_GetPendingIRQ(DMAC_IRQn));  
//   // Serial.println(NVIC_GetPendingIRQ(USB_IRQn));    
//   // Serial.println(NVIC_GetPendingIRQ(EVSYS_IRQn));  
//   // Serial.println(NVIC_GetPendingIRQ(SERCOM0_IRQn));  
//   // Serial.println(NVIC_GetPendingIRQ(SERCOM1_IRQn));   
//   // Serial.println(NVIC_GetPendingIRQ(SERCOM2_IRQn));   
//   // Serial.println(NVIC_GetPendingIRQ(SERCOM3_IRQn));   
//   // Serial.println(NVIC_GetPendingIRQ(SERCOM4_IRQn));  
//   // Serial.println(NVIC_GetPendingIRQ(SERCOM5_IRQn));      
//   // Serial.println(NVIC_GetPendingIRQ(TCC0_IRQn));              
//   // Serial.println(NVIC_GetPendingIRQ(TCC1_IRQn));           
//   // Serial.println(NVIC_GetPendingIRQ(TCC2_IRQn));  
//   // Serial.println(NVIC_GetPendingIRQ(TC3_IRQn)); 
//   // Serial.println(NVIC_GetPendingIRQ(TC4_IRQn));    
//   // Serial.println(NVIC_GetPendingIRQ(TC5_IRQn));  
//   // Serial.println(NVIC_GetPendingIRQ(TC6_IRQn));  
//   // Serial.println(NVIC_GetPendingIRQ(TC7_IRQn));  
//   // Serial.println(NVIC_GetPendingIRQ(ADC_IRQn));    
//   // Serial.println(NVIC_GetPendingIRQ(AC_IRQn));   
//   // Serial.println(NVIC_GetPendingIRQ(DAC_IRQn));   
//   // Serial.println(NVIC_GetPendingIRQ(PTC_IRQn)); 
//   // Serial.println(NVIC_GetPendingIRQ(I2S_IRQn));      
           

//   // NVIC_ClearPendingIRQ(TC4_IRQn);
//   // Serial.print("NVIC_GetPendingIRQ(TC4_IRQn)");
//   // Serial.println(NVIC_GetPendingIRQ(TC4_IRQn));

//   if (TC4->COUNT16.INTFLAG.bit.OVF && TC4->COUNT16.INTENSET.bit.OVF)             
//   {
//     /*write your interrupt code here*/
//     timerFlag = !timerFlag; // toggle flag (set to true)
//     // Serial.println(timerFlag);
//     bool debug = false;
//     if (debug){
//       Serial.print("REG_TC4_INTFLAG bef flag reset: ");
//       Serial.println(REG_TC4_INTFLAG, BIN);
//     }

//     REG_TC4_INTFLAG = TC_INTFLAG_OVF;

//     if (debug){
//       Serial.print("REG_TC4_INTFLAG aft flag reset: ");
//       Serial.println(REG_TC4_INTFLAG, BIN);
//     }

//     // acknowledge the interrupt request
//     // TC4->COUNT16.CTRLA.reg = TC_CTRLA_ENABLE;
    
//     //NVIC_ClearPendingIRQ(TC4_IRQn);

//     // if (debug){
//     //   Serial.print("REG_TC4_INTENCLR bef interupt reset: ");
//     //   Serial.println(REG_TC4_INTENCLR, BIN);
//     // }
//     // REG_TC4_INTENCLR = TC_INTENCLR_OVF;
//     // if (debug){
//     //   Serial.print("REG_TC4_INTENCLR aft interupt reset: ");
//     //   Serial.println(REG_TC4_INTENCLR, BIN);
//     // }


//     // //clear all flags
//     // REG_TC4_INTFLAG = TC_INTFLAG_MC0;
//     // REG_TC4_INTFLAG = TC_INTFLAG_MC1;

//     // if (debug){
//     //   Serial.print("REG_TC4_INTFLAG aft all clear: ");
//     //   Serial.println(REG_TC4_INTFLAG, BIN);
//     // }
//   }
// }