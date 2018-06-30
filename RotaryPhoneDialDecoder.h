#ifndef ROTARYPHONEDIALDECODER_H
#define ROTARYPHONEDIALDECODER_H

#include <Arduino.h>

class RotaryPhoneDialDecoder {
  public:
    RotaryPhoneDialDecoder(uint8_t _aPin, uint8_t _bPin);
    void setup();
    uint8_t waitForDigit();
  private:
    uint8_t aPin, bPin;
};

#endif
