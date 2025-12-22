
#pragma once

// ============================================================================
// TRAFFIC LIGHT CLASS
// ============================================================================
class TrafficLight {
private:
    int currentGreenLane;
    int switchingCost;
    int switchCount;
    int switchCooldown;

public:
    // Constructor
    TrafficLight();

    // ================= Setters =================
    void setSwitchingCost(int cost);

    // ================= Getters =================
    int getCurrentGreenLane() const;
    int getSwitchCount() const;

    // ================= Logic =================
    bool canSwitch() const;
    void updateCooldown();
    void switchTo(int lane);
    void reset();
};
