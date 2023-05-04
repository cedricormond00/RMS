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
 * \param freq_ = interrupt frequency
*/
uint16_t Timer_getCounterPrescaler(float freq_);



// /** \brief intialises the TC4 timer/counter in 16bit mode 
//  * \param freq_ = desired interrupt frequency
// */
// void Timer_tc4_init16bit(uint32_t freq_);

// /** \brief intialises the TC4 timer/counter in 16bit mode
//  * \param t_ = desired interrupt period. t_ is in ms (1s = 1000ms)
// */
// void Timer_tc4_init16bit_t(uint32_t t_);

// /** \brief util function to obtain the counterPrescaler 
//  * \param freq_ = interrupt frequency
// */
// uint16_t Timer_getCounterPrescaler(uint32_t freq_);



uint16_t get_clk_div(uint32_t freq_);



#endif
