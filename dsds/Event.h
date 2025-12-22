#pragma once
#pragma once
#include <cstring>


class Event {
private:
    char type[5];     // A, X, P, ACC, RC
    int time;
    int vehicleID;
    char vehType[5];
    int intersection;
    int lane;
    int duration;
    int xd;
    int pr;
    Event* next;

public:
    // Constructor
    Event();
    //getters
    const char* getType() const;
    int getTime() const;
    int getVehicleID() const;
    const char* getVehType() const;
    int getIntersection() const;
    int getLane() const;
    int getDuration() const;
    int getXD() const;
    int getPR() const;
    Event* getNext() const;

    //setters
    void setType(const char* t);
    void setTime(int t);
    void setVehicleID(int id);
    void setVehType(const char* t);
    void setIntersection(int i);
    void setLane(int l);
    void setDuration(int d);
    void setXD(int x);
    void setPR(int p);
    void setNext(Event* n);
};
