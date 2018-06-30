#include <avr/sleep.h>
#include <SoftwareSerial.h>

#include "Adafruit_Soundboard.h"
#include "RotaryPhoneDialDecoder.h"

static const uint8_t ROTARY_PHONE_A_PIN = A0;
static const uint8_t ROTARY_PHONE_B_PIN = A1;
static const uint8_t SFX_TX = 5;
static const uint8_t SFX_RX = 6;

RotaryPhoneDialDecoder rotaryPhoneDialDecoder(ROTARY_PHONE_A_PIN, ROTARY_PHONE_B_PIN);
SoftwareSerial sfxSerial = SoftwareSerial(SFX_TX, SFX_RX);
Adafruit_Soundboard sfx = Adafruit_Soundboard(&sfxSerial, NULL, SFX_TX); // Last argument is reset pin, which we don't use

void setup() {
  rotaryPhoneDialDecoder.setup();
  pinMode(13, OUTPUT);
  sfxSerial.begin(9600 * (16000000L / F_CPU)); // Because we run with 8 MHz,
  delay(250);
}

void flushInput() {
  // Read all available serial input to flush pending data.
  uint16_t timeoutloop = 0;
  while (timeoutloop++ < 40) {
    while(sfxSerial.available()) {
      sfxSerial.read();
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
