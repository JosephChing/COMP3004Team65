#include "battery.h"
#include <iostream>

Battery::Battery() : batteryLevel(100), isLow(false) {}

void Battery::update() {
    batteryLevel -= 0.2;
    if (batteryLevel <= 20) {
        this->isLow = true;
    } else {
        this->isLow = false;
    }
}

double Battery::getBatteryLevel() const {
    return batteryLevel;
}

void Battery::setBatteryLevel(double level) {
    if (level >= 0.0 && level <= 100.0) {
        batteryLevel = level;
    } else {
        std::cerr << "Error: Invalid battery level.  Must be in range 0-100." << std::endl;
    }
}
