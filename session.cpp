#include "session.h"

Session::Session(int id)
    : sessionID(id), clock(0), coheranceRating(0), length(60), started(false),
      ended(false), paused(false){
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
