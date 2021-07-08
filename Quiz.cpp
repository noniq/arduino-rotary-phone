#include "Arduino.h"
#include "Quiz.h"

Quiz::Quiz(RotaryPhoneDialDecoder *_dialDecoder, Adafruit_Soundboard *_sfx, void (*_playTrack)(char *)) {
  dialDecoder = _dialDecoder;
  sfx = _sfx;
  playTrack = _playTrack;
}

void Quiz::start() {
  randomSeed(analogRead(0));
  playTrack("Q-START OGG");
  delay(200);

  uint8_t correctAnswers = 0, questionId;
  char filename[] = "Q-FRAGE?OGG";
  for (uint8_t i = 1; i < 6; i++) {
    filename[7] = i + '0';
    playTrack(filename);
    delay(1000);

    questionId = chooseRandomQuestion();
    chosenQuestionIds[i  - 1] = questionId;
    if (doQuestion(questionId)) {
      correctAnswers++;
    }
    delay(1500);
  }

  char filename2[] = "Q-?VON5 OGG";
  filename2[2] = correctAnswers + '0';
  playTrack(filename2);
  delay(1500);
}

uint8_t Quiz::chooseRandomQuestion() {
  uint8_t category, question, questionId;
  do {
    category = random(1, 6);
    question = random(1, 10);
    questionId = category * 100 + question;
  } while (questionHasAlreadyBeenChosen(questionId));
  return questionId;
}

bool Quiz::questionHasAlreadyBeenChosen(uint8_t questionId) {
  for (uint8_t i = 0; i < 5; i++) {
    if (chosenQuestionIds[i] == questionId) return true;
  }
  return false;
}

bool Quiz::doQuestion(uint8_t questionId) {
  uint8_t category = questionId / 100, question = questionId % 100;

  char filename1[] = "Q?-?--0?OGG";
  char filename2[] = "Q-ANTW? OGG";
  filename1[1] = category + '0'; // Category number
  filename1[3] = question < 10 ? question + '0' : 'A'; // Question number
  filename1[7] = '1';
  playTrack(filename1); // Play question
  delay(750);
  uint8_t *answerSequence = ANSWER_SEQUENCES[random(0, 5)];
  for (int i = 0; i < 3; i++) {
    filename2[6] = i + '1';
    playTrack(filename2); // Play answer number
    delay(350);
    filename1[7] = answerSequence[i] + '1';
    playTrack(filename1); // Play answer
    delay(1000);
  }

  int8_t digit = readDigit1to3();
  filename1[7] = answerSequence[digit - 1] + '1';
  playTrack(filename1); // Play chosen answer
  delay(1000);

  bool isCorrect = answerSequence[digit - 1] == 3;
  if (isCorrect) {
    playTrack("Q-RICHT OGG");
  } else {
    playTrack("Q-FALSCHOGG");
  }
  delay(500);
  filename1[7] = '5';
  playTrack(filename1); // Play explanation
  return isCorrect;
}

int8_t Quiz::readDigit1to3() {
  int8_t digit = -1;
  while(digit < 1 || digit > 3) {
    while (!dialDecoder->isDialling()) { delay(10); }
    digit = dialDecoder->readDigit();
  }
  return digit;
}
