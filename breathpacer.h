#include <iostream>

class BreathPacer {
public:
  BreathPacer();
  int clock;
  int currentPosition;
  int frequency;

  void update();
  void changeFrequency(int newFrequency);
};
