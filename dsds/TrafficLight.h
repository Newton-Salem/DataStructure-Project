
#pragma once


class TrafficLight {
private:
    int currentGreenLane;
    int switchingCost;
    int switchCount;
    int switchCooldown;

public:
    // Constructor
    TrafficLight();

    void setSwitchingCost(int cost);

    int getCurrentGreenLane() const;
    int getSwitchCount() const;

    bool canSwitch() const;
    void updateCooldown();
    void switchTo(int lane);
    void reset();
};
