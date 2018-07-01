#include <avr/sleep.h>

#include "Adafruit_Soundboard.h"
#include "RotaryPhoneDialDecoder.h"

static const uint8_t ROTARY_PHONE_A_PIN = A0;
static const uint8_t ROTARY_PHONE_B_PIN = A1;
static const uint8_t SFX_ACT = 7;

RotaryPhoneDialDecoder rotaryPhoneDialDecoder(ROTARY_PHONE_A_PIN, ROTARY_PHONE_B_PIN);
Adafruit_Soundboard sfx = Adafruit_Soundboard(&Serial, NULL, 9); // Last argument is reset pin, which we don't use

void setup() {
  rotaryPhoneDialDecoder.setup();
  pinMode(SFX_ACT, INPUT);
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
  delay(500);
  while(digitalRead(SFX_ACT) == LOW) {
    delay(10);
  }
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_mode();
  while(1) { // never reached (if sleep mode works correctly)
    digitalWrite(13, LOW);
    delay(200);
    digitalWrite(13, HIGH);
    delay(200);
  }
}
