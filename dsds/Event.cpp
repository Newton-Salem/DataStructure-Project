#include "Event.h"
#include <cstring>

// 
// Constructor
Event::Event() {
    strcpy(type, "");
    strcpy(vehType, "");
    time = vehicleID = intersection = lane = duration = xd = pr = 0;
    next = nullptr;
}

//getters
const char* Event::getType() const { return type; }
int Event::getTime() const { return time; }
int Event::getVehicleID() const { return vehicleID; }
const char* Event::getVehType() const { return vehType; }
int Event::getIntersection() const { return intersection; }
int Event::getLane() const { return lane; }
int Event::getDuration() const { return duration; }
int Event::getXD() const { return xd; }
int Event::getPR() const { return pr; }
Event* Event::getNext() const { return next; }

//setters
void Event::setType(const char* t) { strcpy(type, t); }
void Event::setTime(int t) { time = t; }
void Event::setVehicleID(int id) { vehicleID = id; }
void Event::setVehType(const char* t) { strcpy(vehType, t); }
void Event::setIntersection(int i) { intersection = i; }
void Event::setLane(int l) { lane = l; }
void Event::setDuration(int d) { duration = d; }
void Event::setXD(int x) { xd = x; }
void Event::setPR(int p) { pr = p; }
void Event::setNext(Event* n) { next = n; }
