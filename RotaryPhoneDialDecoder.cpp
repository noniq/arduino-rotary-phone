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

bool RotaryPhoneDialDecoder::isDialling() {
  return digitalRead(aPin) == LOW; // aPin is low as soon as dialling has started (and during the complete dial process)
}

uint8_t RotaryPhoneDialDecoder::readDigit() {
  uint8_t digit = 0, lastB = LOW, curB = LOW;
  while (digitalRead(aPin) == LOW) {
    curB = digitalRead(bPin);
    if (lastB == LOW && curB == HIGH) digit++; // count how often bPin toggles its state
    lastB = curB;
    delay(10);
  }
  return digit >= 10 ? 0 : digit;
}
