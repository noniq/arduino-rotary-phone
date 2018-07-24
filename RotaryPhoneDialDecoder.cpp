#include <Arduino.h>
#include "RotaryPhoneDialDecoder.h"

RotaryPhoneDialDecoder::RotaryPhoneDialDecoder(uint8_t _diallingSignalPin, uint8_t _digitSignalPin) {
  diallingSignalPin = _diallingSignalPin;
  digitSignalPin = _digitSignalPin;
}

void RotaryPhoneDialDecoder::setup() {
  pinMode(diallingSignalPin, INPUT_PULLUP);
  pinMode(digitSignalPin, INPUT_PULLUP);
}

bool RotaryPhoneDialDecoder::isDialling() {
  return digitalRead(diallingSignalPin) == LOW; // diallingSignalPin is low as soon as dialling has started (and during the complete dial process)
}

// Return currenty dialled digit (0-9), or -1 if the digit was not recognized correctly.
int8_t RotaryPhoneDialDecoder::readDigit() {
  uint8_t digit = 0, last = LOW, current = LOW;
  while (digitalRead(diallingSignalPin) == LOW) {
    current = digitalRead(digitSignalPin);
    if (last == LOW && current == HIGH) digit++; // count how often digitSignalPin toggles its state
    last = current;
    delay(10);
  }
  if (digit < 1 || digit > 10) return -1;
  return digit == 10 ? 0 : digit;
}
