// Timers.h
#ifndef TIMERS_H // include guard
#define TIMERS_H
// #pragma once

// Function prototype
/**< \brief intialises the timer/counter in 8bit mode */
void Timers_tc4_init8bit(uint16_t clk_div_, uint8_t count_);
void Timers_tc4_init8bit(uint32_t freq_);

uint16_t next_pow2(uint16_t v_);
uint16_t get_clk_div(uint32_t freq_);


#endif
