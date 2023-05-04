#include <Arduino.h>


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

