#include <iostream>

class BreathPacer {
public:
  BreathPacer();
  double clock;
  double currentPosition;
  double frequency;

  void update();
  void changeFrequency(int newFrequency);
};
