#include "session.h"

Session::Session(std::string id)
    : sessionID(id), clock(0), coheranceRating(0), length(0), started(false),
      ended(false), paused(false),
      hrArray(60 * N, 0), coheranceLevelArray(60 * N, 0), seconds(60* N, 0) {
    time = std::chrono::system_clock::now();
}

void Session::update() {
    if (!paused && started && !ended) {
        clock += 1;
    }
}

void Session::updateCoheranceRating() {
    // Your implementation here
}

void Session::calculateAchievementScore() {
    // Your implementation here
}

void Session::calculateCoheranceLevel() {
    // Your implementation here
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