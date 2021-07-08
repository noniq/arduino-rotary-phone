#ifndef QUIZ_H
#define QUIZ_H

#include "Adafruit_Soundboard.h"
#include "RotaryPhoneDialDecoder.h"

class Quiz {
  public:
    Quiz(RotaryPhoneDialDecoder *_dialDecoder, Adafruit_Soundboard *_sfx, void (*_playTrack)(char *));
    void start();

  private:
    RotaryPhoneDialDecoder *dialDecoder;
    Adafruit_Soundboard *sfx;
    uint8_t ANSWER_SEQUENCES[6][3] = {
      1, 2, 3,
      1, 3, 2,
      2, 1, 3,
      2, 3, 1,
      3, 1, 2,
      3, 2, 1
    };

    void (*playTrack)(char *);
    bool randomQuestion();
    int8_t readDigit1to3();
};

#endif
