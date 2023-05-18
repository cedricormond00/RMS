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




#endif