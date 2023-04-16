#include "session.h"

Session::Session(int id)
    : sessionID(id), clock(0),heartRate(0), coheranceRating(0), length(60), achievementScore(0), started(false),
      ended(false), paused(false){
    time = std::chrono::system_clock::now();
    this->summary = "";
}

void Session::update() {
    //std::cout<< "Clock= " << this->clock << std::endl;

    if (!paused && started && !ended) {
        this->heartRate = hrArray[clock];
        this->updateCoheranceRating();
        clock += 1;
        if(clock >= length) {
            ended = true;
            generateSummary();
            coheranceRating = 0;
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



QString Session::generateSummary() {
    QString summary;
    summary.append("Session ID: ");
    summary.append(QString::number(sessionID));
    summary.append("\nSession Length: ");
    summary.append(QString::number(clock));
    summary.append("\nAchievement Score: ");
    summary.append(QString::number(achievementScore));
    summary.append("\n");
    return summary;

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
