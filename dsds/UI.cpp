#include "UI.h"
#include "Intersection.h"

// Constructor
UI::UI() {}

// Input
void UI::getInputFilename(char* filename) {
    cout << "Enter input filename: ";
    cin.getline(filename, 100);
}
//Output
void UI::getOutputFilename(char* filename) {
    cout << "Enter output filename: ";
    cin.getline(filename, 100);
}
//allow user to choose the mood 
int UI::getSimulationMode() {
    cout << "\nSelect Simulation Mode:\n";
    cout << "1. Interactive (press Enter to advance)\n";
    cout << "2. Step-by-step (auto-advance with display)\n";
    cout << "3. Silent (no display, output file only)\n";
    cout << "Enter choice: ";

    int mode;
    cin >> mode;
    cin.ignore();
    return mode;
}
//cmd output
void UI::printTimestep(int time) {
    cout << "\n========================================\n";
    cout << "Current Timestep: " << time << "\n";
    cout << "========================================\n";
}

void UI::printIntersection(Intersection* inter, int intNum) {
    cout << "\n=============== INTERSECTION #" << intNum << " ===============\n";

    // EV queue
    cout << "EV queue: ";
    if (inter->getCrossingVehicle() &&
        strcmp(inter->getCrossingVehicle()->getTYPE(), "EV") == 0) {
        cout << inter->getCrossingVehicle()->getID() << " ";
    }
    inter->getEVQueue()->printIDs();
    cout << "\n";

    // PT queue
    cout << "PT queue: ";
    if (inter->getCrossingVehicle() &&
        strcmp(inter->getCrossingVehicle()->getTYPE(), "PT") == 0) {
        cout << inter->getCrossingVehicle()->getID() << " ";
    }
    inter->getPTQueue()->printIDs();
    cout << "\n";

    // NC queue
    cout << "NC queue: ";
    if (inter->getCrossingVehicle() &&
        strcmp(inter->getCrossingVehicle()->getTYPE(), "NC") == 0) {
        cout << inter->getCrossingVehicle()->getID() << " ";
    }
    inter->getNCQueue()->printIDs();
    cout << "\n";

    // FV queue
    cout << "FV queue: ";
    if (inter->getCrossingVehicle() &&
        strcmp(inter->getCrossingVehicle()->getTYPE(), "FV") == 0) {
        cout << inter->getCrossingVehicle()->getID() << " ";
    }
    inter->getFVQueue()->printIDs();
    cout << "\n";

    // Active lane
    int greenLane = inter->getTrafficLight()->getCurrentGreenLane();
    if (greenLane != -1)
        cout << "Active Green Lane: LN" << (greenLane + 1) << "\n";
    else
        cout << "Active Green Lane: NONE\n";

    // Crossing vehicle
    if (inter->getCrossingVehicle()) {
        cout << "Vehicles Crossing: "
            << inter->getCrossingVehicle()->getID()
            << " (XD remaining = "
            << inter->getCrossingRemaining() << ")\n";
    }

    cout << "----------------------------------------------\n";
}

void UI::waitForEnter() {
    cout << "\nPress Enter...\n";
    cin.get();
}

void UI::printSimulationStart() {
    cout << "\n>>> Simulation Starting...\n\n";
}

void UI::printSimulationEnd() {
    cout << "\n>>> Simulation Complete!\n";
}

void UI::printError(const char* msg) {
    cout << "ERROR: " << msg << "\n";
}
