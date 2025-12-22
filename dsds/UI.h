#pragma once
#pragma once

#include <iostream>
#include <cstring>
using namespace std;

class Intersection;


class UI {
public:
    // Constructor
    UI();

    void getInputFilename(char* filename);
    void getOutputFilename(char* filename);
    int getSimulationMode();

    void printTimestep(int time);
    void printIntersection(Intersection* inter, int intNum);
    void waitForEnter();
    void printSimulationStart();
    void printSimulationEnd();
    void printError(const char* msg);
};
