#include <avr/sleep.h>

#include "Adafruit_Soundboard.h"
#include "RotaryPhoneDialDecoder.h"

static const uint8_t ROTARY_PHONE_A_PIN = A0;
static const uint8_t ROTARY_PHONE_B_PIN = A1;

RotaryPhoneDialDecoder rotaryPhoneDialDecoder(ROTARY_PHONE_A_PIN, ROTARY_PHONE_B_PIN);
Adafruit_Soundboard sfx = Adafruit_Soundboard(&Serial, NULL, 9); // Last argument is reset pin, which we don't use

void setup() {
  rotaryPhoneDialDecoder.setup();
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  delay(250);
}

void flushInput() {
  // Read all available serial input to flush pending data.
  uint16_t timeoutloop = 0;
  while (timeoutloop++ < 40) {
    while(Serial.available()) {
      Serial.read();
      timeoutloop = 0;  // If char was received reset the timer
    }
    delay(1);
  }
}

void loop() {
  char filename[] = "F       OGG";
  flushInput();
  sfx.playTrack(filename);
  uint8_t digit = rotaryPhoneDialDecoder.waitForDigit();
  sfx.stop();
  filename[0] = digit + '0';
  sfx.playTrack(filename);
  while(1);
}
