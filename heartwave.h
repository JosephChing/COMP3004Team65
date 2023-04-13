#ifndef HEARTWAVE_H
#define HEARTWAVE_H

#include <vector>
#include "session.h"

#include "battery.h"
#include "breathpacer.h"
class Heartwave
{
public:
    Heartwave();
    std::vector<Session> sessions;
    Session * currentSession;

    Battery * battery;
    BreathPacer * breathPacer;

    // methods

    void update();
    void setCurrentSession(int);
    void interruptSession(int);
    void resetData();
    void replaceBattery();

private:
    int clock;
    bool activePulseReading;

};

#endif // HEARTWAVE_H
