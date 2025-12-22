#ifndef TRAFFIC_CONTROL_CENTER_H
#define TRAFFIC_CONTROL_CENTER_H

#include <iostream>
#include <fstream>
#include <cstring>
#include "UI.h"
#include "Vehicle.h"
#include "Event.h"
#include "Intersection.h"
#include "AdjacencyList.h"

class TrafficControlCenter {
private:
    Intersection* intersections[50];
    int numIntersections;
    AdjacencyList adjacencyMap[50];

    Event* eventHead;
    Vehicle* completedVehicles;
    Vehicle* allVehicles[10000];
    int vehicleCount;

    int currentTime;
    int switchingCost;
    int autoP;
    int cancelT;
    bool reroutingEnabled;

    UI* ui;

    //helper functions
    void rerouteVehiclesFromIntersection(int intIdx);
    void handleArrival(Event* e);
    void handleEVInterrupt(int intIdx);
    void handleCancellation(Event* e);
    void handlePromotion(Event* e);
    void handleAccident(Event* e);
    void handleRoadClosure(Event* e);
    void insertEvent(Event* e);
    void updateAllWaitingTimes();
    void assignGreenLanes();
    void processAllCrossing();
    bool hasActiveVehicles();

public:
    TrafficControlCenter(UI* userInterface);

    bool loadFromFile(const char* filename);
    void checkAutoPromotions();
    void checkAutoCancellations();
    void processEvents();
    void simulate(int mode);
    void sortVehiclesByCT();
    void writeOutput(const char* filename);
};

#endif
