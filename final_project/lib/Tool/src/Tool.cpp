#include <Arduino.h>

#include "Tool.h"


uint16_t Tool_16bitNextPow2(uint16_t v_)
{
  // the next power-of-2 of the value (if v_ is pow-of-2 returns v_)
  --v_;
  v_|=v_>>1;
  v_|=v_>>2;
  v_|=v_>>4;
  v_|=v_>>8;
  return v_+1;
}
float Tool_periodToFrequency(uint32_t t_){
    float freq = 1/(float(t_)*float(0.001));
    return freq;
}

void Tool_setBitOff(uint8_t* byte, uint8_t mask){
  *byte &= ~(mask);
}

// void Tool_addToRingBuffer(float ringBuffer[], int bufferSize, float value) {
//     // Shift existing values to the right
//     for (int i = bufferSize - 1; i > 0; --i) {
//         ringBuffer[i] = ringBuffer[i - 1];
//     }
//     ringBuffer[0] = value;  // Add the new value at position 0
// }



// RingBuffer::RingBuffer(int size) {
//     bufferSize = size;
//     buffer = new float[bufferSize];
//     currentIndex = 0;
// }

// RingBuffer::~RingBuffer() {
//     delete[] buffer;
// }

// void RingBuffer::addToBuffer(float value) {
//     // Shift existing values to the right
//     for (int i = bufferSize - 1; i > 0; --i) {
//         buffer[i] = buffer[i - 1];
//     }
//     buffer[0] = value;  // Add the new value at position 0
// }

// float RingBuffer::getOldestValue() {
//     return buffer[bufferSize - 1];
// }

// float RingBuffer::getNewestValue() {
//     return buffer[0];
// }
