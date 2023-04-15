#ifndef BATTERY_H
#define BATTERY_H

class Battery {
public:
    Battery();

    bool isLow;
    void update();
    double getBatteryLevel() const;

    void setBatteryLevel(double level);

private:
    double batteryLevel;
};

#endif // BATTERY_H
