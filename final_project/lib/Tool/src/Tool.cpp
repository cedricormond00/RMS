#include <Arduino.h>
#include <TimeLib.h>

#include "Tool.h"

void Tools_print2digits(int number) {
  if (number < 10) {
    Serial.print("0");
  }
  Serial.print(number);
}

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

void Tool_setBitOff(volatile uint8_t* byte, uint8_t mask){
  *byte &= ~(mask);
}


void Tool_setBitOn(uint8_t* byte, uint8_t mask){
  *byte |= (mask);
}

void Tool_setBitOn(volatile uint8_t* byte, uint8_t mask){
  *byte |= (mask);
}


bool Tool_isBitOn(uint8_t byte, uint8_t mask) {
    return (byte & mask) != 0;
}

/* convert the time stamp into readable form */
void Tool_stringTime(uint32_t ePochTime, char* buf){
  // // Convert the epoch time to time_t
  // time_t time = ePochTime;

  // uint32_t time = ePochTime;

  // Convert the timestamp to a tm structure
  tmElements_t timeInfo;
  breakTime(ePochTime, timeInfo);

  // Format the time components into the provided buffer
  // sprintf(buf, "%2d%02d%02d", timeInfo.Year + 1970, timeInfo.Month, timeInfo.Day);//, timeInfo.Hour, timeInfo.Minute, timeInfo.Second);

  sprintf(buf, "%4d/%02d/%02d/-%02d:%02d:%02d", timeInfo.Year + 1970, timeInfo.Month, timeInfo.Day, timeInfo.Hour, timeInfo.Minute, timeInfo.Second);
}

// bool Tool_isBitOn(volatile uint8_t byte, uint8_t mask) {
//     return (byte & mask) != 0;
// }
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
