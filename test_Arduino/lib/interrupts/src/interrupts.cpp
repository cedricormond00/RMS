#include <Arduino.h>
#include <sam.h>

#include "interrupts.h"
#include "constant.h"
#include "LED.h"

#define wait_t 50

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

void __attribute__((interrupt)) MyEIC_Handler() {
    // Code to execute when EIC interrupt is triggered
    if(EIC->INTFLAG.bit.EXTINT11){
        // clear the interrupt flag
        EIC->INTFLAG.reg = EIC_INTFLAG_EXTINT11;
        bp = true;
    }
    
    // Serial.println();
    // Serial.println("in ISR");
    // Serial.print("bp:");
    // Serial.println(bp);
    // Serial.println();
}

void RegisterView(void){
    Serial.print("GCLK->GENCTRL.reg: ");
    Serial.println(GCLK->GENCTRL.reg, BIN);
    delay(wait_t);
    // Configure the generic clock, in one single 16 bit wrote
    Serial.print("GCLK->CLKCTRL.reg: ");
    Serial.println(GCLK->CLKCTRL.reg, BIN);
    delay(wait_t);
    Serial.print("GCLK->GENDIV.reg: ");
    Serial.println(GCLK->GENDIV.reg, BIN);
    delay(wait_t);


    // EIC REGISTERS
    Serial.print("EIC->CTRL.reg: ");
    Serial.println(EIC->CTRL.reg, BIN);
    delay(wait_t);

    Serial.print("EIC->STATUS.reg: ");
    Serial.println(EIC->STATUS.reg, BIN);
    delay(wait_t);
    // / Write to the EIC configuration registers
    // Enable event associated with the EXTINTx pin to generate interrupts
    Serial.print("EIC->EVCTRL.reg: ");
    Serial.println(EIC->EVCTRL.reg, BIN);
    delay(wait_t);

    Serial.print("EIC->INTENCLR.reg: ");
    Serial.println(EIC->INTENCLR.reg, BIN);
    delay(wait_t);

    // Enable EXTERNAL interrupt 
    Serial.print("EIC->INTENSET.reg: ");
    Serial.println(EIC->INTENSET.reg, BIN);
    delay(wait_t);

    Serial.print("EIC->INTFLAG.reg: ");
    Serial.println(EIC->INTFLAG.reg, BIN);
    delay(wait_t);

    Serial.print("EIC->WAKEUP.reg: ");
    Serial.println(EIC->WAKEUP.reg, BIN);
    delay(wait_t);

    // Configure pin PB11 to trigger on rising edge
    Serial.print("EIC->CONFIG[0].reg: ");
    Serial.println(EIC->CONFIG[0].reg, BIN);
    delay(wait_t);

    Serial.print("EIC->CONFIG[1].reg: ");
    Serial.println(EIC->CONFIG[1].reg, BIN);
    delay(wait_t);

    
    Serial.println(NVIC->ICER.reg, BIN);
    Serial.println(NVIC->ICPR, BIN);
    Serial.println(NVIC->IP, BIN);
    Serial.println(NVIC->ISER, BIN);
    Serial.println(NVIC->ISPR, BIN);

    
}



void Interrupts_init_LL (void) {
    /*
    enable even associated with EXTINT11 to generate an even 
    */
    ///Disable AIC while we configure it
    EIC->CTRL.bit.ENABLE = 0; 
    Serial.println("here");

    /// Enable the corresponding IO pins
    // done through the pinMode etc
    /// Configure and enable a generic clock, to clock the peripheral
    // No need to configure any specific divsion factors: leave defaults
    // GCLK -> GENDIV = 0x0;
    // Enable the generic clovk generator performing a single 32 bit write
    // GCLK->GENCTRL.reg = 
    //     GCLK_GENCTRL_GENEN |
    //     GCLK_GENCTRL_SRC_XOSC |
    //     GCLK_GENCTRL_ID(0);
    // Serial.print("GCLK->GENCTRL.reg: ");
    // Serial.println(GCLK->GENCTRL.reg, BIN);
    // delay(500);
    // Configure the generic clock, in one single 16 bit wrote
    GCLK->CLKCTRL.reg = 
        // GCLK_CLKCTRL_CLKEN | // enable the clock (in this case default clock: GCLKGEN0)
        GCLK_CLKCTRL_GEN_GCLK5; // | // define the Generic Clock Generator which will be associated with the peripheral GCLK clock defined in the CLKCTRL.ID.: Use the default (clock0)
        // GCLK_CLKCTRL_ID(GCLK_CLKCTRL_ID_EIC); // Select the peripheral clock associated to the generic clock generator defined in CLKCTRL.GEN 
    // Serial.print("GCLK->CLKCTRL.reg: ");
    // Serial.println(GCLK->CLKCTRL.reg, BIN);
    // delay(500);
    // / Wait for synchronization
    Serial.println("GCLK end");
    while (GCLK->STATUS.bit.SYNCBUSY);
    Serial.println("EIC begin");


    // / Write to the EIC configuration registers
    // Enable event associated with the EXTINTx pin to generate interrupts
    // EIC->EVCTRL.bit.EXTINTEO11=1;
    // Serial.print("EIC->EVCTRL.reg: ");
    // Serial.println(EIC->EVCTRL.reg, BIN);
    // delay(500);
    //EIC->EVCTRL.reg |= EIC_EVCTRL_EXTINTEO5; //Equivalent to top
    // Enable EXTERNAL interrupt 
    EIC->INTENCLR.reg=EIC_INTENCLR_EXTINT11;
    EIC->INTENSET.reg=EIC_INTENSET_EXTINT11;

    // Serial.print("EIC->INTENSET.reg: ");
    // Serial.println(EIC->INTENSET.reg, BIN);
    // delay(500);
    EIC->INTFLAG.reg = 0;
    EIC->WAKEUP.reg = EIC_WAKEUP_WAKEUPEN11;
    // // = equivalent to
    // EIC->INTENSET.EXTINT11 = 1;
    // Configure pin PB11 to trigger on rising edge
    EIC->CONFIG[1].bit.SENSE3 = 0x01; 
    // Serial.print("EIC->CONFIG.reg: ");
    // Serial.println(EIC->CONFIG[1].reg, BIN);
    // delay(500);

    // Finally enable the EIC
    EIC->CTRL.bit.ENABLE = 1; 
    // Serial.print("EIC->CTRL.reg: ");
    // Serial.println(EIC->CTRL.reg, BIN);
    // delay(500);
    Serial.println("EIC end");
    // EIC->ASYNCH.reg = EIC_ASYNCH_ASYNCH(0x0); // configure asynchronous mode

    RegisterView();
    // / Wait for synchronization
    // while (EIC->STATUS.bit.SYNCBUSY);
    Serial.println("NVIC begin");
    // Enable EIC interrupt on NVIC
    NVIC_EnableIRQ(EIC_IRQn); // Enable EIC interrupt
    NVIC_SetPriority(EIC_IRQn, 0); // Set EIC interrupt priority to 0  (highest)
    // Set the EIC interrupt handler function
    //*reinterpret_cast<void (**)(void)>(EIC_ADDR + EIC_INTENSET_OFFSET) = &MyEIC_Handler;

    
}