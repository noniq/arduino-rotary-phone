#include <avr/sleep.h>

#include "Adafruit_Soundboard.h"
#include "RotaryPhoneDialDecoder.h"

#define DEBUG

static const uint8_t DEBUG_LED_PIN = 13;
static const uint8_t ROTARY_PHONE_A_PIN = A0;
static const uint8_t ROTARY_PHONE_B_PIN = A1;
static const uint8_t SFX_PWR = 2;
static const uint8_t SFX_ACT = 7;

RotaryPhoneDialDecoder rotaryPhoneDialDecoder(ROTARY_PHONE_A_PIN, ROTARY_PHONE_B_PIN);
Adafruit_Soundboard sfx = Adafruit_Soundboard(&Serial, NULL, 9); // Last argument is reset pin, which we don't use

void debugBlink(uint16_t millis) {
#ifdef DEBUG
  digitalWrite(DEBUG_LED_PIN, LOW);
  delay(millis);
  digitalWrite(DEBUG_LED_PIN, HIGH);
  delay(millis);
#endif
}

void setup() {
  digitalWrite(SFX_PWR, HIGH); // Make sure Sound Board is powered off initially (pin is active low)
  pinMode(SFX_PWR, OUTPUT);
  pinMode(SFX_ACT, INPUT);
#ifdef DEBUG
  pinMode(DEBUG_LED_PIN, OUTPUT);
  digitalWrite(DEBUG_LED_PIN, HIGH);
#endif
  Serial.begin(9600);
  delay(100);
  digitalWrite(SFX_PWR, LOW);
  rotaryPhoneDialDecoder.setup();
  sfx.readLine(); // Wait for Sound Board to boot up (or time out after 500ms ...)
  debugBlink(50);
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
  digitalWrite(SFX_PWR, HIGH);
  pinMode(SFX_PWR, INPUT);
  pinMode(DEBUG_LED_PIN, INPUT);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_mode();
  while(1) { // never reached (if sleep mode works correctly)
    debugBlink(200);
  }
}
