// Timers.h
#ifndef TIMERS_H // include guard
#define TIMERS_H
// #pragma once

// Function prototype
/**< \brief intialises the timer/counter in 8bit mode */
void Timers_tc4_init16bit(uint16_t clk_div_, uint16_t count_);
void Timers_tc4_init16bit(uint32_t freq_);

void Timers_tc4_init16bit_t(uint32_t t_);

uint16_t next_pow2(uint16_t v_);
uint16_t get_clk_div(uint32_t freq_);
uint16_t Timers_getClkDiv_t(float freq_);


#endif
