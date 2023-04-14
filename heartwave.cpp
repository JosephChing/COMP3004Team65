#include "heartwave.h"

Heartwave::Heartwave() : clock(0), activePulseReading(false), /*currentSession(nullptr), */battery(new Battery()), breathPacer(new BreathPacer())
{


    this->currentSession = new Session(1);

    for(double i = 0; i < 60 ; i++){
//        this->currentSession->hrArray.append(i);
        this->currentSession->seconds.append(i);

    }

    this->currentSession->hrArray = {60, 63, 67, 70, 72, 74, 75, 74, 72, 70, 67, 63, 60, 57, 53, 50, 48, 46, 45, 46, 48, 50, 53, 57, 60, 63, 67, 70, 72, 74, 75, 74, 72, 70, 67, 63, 60, 57, 53, 50, 60, 63, 67, 70, 72, 74, 75, 74, 72, 70, 67, 63, 60, 57, 53, 50, 48, 46, 45, 46, 48, 50, 53, 57, 60, 63, 67, 70, 72, 74, 75, 74, 72, 70, 67, 63, 60, 57, 53, 50, 48, 46, 45, 46, 48, 50, 53, 57};
//    this->currentSession->hrArray.append(1);
//    this->currentSession->hrArray.append(2);
//    this->currentSession->hrArray.append(3);
//    this->currentSession->hrArray.append(4);
//    this->currentSession->hrArray.append(5);
//    this->currentSession->hrArray.append(4);
//    this->currentSession->hrArray.append(3);
//    this->currentSession->hrArray.append(2);
//    this->currentSession->hrArray.append(1);
//    this->currentSession->hrArray.push_back(2);
//    this->currentSession->hrArray.push_back(3);
//    this->currentSession->hrArray.push_back(4);
//    this->currentSession->hrArray.push_back(5);
//    this->currentSession->hrArray.push_back(6);
//    this->currentSession->hrArray.push_back(5);
//    this->currentSession->hrArray.push_back(4);
//    this->currentSession->hrArray.push_back(3);
//    this->currentSession->hrArray.push_back(2);
//    this->currentSession->hrArray.push_back(1);

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

bool Heartwave::getActivePulseReading()
{
    return this->activePulseReading;
}

void Heartwave::setActivePulseReading(bool b)
{
    this->activePulseReading = b;
}


