#include "session.h"

Session::Session(int id)
    : sessionID(id), clock(0),heartRate(0), coheranceRating(0), length(60), achievementScore(0), started(false),
      ended(false), paused(false){
    time = std::chrono::system_clock::now();
}

void Session::update() {
    std::cout<< "Clock= " << this->clock << std::endl;
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
    achievementScore = 0;
}

void Session::stop() {
    ended = true;
}

void Session::updateCoheranceRating() {
    if (coheranceLevelArray[clock] != 0) {
        this->coheranceRating = coheranceLevelArray[clock];
        calculateAchievementScore();
    }
}

void Session::calculateAchievementScore() {
    achievementScore += coheranceRating;
}
double Session::getCoheranceRating()
{
    return this->coheranceRating;
}



std::string Session::generateSummary() {
    std::stringstream summary;
    summary << "Session ID: " << sessionID << '\n'
            << "Session Length: " << length << '\n'
            << "Achievement Score: " << achievementScore << '\n';
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
