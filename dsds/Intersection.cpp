#include "Intersection.h"
#include <cstring>

// Constructor
Intersection::Intersection() {
    id = 0;
    trafficLight = new TrafficLight();
    crossingVehicle = nullptr;
    crossingRemaining = 0;
    blocked = false;
    numLanes = 4;
    timeActive = 0;

    for (int i = 0; i < 10; i++) {
        laneBlocked[i] = false;
    }
}
//getters
int Intersection::getID() const { return id; }
int Intersection::getTimeActive() const { return timeActive; }
TrafficLight* Intersection::getTrafficLight() { return trafficLight; }
Vehicle* Intersection::getCrossingVehicle() const { return crossingVehicle; }
int Intersection::getCrossingRemaining() const { return crossingRemaining; }

//setters
void Intersection::setID(int i) { id = i; }
void Intersection::setCrossingVehicle(Vehicle* v) { crossingVehicle = v; }
void Intersection::setCrossingRemaining(int r) { crossingRemaining = r; }
void Intersection::setNumLanes(int n) { numLanes = n; }
void Intersection::setBlocked(bool b) { blocked = b; }
void Intersection::setLaneBlocked(int lane, bool b) { laneBlocked[lane] = b; }

bool Intersection::isBlocked() const { return blocked; }

// Vehicle Handling
void Intersection::addVehicle(Vehicle* v) {
    const char* type = v->getTYPE();

    if (strcmp(type, "EV") == 0)
        evQueue.enqueue(v);
    else if (strcmp(type, "PT") == 0)
        ptQueue.enqueue(v);
    else if (strcmp(type, "NC") == 0)
        ncQueue.enqueue(v);
    else if (strcmp(type, "FV") == 0)
        fvQueue.enqueue(v);
}

bool Intersection::removeVehicle(int id) {
    if (evQueue.remove(id)) return true;
    if (ptQueue.remove(id)) return true;
    if (ncQueue.remove(id)) return true;
    if (fvQueue.remove(id)) return true;
    return false;
}

void Intersection:: incrementTimeActive() {
    if (crossingVehicle != NULL) {
        timeActive++;
    }
}

void Intersection::updateWaitingTimes(int currentTime, int autoP, int cancelT) {
    // Update EV queue
    Vehicle* curr = evQueue.peek();
    while (curr != NULL) {
        curr->setWT(currentTime - curr->getAT());
        curr = curr->getNext();
    }

    // Update PT queue and check for auto-promotion
    curr = ptQueue.peek();
    Vehicle* prev = NULL;
    while (curr != NULL) {
        curr->setWT(currentTime - curr->getAT());

        // Auto-promote if WT >= autoP
        if (autoP > 0 && curr->getWT() >= autoP && !curr->isPromoted()) {
            // دى هتتنفذ من ال promotion events اللى فى ال TrafficControlCenter
            // For now, just update WT
        }

        prev = curr;
        curr = curr->getNext();
    }

    // Update NC queue and check for auto-cancellation
    curr = ncQueue.peek();
    while (curr != NULL) {
        curr->setWT(currentTime - curr->getAT());

        // Auto-cancelهتتعمل ب ال TrafficControlCenter

        curr = curr->getNext();
    }

    // Update FV queue
    curr = fvQueue.peek();
    while (curr != NULL) {
        curr->setWT(currentTime - curr->getAT());
        curr = curr->getNext();
    }
}


Vehicle* Intersection::selectNextVehicle() {
    // Try EV queue
    Vehicle* curr = evQueue.peek();
    while (curr != NULL) {
        if (!laneBlocked[curr->getLN() - 1]) {
            return evQueue.dequeue();
        }
        curr = curr->getNext();
    }

    // Try PT queue
    curr = ptQueue.peek();
    while (curr != NULL) {
        if (!laneBlocked[curr->getLN() - 1]) {
            return ptQueue.dequeue();
        }
        curr = curr->getNext();
    }

    // Try NC queue
    curr = ncQueue.peek();
    while (curr != NULL) {
        if (!laneBlocked[curr->getLN() - 1]) {
            return ncQueue.dequeue();
        }
        curr = curr->getNext();
    }

    // Try FV queue
    curr = fvQueue.peek();
    while (curr != NULL) {
        if (!laneBlocked[curr->getLN() - 1]) {
            return fvQueue.dequeue();
        }
        curr = curr->getNext();
    }

    return NULL;
}

void Intersection::processCrossing() {
    trafficLight->updateCooldown(); 

    if (crossingVehicle != NULL) {
        crossingRemaining--;
        if (crossingRemaining <= 0) {
            crossingVehicle = NULL;
            trafficLight->reset();
        }
        return;
    }

    // Can only select new vehicle if cooldown is done
    if (!trafficLight->canSwitch()) return;
    Vehicle* next = selectNextVehicle();
    if (next != NULL) {
        crossingVehicle = next;
        crossingRemaining = next->getXD();
        trafficLight->switchTo(next->getLN() - 1);
    }
}

void Intersection::assignGreenLane() {
    if (crossingVehicle != NULL) {
        return; 
    }
    Vehicle* next = NULL;

    // Check priority: EV > PT > FV > NC
    if (!evQueue.isEmpty()) {
        next = evQueue.peek();
    }
    else if (!ptQueue.isEmpty()) {
        next = ptQueue.peek();
    }
    else if (!fvQueue.isEmpty()) {
        next = fvQueue.peek();
    }
    else if (!ncQueue.isEmpty()) {
        next = ncQueue.peek();
    }

    if (next != NULL) {
        trafficLight->switchTo(next->getLN() - 1);
    }
}
bool Intersection::canInterruptForEV() {
    if (crossingVehicle == nullptr) return true;
    if (strcmp(crossingVehicle->getTYPE(), "FV") == 0) return false;
    return true;
}

// Queue Access
PriorityQueue* Intersection::getEVQueue() { return &evQueue; }
Queue* Intersection::getPTQueue() { return &ptQueue; }
Queue* Intersection::getNCQueue() { return &ncQueue; }
Queue* Intersection::getFVQueue() { return &fvQueue; }

bool Intersection::hasVehicles() const {
    return !evQueue.isEmpty() ||
        !ptQueue.isEmpty() ||
        !ncQueue.isEmpty() ||
        !fvQueue.isEmpty() ||
        crossingVehicle != nullptr;
}
