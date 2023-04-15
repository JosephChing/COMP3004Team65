#ifndef SESSION_H
#define SESSION_H

#include <chrono>
#include <ctime>
#include <string>
#include <QVector>
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

    double coheranceRating;
    double length;
    bool started;
    bool ended;
    bool paused;
    std::string summary;



    // Constructor
    Session(int i );

    // Methods
    void update();

    void updateCoheranceRating();

    void calculateAchievementScore();

    void calculateCoheranceLevel();

    void generateSummary();

    int getSessionLength();

    void interruptSession();

    void resumeSession();


};

#endif // SESSION_H
