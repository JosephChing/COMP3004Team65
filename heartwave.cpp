#include "heartwave.h"

Heartwave::Heartwave() : clock(0), activePulseReading(false), /*currentSession(nullptr), */battery(new Battery()), breathPacer(new BreathPacer())
{


    Session* session1 = new Session(1);
    Session* session2 = new Session(2);
    Session* session3 = new Session(3);


    session1->hrArray = {60, 63, 67, 70, 72, 74, 75, 74, 72, 70, 67, 63, 60, 57, 53, 50, 48, 46, 45, 46, 48, 50, 53, 57, 60, 63, 67, 70, 72, 74, 75, 74, 72, 70, 67, 63, 60, 57, 53, 50, 60, 63, 67, 70, 72, 74, 75, 74, 72, 70, 67, 63, 60, 57, 53, 50, 48, 46, 55, 60};
    session1->coheranceLevelArray = {1, 0, 0, 0, 0, 3, 0, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 0, 1, 0, 0, 0, 0, 3, 0, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 0, 1, 0, 0, 0, 0, 3, 0, 0, 0, 0};

    session2->hrArray = {60, 64, 70, 80, 90, 100, 110, 120, 130, 140, 150, 140, 130, 120, 110, 100, 90, 80, 70, 64, 60, 64, 70, 80, 90, 100, 110, 120, 130, 140, 150, 140, 130, 120, 110, 100, 90, 80, 70, 64, 60, 64, 70, 80, 90, 100, 110, 120, 130, 140, 150, 140, 130, 120, 110, 100, 90, 80, 70, 60};
    session2->coheranceLevelArray = {1.5, 0, 0, 0, 0, 1.2, 0, 0, 0, 0, 1.9, 0, 0, 0, 0, 1.3, 0, 0, 0, 0, 1.6, 0, 0, 0, 0, 1.8, 0, 0, 0, 0, 1.1, 0, 0, 0, 0, 1.7, 0, 0, 0, 0, 1.4, 0, 0, 0, 0, 1.9, 0, 0, 0, 0, 1.5, 0, 0, 0, 0, 1.2, 0, 0, 0, 0};

    session3->hrArray = {60, 65, 75, 95, 120, 145, 175, 195, 175, 145, 120, 95, 75, 65, 60, 65, 75, 95, 120, 145, 175, 195, 175, 145, 120, 95, 75, 65, 60, 65, 75, 95, 120, 145, 175, 195, 175, 145, 120, 95, 75, 65, 60, 65, 75, 95, 120, 145, 175, 195, 175, 145, 120, 95, 75, 65, 64, 65, 62, 60};
    session3->coheranceLevelArray = {0.5, 0, 0, 0, 0, 0.3, 0, 0, 0, 0, 0.8, 0, 0, 0, 0, 0.1, 0, 0, 0, 0, 0.6, 0, 0, 0, 0, 0.9, 0, 0, 0, 0, 0.4, 0, 0, 0, 0, 0.2, 0, 0, 0, 0, 0.7, 0, 0, 0, 0, 0.3, 0, 0, 0, 0, 0.5, 0, 0, 0, 0, 0.8, 0, 0, 0, 0};

    for(double i = 0; i < session1->hrArray.length() ; i++){
      session1->seconds.append(i);
      session2->seconds.append(i);
      session3->seconds.append(i);
    }

    this->currentSession = session1;





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



std::string Heartwave::currentLight()
{
    if(this->currentSession->getCoheranceRating()<=1){
        return "red";
    }

    if(this->currentSession->getCoheranceRating()<=2){
        return "yellow";
    }

    if(this->currentSession->getCoheranceRating()<=3){
        return "green";
    }
    return "red";
}


