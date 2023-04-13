#ifndef SESSION_H
#define SESSION_H

#include <chrono>
#include <ctime>
#include <string>
#include <vector>
#include <sstream>

class Session {
public:
    // Constants
    static const int N = 10;

    // Properties
    std::string sessionID;
    std::chrono::system_clock::time_point time;
    int clock;
    std::vector<double> hrArray; // length 60 * N
    std::vector<double> coheranceLevelArray; // length 60 * N

    std::vector<double> seconds;

    double coheranceRating;
    double length;
    bool started;
    bool ended;
    bool paused;

    // Constructor
    Session(std::string id);

    // Methods
    void update();

    void updateCoheranceRating();

    void calculateAchievementScore();

    void calculateCoheranceLevel();

    std::string generateSummary();

    int getSessionLength();

    void interruptSession();

    void resumeSession();
};

#endif // SESSION_H
