#include "heartwave.h"

Heartwave::Heartwave() : clock(0), activePulseReading(false), currentSession(nullptr), battery(new Battery()), breathPacer(new BreathPacer())
{
    //
}


void Heartwave::update()
{

    if (currentSession != nullptr ) {
        currentSession->update();
    }
    battery->update();
    breathPacer->update();
}

void Heartwave::setCurrentSession(int)
{

}

void Heartwave::resetData()
{
    currentSession = nullptr;
    sessions.clear();
}

void Heartwave::replaceBattery()
{

}


