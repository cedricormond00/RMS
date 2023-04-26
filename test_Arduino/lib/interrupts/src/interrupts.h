// LED.h
#ifndef INTERRUPTS_H // include guard
#define INTERRUPTS_H
// #pragma once

// Function prototype
void Interrupts_init(void);
void Interrupts_init_LL (void); // directly modifying the registers to intialise the interrupt
void ISR_buttonPress (void);
void __attribute__((interrupt)) MyEIC_Handler(void);
void RegisterView(void);


//void EIC_Handler(void);


#endif
