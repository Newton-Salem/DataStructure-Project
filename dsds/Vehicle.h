#pragma once
#include <cstring>


class Vehicle {
private:
    int AT;           // Arrival Time
    int ID;           // Vehicle ID
    char TYPE[5];     // EV, PT, NC, FV
    int INT;          // Intersection ID
    int LN;           // Lane ID
    int XD;           // Crossing Duration
    int PR;           // Priority (for EV)
    int WT;           // Waiting Time
    int CT;           // Completion Time
    bool canceled;
    bool promoted;
    Vehicle* next;

public:
    // Constructor
    Vehicle();
    //getters
    int getAT() const;
    int getID() const;
    const char* getTYPE() const;
    int getINT() const;
    int getLN() const;
    int getXD() const;
    int getPR() const;
    int getWT() const;
    int getCT() const;
    Vehicle* getNext() const;

    bool isCanceled() const;
    bool isPromoted() const;

    //setters
    void setAT(int at);
    void setID(int id);
    void setTYPE(const char* type);
    void setINT(int i);
    void setLN(int ln);
    void setXD(int xd);
    void setPR(int pr);
    void setWT(int wt);
    void setCT(int ct);
    void setCanceled(bool c);
    void setPromoted(bool p);
    void setNext(Vehicle* n);

    double calculatePriority() const;
};
