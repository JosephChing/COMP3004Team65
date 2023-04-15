#include "session.h"

Session::Session(int id)
    : sessionID(id), clock(0),heartRate(0), coheranceRating(0), length(60), started(false),
      ended(false), paused(false){
    time = std::chrono::system_clock::now();
}

void Session::update() {
//    std::cout<<
    if (!paused && started && !ended) {
        this->heartRate = hrArray[clock];
        this->updateCoheranceRating();
        clock += 1;
        if(clock >= length) {
            ended = true;
            generateSummary();
        }
    }
}

void Session::start() {
    paused = false;
    started = true;
    ended = false;
    clock = 0;
}

void Session::updateCoheranceRating() {
    // Your implementation here
    if (coheranceLevelArray[clock] != 0) {
        this->coheranceRating = coheranceLevelArray[clock];
    }
}

void Session::calculateAchievementScore() {
    // Your implementation here
}
double Session::getCoheranceRating()
{
    return this->coheranceRating;
}



std::string Session::generateSummary() {
    std::stringstream summary;
    summary << "Session ID: " << sessionID << '\n'
            << "Session Length: " << length << '\n'
            << "Coherance Rating: " << coheranceRating << '\n';
    return summary.str();
}

int Session::getSessionLength() {
    return clock;
}

void Session::interruptSession() {
    paused = true;
}

void Session::resumeSession() {
    paused = false;
}
