#include "heartwave.h"

Heartwave::Heartwave() : clock(0), activePulseReading(false), currentSession(nullptr), battery(new Battery()), breathPacer(new BreathPacer())
{
<<<<<<< Updated upstream
    //
=======


    this->currentSession = new Session(1);

    for(double i = 0; i < 60 ; i++){
//        this->currentSession->hrArray.append(i);
        this->currentSession->seconds.append(i);

    }

    this->currentSession->hrArray = {60, 63, 67, 70, 72, 74, 75, 74, 72, 70, 67, 63, 60, 57, 53, 50, 48, 46, 45, 46, 48, 50, 53, 57, 60, 63, 67, 70, 72, 74, 75, 74, 72, 70, 67, 63, 60, 57, 53, 50, 60, 63, 67, 70, 72, 74, 75, 74, 72, 70, 67, 63, 60, 57, 53, 50, 48, 46, 45, 46, 48, 50, 53, 57, 60, 63, 67, 70, 72, 74, 75, 74, 72, 70, 67, 63, 60, 57, 53, 50, 48, 46, 45, 46, 48, 50, 53, 57};

>>>>>>> Stashed changes
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


