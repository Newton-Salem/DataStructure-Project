#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

#include "UI.h"
#include "TrafficControlCenter.h"

// ============================================================================
// MAIN FUNCTION
// ============================================================================
int main() {
    UI ui;
    ui.printWelcome();

    char inputFile[100];
    char outputFile[100];

    ui.getInputFilename(inputFile);
    ui.getOutputFilename(outputFile);

    TrafficControlCenter tcc(&ui);

    if (!tcc.loadFromFile(inputFile)) {
        return 1;
    }

    int mode = ui.getSimulationMode();

    tcc.simulate(mode);
    tcc.writeOutput(outputFile);

    ui.printSimulationEnd();

    return 0;
}
