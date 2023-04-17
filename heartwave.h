#ifndef HEARTWAVE_H
#define HEARTWAVE_H

#include <QVector>
#include <vector>
#include "session.h"

#include "battery.h"
#include "breathpacer.h"
#include <string>
class Heartwave
{
public:
    Heartwave();
    std::vector<Session*> sessions;
    Session * currentSession;
    QString summaryArray;
    Battery * battery;
    BreathPacer * breathPacer;

    bool on;
    // methods

    void update();
    void setCurrentSession(int);
    void interruptSession(int);
    void resetData();
    void replaceBattery();
    bool getActivePulseReading();
    void setActivePulseReading(bool b);

    void turnOn();
    void turnOff();


private:
    int clock;
    bool activePulseReading;

};

#endif // HEARTWAVE_H
