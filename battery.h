#ifndef BATTERY_H
#define BATTERY_H

class Battery {
public:
    Battery();

    void update();

    void lowBatteryWarning() const;

    double getBatteryLevel() const;

    void setBatteryLevel(double level);

private:
    double batteryLevel;
};

#endif // BATTERY_H
