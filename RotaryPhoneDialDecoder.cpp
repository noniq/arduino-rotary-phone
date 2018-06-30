#include <Arduino.h>
#include "RotaryPhoneDialDecoder.h"

RotaryPhoneDialDecoder::RotaryPhoneDialDecoder(uint8_t _aPin, uint8_t _bPin) {
  aPin = _aPin;
  bPin = _bPin;
}

void RotaryPhoneDialDecoder::setup() {
  pinMode(aPin, INPUT_PULLUP);
  pinMode(bPin, INPUT_PULLUP);
}

uint8_t RotaryPhoneDialDecoder::waitForDigit() {
  while (digitalRead(aPin) == 1) delay(100); // Wait until dialing starts (aPin is low during the complete dial process)
  uint8_t digit = 0, lastB = 0, curB = 0;
  while (digitalRead(aPin) == 0) {
    curB = digitalRead(bPin);
    if (lastB == 0 && curB == 1) digit++; // Count how often bPin toggles its state
    lastB = curB;
    delay(10);
  }
  return digit >= 10 ? 0 : digit;
}
