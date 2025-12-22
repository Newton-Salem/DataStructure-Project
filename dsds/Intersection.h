#pragma once
#include "Vehicle.h"
#include "TrafficLight.h"
#include "PriorityQueue.h"
#include "Queue.h"

class Intersection {
private:
    int id;
    PriorityQueue evQueue;    
    Queue ptQueue;            
    Queue ncQueue;           
    Queue fvQueue;            

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

    //setters
    void setID(int i);
    void setNumLanes(int n);
    void setBlocked(bool b);
    void setLaneBlocked(int lane, bool b);
    void setCrossingVehicle(Vehicle* v);
    void setCrossingRemaining(int r);

    //getters
    int getID() const;
    int getTimeActive() const;
    TrafficLight* getTrafficLight();
    Vehicle* getCrossingVehicle() const;
    int getCrossingRemaining() const;
    PriorityQueue* getEVQueue();
    Queue* getPTQueue();
    Queue* getNCQueue();
    Queue* getFVQueue();

    //logic
    void addVehicle(Vehicle* v);
    bool isBlocked() const;
    bool removeVehicle(int id);
    void incrementTimeActive();
    void updateWaitingTimes(int currentTime, int autoP, int cancelT);
    Vehicle* selectNextVehicle();
    void processCrossing();
    void assignGreenLane();
    bool canInterruptForEV();
    bool hasVehicles() const;
};
