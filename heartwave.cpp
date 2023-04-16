#include "heartwave.h"

Heartwave::Heartwave() : clock(0), activePulseReading(false), /*currentSession(nullptr), */battery(new Battery()), on(false), breathPacer(new BreathPacer())
{
    QStringList summaryArray;

    Session* session1 = new Session(1);
    Session* session2 = new Session(2);
    Session* session3 = new Session(3);


    session1->hrArray = {60, 65, 75, 95, 120, 145, 175, 195, 175, 145, 120, 95, 75, 65, 60, 65, 75, 95, 120, 145, 175, 195, 175, 145, 120, 95, 75, 65, 60, 65, 75, 95, 120, 145, 175, 195, 175, 145, 120, 95, 75, 65, 60, 65, 75, 95, 120, 145, 175, 195, 175, 145, 120, 95, 75, 65, 64, 65, 62, 60};
    session1->coheranceLevelArray = {1.9, 0, 0, 0, 0, 2.9, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2.4, 0, 0, 0, 0, 4, 0, 0, 0, 0, 2.1, 0, 0, 0, 0, 6, 0, 0, 0, 0, 3.9, 0, 0, 0, 0, 8.3, 0, 0, 0, 0, 10, 0, 0, 0, 0, 12, 0, 0, 0, 0, 5.4, 0, 0, 0, 0};

    session2->hrArray = {60, 65, 75, 95, 120, 145, 175, 195, 175, 145, 120, 95, 75, 85, 60, 75, 73, 95, 85, 125, 175, 165, 175, 125, 120, 85, 65, 75, 30, 65, 75, 95, 170, 145, 175, 195, 135, 145, 120, 95, 75, 65, 60, 65, 75, 95, 120, 145, 175, 195, 175, 145, 120, 95, 75, 65, 64, 65, 62, 60};
    session2->coheranceLevelArray = {3.2, 0, 0, 0, 0, 2.7, 0, 0, 0, 0, 1.8, 0, 0, 0, 0, 0.9, 0, 0, 0, 0, 0.7, 0, 0, 0, 0, 1.8, 0, 0, 0, 0, 0.3, 0, 0, 0, 0, 0.6, 0, 0, 0, 0, 0.7, 0, 0, 0, 0, 1.4, 0, 0, 0, 0, 2.7, 0, 0, 0, 0, 3.6, 0, 0, 0, 0};

    session3->hrArray = {60, 80, 100, 90, 110, 150, 160, 170, 150, 70, 80, 100, 89, 65, 80, 99, 88, 78, 85, 89, 121, 144, 152, 170, 187, 200, 163, 122, 113, 112, 95, 96, 115, 120, 175, 195, 175, 145, 120, 94, 73, 64, 60, 73, 80, 97, 102, 145, 175, 195, 175, 145, 120, 95, 75, 65, 64, 65, 62, 60};
    session3->coheranceLevelArray = {0.8, 0, 0, 0, 0, 0.7, 0, 0, 0, 0, 0.2, 0, 0, 0, 0, 1.7, 0, 0, 0, 0, 0.9, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0.5, 0, 0, 0, 0, 0.2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1.8, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1.9, 0, 0, 0, 0};

    for(double i = 0; i < session1->hrArray.length() ; i++) {
      session1->seconds.append(i);
      session2->seconds.append(i);
      session3->seconds.append(i);
    }

    this->currentSession = session1;

    this->sessions.push_back(session1);
    this->sessions.push_back(session2);
    this->sessions.push_back(session3);
}

void Heartwave::update()
{
    if (currentSession != nullptr ) {
        if (activePulseReading) {
            currentSession->update();
        }
        if(currentSession->started && !currentSession->ended && !currentSession->paused && activePulseReading) {
             breathPacer->update();
        }
    }
    battery->update();
}

void Heartwave::setCurrentSession(int index)
{

    if (index >= 1 || index <= 3) {
        currentSession = this->sessions[index - 1];
        currentSession->started = true;
        currentSession->paused = false;
        currentSession->ended = true;

        for (int i = 0; i < (int) sessions.size(); i++) {
            sessions[i]->started = false;
            sessions[i]->paused = true;
            sessions[i]->ended = true;
        }

    } else {
        return;
    }
}

void Heartwave::resetData()
{
    currentSession = nullptr;
    sessions.clear();
}

void Heartwave::replaceBattery()
{
    this->battery->setBatteryLevel(100.0);
}

bool Heartwave::getActivePulseReading()
{
    return this->activePulseReading;
}

void Heartwave::setActivePulseReading(bool b)
{
    this->activePulseReading = b;
}


void Heartwave::turnOn() {
    this->on = true;
}
void Heartwave::turnOff() {
    this->on = false;
}


