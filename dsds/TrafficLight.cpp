#include "TrafficLight.h"

// ============================================================================
// Constructor
// ============================================================================
TrafficLight::TrafficLight() {
    currentGreenLane = -1;
    switchingCost = 0;
    switchCount = 0;
    switchCooldown = 0;
}

// ============================================================================
// Setters
// ============================================================================
void TrafficLight::setSwitchingCost(int cost) {
    switchingCost = cost;
}

// ============================================================================
// Getters
// ============================================================================
int TrafficLight::getCurrentGreenLane() const {
    return currentGreenLane;
}

int TrafficLight::getSwitchCount() const {
    return switchCount;
}

// ============================================================================
// Logic
// ============================================================================
bool TrafficLight::canSwitch() const {
    return switchCooldown <= 0;
}

void TrafficLight::updateCooldown() {
    if (switchCooldown > 0) switchCooldown--;
}
void TrafficLight::switchTo(int lane) {
    if (currentGreenLane != -1 && currentGreenLane != lane) {
        switchCount++;
        switchCooldown = switchingCost; // Apply delay
    }
    currentGreenLane = lane;
}

void TrafficLight::reset() {
    currentGreenLane = -1;
}
