#include <avr/sleep.h>

#include "Adafruit_Soundboard.h"
#include "RotaryPhoneDialDecoder.h"
#include "Quiz.h"

static const uint8_t DEBUG_LED_PIN = 13;
static const uint8_t ROTARY_PHONE_A_PIN = 9;
static const uint8_t ROTARY_PHONE_B_PIN = 10;
static const uint8_t SFX_PWR = 2;
static const uint8_t SFX_ACT = 8;

uint8_t playAudioTrackBlocking(char *filename, bool);

RotaryPhoneDialDecoder rotaryPhoneDialDecoder(ROTARY_PHONE_A_PIN, ROTARY_PHONE_B_PIN);
Adafruit_Soundboard sfx = Adafruit_Soundboard(&Serial, NULL, 9); // Last argument is reset pin, which we don't use
Quiz quiz = Quiz(&rotaryPhoneDialDecoder, &sfx, playAudioTrackBlocking);

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
  delay(10);
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

void powerDown() {
  digitalWrite(SFX_PWR, HIGH);
  pinMode(SFX_PWR, INPUT);
  pinMode(DEBUG_LED_PIN, INPUT);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_mode();
  while(1) { // never reached (if sleep mode works correctly)
    debugBlink(200);
  }
}

bool isAudioPlaying() {
  return digitalRead(SFX_ACT) == LOW;
}

void loop() {
  flushInput();
  while (sfx.volUp() < 200);
  sfx.playTrack("F       OGG");
  delay(200); // give soundboard some time to actually start playing the file

  while (isAudioPlaying()) {
    if (rotaryPhoneDialDecoder.isDialling()) {
      sfx.stop();
      uint8_t digit = rotaryPhoneDialDecoder.readDigit();
      if (digit == -1) break;
      if (digit == 1) {
        quiz.start();
      } else {
        sfx.playTrack("KEIN-AN OGG");
        delay(200);
      }
      break;
    }
    delay(10);
  }

  while (isAudioPlaying()) delay(10);

  sfx.playTrack("B       OGG");
  delay(200); // give soundboard some time to actually start playing the file
  while (isAudioPlaying()) delay(10);

  powerDown();
}

uint8_t playAudioTrackBlocking(char *filename, bool interruptible) {
  sfx.playTrack(filename);
  delay(200);
  while (isAudioPlaying()) {
    if (interruptible && rotaryPhoneDialDecoder.isDialling()) {
      sfx.stop();
      int8_t digit = rotaryPhoneDialDecoder.readDigit();
      delay(200);
      return digit;
    }
    delay(10);
  }
  return -1;
}
