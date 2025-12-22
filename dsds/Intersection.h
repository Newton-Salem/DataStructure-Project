#pragma once
#include "Vehicle.h"
#include "TrafficLight.h"
#include "PriorityQueue.h"
#include "Queue.h"

// ============================================================================
// INTERSECTION CLASS
// ============================================================================
class Intersection {
private:
    int id;
    PriorityQueue evQueue;    // Priority queue for EVs
    Queue ptQueue;            // Regular queue for PT
    Queue ncQueue;            // Regular queue for NC
    Queue fvQueue;            // Regular queue for FV

    TrafficLight* trafficLight;
    Vehicle* crossingVehicle;
    int crossingRemaining;
    bool blocked;
    bool laneBlocked[10];
    int numLanes;
    int timeActive;

public:
    // Constructor
    Intersection();

    // ================= Setters / Getters =================
    void setID(int i);
    int getID() const;
    void setNumLanes(int n);
    void setBlocked(bool b);
    bool isBlocked() const;
    void setLaneBlocked(int lane, bool b);
    int getTimeActive() const;
    TrafficLight* getTrafficLight();
    Vehicle* getCrossingVehicle() const;
    int getCrossingRemaining() const;
    void setCrossingVehicle(Vehicle* v);
    void setCrossingRemaining(int r);
    // ================= Core Logic =================
    void addVehicle(Vehicle* v);
    bool removeVehicle(int id);
    void incrementTimeActive();
    void updateWaitingTimes(int currentTime, int autoP, int cancelT);
    Vehicle* selectNextVehicle();
    void processCrossing();
    void assignGreenLane();
    bool canInterruptForEV();

    // ================= Queue Access =================
    PriorityQueue* getEVQueue();
    Queue* getPTQueue();
    Queue* getNCQueue();
    Queue* getFVQueue();

    bool hasVehicles() const;
};
