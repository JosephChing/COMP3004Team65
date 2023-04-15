#include "breathpacer.h"

BreathPacer::BreathPacer() {
  clock = 0;
  currentPosition = 0;
  frequency = 10;
}

void BreathPacer::update() {
    if(clock <= frequency) {
        clock++;
    } else {
        clock = 0;
    }
    currentPosition = (clock / frequency) * 100;
//    std::cout<< "currentPosition "<< currentPosition << std::endl;
}

void BreathPacer::changeFrequency(int newFrequency) {
  frequency = newFrequency;
}

