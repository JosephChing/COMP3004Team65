#ifndef SESSION_H
#define SESSION_H

#include <chrono>
#include <ctime>
#include <string>
#include <QVector>
#include <iostream>
#include <sstream>

class Session {
public:
    // Constants
    static const int N = 10;

    // Properties
    int sessionID;
    std::chrono::system_clock::time_point time;
    int clock;
    QVector<double> hrArray; // length 60 * N
    QVector<double> coheranceLevelArray; // length 60 * N

    QVector<double> seconds;

    double heartRate;
    double coheranceRating;
    double length;
    bool started;
    bool ended;
    bool paused;

    // Constructor
    Session(int i );

    // Methods
    void update();
    void start();
    void stop();

    void updateCoheranceRating();

    void calculateAchievementScore();

    double getCoheranceRating();

    std::string generateSummary();

    int getSessionLength();

    void interruptSession();

    void resumeSession();
};

#endif // SESSION_H
