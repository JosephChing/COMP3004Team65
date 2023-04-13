#include "battery.h"
#include <iostream>

Battery::Battery() : batteryLevel(100) {}

void Battery::update() {
    batteryLevel -= 0.2;
}

void Battery::lowBatteryWarning() const {
    if (batteryLevel <= 20) {
        std::cout << "Warning: Low battery. Please change your battery." << std::endl;
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
