#include <avr/sleep.h>

#include "RotaryPhoneDialDecoder.h"

static const uint8_t ROTARY_PHONE_BASE_PIN = 2;
static const uint8_t ROTARY_PHONE_A_PIN = 3;
static const uint8_t ROTARY_PHONE_B_PIN = 4;

RotaryPhoneDialDecoder rotaryPhoneDialDecoder(ROTARY_PHONE_BASE_PIN, ROTARY_PHONE_A_PIN, ROTARY_PHONE_B_PIN);

void setup() {
  Serial.begin(57600);
  rotaryPhoneDialDecoder.setup();
}

void loop() {
  uint8_t digit = rotaryPhoneDialDecoder.waitForDigit();
  Serial.println(digit);
}
