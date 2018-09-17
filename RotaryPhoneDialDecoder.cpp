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

// Return currently dialled digit (0-9), or -1 if the digit was not recognized
// correctly (0 or more than 10 pulses detected). Dialling must already have
// started (`diallingSignalPin` active).
//
// `diallingSignalPin` is assumed to be active-low; for `digitSignalPin` it
// doesn’t matter (edge detection works with both active-low and active-high).
int8_t RotaryPhoneDialDecoder::readDigit() {
  uint8_t digit = 0, last = LOW, current = LOW;
  while (digitalRead(diallingSignalPin) == LOW) {
    current = digitalRead(digitSignalPin);

    // Count how often digitSignalPin toggles its state (edge detection).
    if (last == LOW && current == HIGH) digit++;

    last = current;
    delay(10);
  }
  if (digit < 1 || digit > 10) return -1;
  return digit == 10 ? 0 : digit;
}
