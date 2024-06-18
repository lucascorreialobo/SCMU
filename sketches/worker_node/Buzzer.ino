#include "pitches.h"


int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

int currentNote = 0;

void playTune() {
  
  Serial.println("Playing tune");

  for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(BUZZZER_PIN, melody[thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(BUZZZER_PIN);
  }

  Serial.println("Tune ended");
}

void dealBuzzer(unsigned long startTime, unsigned long currentTime){
  Serial.println("Time before tune = " + String(millis()));

  

  Serial.println("Time after tune = " + String(millis()));

  delay(1000);
}

//void generateNoteDurationsFromStartTime(unsigned long startTime){
//  unsigned long totalTime = 0;
//  unsigned long[8] noteEndTimes;
//  for (int thisNote = 0; thisNote < 8; thisNote++) {
//    int noteDuration = 1000 / noteDurations[thisNote] * 1.30;
//    totalTime += noteDuration;
//    noteEndTime[thisNote] = totalTime;
//  }
//}