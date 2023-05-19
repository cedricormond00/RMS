// Tool.h
#ifndef TOOL_H // include guard
#define TOOL_H


// #pragma once

// Function prototype
/** \brief obtains the next power of two, for a 16 bit integer
 * \param v_ = input 16 bit integer
*/
uint16_t Tool_16bitNextPow2(uint16_t v_);

/** \brief converts a period to a frequency
 * \param t_ = input 32 bit integer
*/
float Tool_periodToFrequency(uint32_t t_);

/** \brief Sets the bits off where 1 are in the mask
 * \param byte = input 8 bit word. To be had bits turned off
 * \param mask = mask which contains the bits to turn off
*/
void Tool_setBitOff(uint8_t* byte, uint8_t mask);



#endif