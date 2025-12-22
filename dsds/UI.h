#pragma once
#pragma once

#include <iostream>
#include <cstring>
using namespace std;

class Intersection;

// ============================================================================
// UI CLASS
// ============================================================================
class UI {
public:
    // Constructor
    UI();

    // ================= Input =================
    void printWelcome();
    void getInputFilename(char* filename);
    void getOutputFilename(char* filename);
    int getSimulationMode();

    // ================= Output =================
    void printTimestep(int time);
    void printIntersection(Intersection* inter, int intNum);
    void waitForEnter();
    void printSimulationStart();
    void printSimulationEnd();
    void printError(const char* msg);
};
