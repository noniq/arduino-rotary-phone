#ifndef ROTARYPHONEDIALDECODER_H
#define ROTARYPHONEDIALDECODER_H

#include <Arduino.h>

class RotaryPhoneDialDecoder {
  public:
    RotaryPhoneDialDecoder(uint8_t _diallingSignalPin, uint8_t _digitSignalPin);
    void setup();
    bool isDialling();
    int8_t readDigit();
  private:
    uint8_t diallingSignalPin, digitSignalPin;
};

#endif
