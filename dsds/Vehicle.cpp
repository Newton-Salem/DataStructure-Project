#include "Vehicle.h"
#include <cstring>

// ============================================================================
// Constructor
// ============================================================================
Vehicle::Vehicle() {
    AT = ID = INT = LN = XD = PR = WT = CT = 0;
    strcpy(TYPE, "");
    canceled = false;
    promoted = false;
    next = nullptr;
}

// ============================================================================
// Getters
// ============================================================================
int Vehicle::getAT() const { return AT; }
int Vehicle::getID() const { return ID; }
const char* Vehicle::getTYPE() const { return TYPE; }
int Vehicle::getINT() const { return INT; }
int Vehicle::getLN() const { return LN; }
int Vehicle::getXD() const { return XD; }
int Vehicle::getPR() const { return PR; }
int Vehicle::getWT() const { return WT; }
int Vehicle::getCT() const { return CT; }
bool Vehicle::isCanceled() const { return canceled; }
bool Vehicle::isPromoted() const { return promoted; }
Vehicle* Vehicle::getNext() const { return next; }

// ============================================================================
// Setters
// ============================================================================
void Vehicle::setAT(int at) { AT = at; }
void Vehicle::setID(int id) { ID = id; }
void Vehicle::setTYPE(const char* type) { strcpy(TYPE, type); }
void Vehicle::setINT(int i) { INT = i; }
void Vehicle::setLN(int ln) { LN = ln; }
void Vehicle::setXD(int xd) { XD = xd; }
void Vehicle::setPR(int pr) { PR = pr; }
void Vehicle::setWT(int wt) { WT = wt; }
void Vehicle::setCT(int ct) { CT = ct; }
void Vehicle::setCanceled(bool c) { canceled = c; }
void Vehicle::setPromoted(bool p) { promoted = p; }
void Vehicle::setNext(Vehicle* n) { next = n; }

// ============================================================================
// Logic
// ============================================================================
double Vehicle::calculatePriority() const {
    if (strcmp(TYPE, "EV") == 0) {
        return (PR * 2.0) - WT + 10000.0;
    }
    return 0.0;
}
