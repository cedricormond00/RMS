// Timer.h
#ifndef TIMER_H // include guard
#define TIMER_H
// #pragma once

// Function prototype

/** \brief intialises the TC4 timer/counter in 16bit mode 
 * \param counterPrescaler_ = counter prescaler (PRESCALER)
 * \param counterCompare_ = channel compare value (CC0)
*/
void Timer_tc4_init16bit(uint16_t counterPrescaler_, uint16_t counterCompare_);



/** \brief intialises the TC4 timer/counter in 16bit mode
 * \param value_ = desired interrupt value (Hz or ms)
 * \param valueType_ = t (period (ms)) or f (frequency (Hz)) 
*/
void Timer_tc4_init16bit(uint32_t value_, char valueType_);

/** \brief util function to obtain the counterPrescaler 
 * \remark works only for the 16 bit mode
 * \param freq_ = interrupt frequency
*/
uint16_t Timer_16bit32kHz_getCounterPrescaler(float freq_);

/** \brief intialises the genericClockGenerator for TC4 and TC5 
 * \param genClockGeneratorID_ = generic clock generator ID (4-8 works best)
*/
void Timer_genericClockGeneratorTC4TC5_init(uint8_t genClockGeneratorID_);




#endif
