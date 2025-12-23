#include "TrafficControlCenter.h"
#include "AdjacencyList.h"


TrafficControlCenter::TrafficControlCenter(UI* userInterface) {
    ui = userInterface;
    numIntersections = 0;
    eventHead = nullptr;
    completedVehicles = nullptr;
    vehicleCount = 0;
    currentTime = 0;
    switchingCost = 0;
    autoP = 0;
    cancelT = 0;
    reroutingEnabled = false;

    for (int i = 0; i < 50; i++) intersections[i] = nullptr;
    for (int i = 0; i < 10000; i++) allVehicles[i] = nullptr;
}

bool TrafficControlCenter::loadFromFile(const char* filename) {
    std::ifstream infile(filename);
    if (!infile) {
        ui->printError("Cannot open input file");
        return false;
    }

    infile >> numIntersections >> switchingCost >> autoP >> cancelT;
    char rerouteFlag[10];
    infile >> rerouteFlag;
    reroutingEnabled = (strcmp(rerouteFlag, "ON") == 0);

    for (int i = 0; i < numIntersections; i++) {
        intersections[i] = new Intersection();
        intersections[i]->setID(i + 1);
        intersections[i]->getTrafficLight()->setSwitchingCost(switchingCost);
    }

    char line[100];
    infile.getline(line, 100);  
    infile.getline(line, 100);

    for (int i = 0; i < numIntersections; i++) {
        int id;
        char colon;
        infile >> id >> colon;
        while (infile.peek() != '\n' && infile.peek() != EOF) {
            int neighbor;
            infile >> neighbor;
            adjacencyMap[id - 1].addConnection(neighbor);
        }
        infile.ignore();
    }

    int numEvents;
    infile >> numEvents;

    for (int i = 0; i < numEvents; i++) {
        Event* e = new Event();
        char eventType[10];
        infile >> eventType;
        e->setType(eventType);

        if (strcmp(eventType, "A") == 0) {
            char vType[5];
            int time, id, inter, lane, xd;
            infile >> vType >> time >> id >> inter >> lane >> xd;
            e->setVehType(vType);
            e->setTime(time);
            e->setVehicleID(id);
            e->setIntersection(inter);
            e->setLane(lane);
            e->setXD(xd);
            if (strcmp(vType, "EV") == 0) {
                int pr; infile >> pr; e->setPR(pr);
            }
        }
        else if (strcmp(eventType, "X") == 0 || strcmp(eventType, "P") == 0) {
            int time, id;
            infile >> time >> id;
            e->setTime(time); e->setVehicleID(id);
        }
        else if (strcmp(eventType, "ACC") == 0) {
            int time, inter, lane, dur;
            infile >> time >> inter >> lane >> dur;
            e->setTime(time); e->setIntersection(inter);
            e->setLane(lane); e->setDuration(dur);
        }
        else if (strcmp(eventType, "RC") == 0) {
            int time, inter, dur;
            infile >> time >> inter >> dur;
            e->setTime(time); e->setIntersection(inter);
            e->setDuration(dur);
        }

        insertEvent(e);
    }

    infile.close();
    return true;
}

void TrafficControlCenter::insertEvent(Event* e) {
    // Define the event priorities for same timestep
    auto getEventPriority = [](const char* type) {
        if (strcmp(type, "X") == 0) return 1;      // Cancellation first
        if (strcmp(type, "P") == 0) return 2;      // Promotion second
        if (strcmp(type, "ACC") == 0) return 3;    // Accident third
        if (strcmp(type, "RC") == 0) return 3;     // Road closure third
        if (strcmp(type, "A") == 0) return 4;      // Arrival fourth
        if (strcmp(type, "UAC") == 0) return 5;    // Unblock after
        if (strcmp(type, "URC") == 0) return 5;    // Unblock after
        return 10;
        };

    if (eventHead == NULL) {
        e->setNext(NULL);
        eventHead = e;
        return;
    }

    int ePriority = getEventPriority(e->getType());

    // Insert at head if earlier time or same time with higher priority
    if (e->getTime() < eventHead->getTime() ||
        (e->getTime() == eventHead->getTime() &&
            ePriority < getEventPriority(eventHead->getType()))) {
        e->setNext(eventHead);
        eventHead = e;
        return;
    }

    // Find correct position to insert the event
    Event* curr = eventHead;
    while (curr->getNext() != NULL) {
        Event* next = curr->getNext();
        if (e->getTime() < next->getTime() ||
            (e->getTime() == next->getTime() &&
                ePriority < getEventPriority(next->getType()))) {
            break;
        }
        curr = curr->getNext();
    }

    e->setNext(curr->getNext());
    curr->setNext(e);
}
void TrafficControlCenter::checkAutoPromotions() {
    if (autoP <= 0) return;

    for (int i = 0; i < numIntersections; i++) {
        Queue* ptQueue = intersections[i]->getPTQueue();
        Vehicle* curr = ptQueue->peek();

        while (curr != NULL) {
            if (curr->getWT() >= autoP && !curr->isPromoted()) {
                // Create promotion event for next timestep
                Event* promoEvent = new Event();
                promoEvent->setType("P");
                promoEvent->setTime(currentTime + 1);
                promoEvent->setVehicleID(curr->getID());
                insertEvent(promoEvent);

                curr->setPromoted(true);  // Mark to avoid duplicate promotions
            }
            curr = curr->getNext();
        }
    }
}
void TrafficControlCenter::rerouteVehiclesFromIntersection(int intIdx) {
    if (!reroutingEnabled) return;

    Intersection* inter = intersections[intIdx];
    int nearestInt = adjacencyMap[intIdx].getFirstConnection();

    if (nearestInt == -1) return; // No connections

    // Reroute EV queue
    Vehicle* curr = inter->getEVQueue()->peek();
    while (curr != NULL) {
        Vehicle* next = curr->getNext();
        curr->setINT(nearestInt);
        curr->setWT(curr->getWT() + 2); 
        curr = next;
    }
    // Move all EVs to new intersection
    while (!inter->getEVQueue()->isEmpty()) {
        Vehicle* v = inter->getEVQueue()->dequeue();
        intersections[nearestInt - 1]->addVehicle(v);
    }

    // Reroute PT queue
    curr = inter->getPTQueue()->peek();
    while (curr != NULL) {
        Vehicle* next = curr->getNext();
        curr->setINT(nearestInt);
        curr->setWT(curr->getWT() + 2);
        curr = next;
    }
    while (!inter->getPTQueue()->isEmpty()) {
        Vehicle* v = inter->getPTQueue()->dequeue();
        intersections[nearestInt - 1]->addVehicle(v);
    }

    // Reroute NC queue
    curr = inter->getNCQueue()->peek();
    while (curr != NULL) {
        Vehicle* next = curr->getNext();
        curr->setINT(nearestInt);
        curr->setWT(curr->getWT() + 2);
        curr = next;
    }
    while (!inter->getNCQueue()->isEmpty()) {
        Vehicle* v = inter->getNCQueue()->dequeue();
        intersections[nearestInt - 1]->addVehicle(v);
    }

    // Reroute FV queue
    curr = inter->getFVQueue()->peek();
    while (curr != NULL) {
        Vehicle* next = curr->getNext();
        curr->setINT(nearestInt);
        curr->setWT(curr->getWT() + 2);
        curr = next;
    }
    while (!inter->getFVQueue()->isEmpty()) {
        Vehicle* v = inter->getFVQueue()->dequeue();
        intersections[nearestInt - 1]->addVehicle(v);
    }
}

// Similarly for auto-cancellations
void TrafficControlCenter::checkAutoCancellations() {
    if (cancelT <= 0) return;

    for (int i = 0; i < numIntersections; i++) {
        Queue* ncQueue = intersections[i]->getNCQueue();
        Vehicle* curr = ncQueue->peek();

        while (curr != NULL) {
            if (curr->getWT() >= cancelT && !curr->isCanceled()) {
                // Create cancellation event for next timestep
                Event* cancelEvent = new Event();
                cancelEvent->setType("X");
                cancelEvent->setTime(currentTime + 1);
                cancelEvent->setVehicleID(curr->getID());
                insertEvent(cancelEvent);
            }
            curr = curr->getNext();
        }
    }
}
void TrafficControlCenter::processEvents() {
    while (eventHead && eventHead->getTime() == currentTime) {
        Event* e = eventHead;
        eventHead = eventHead->getNext();

        if (strcmp(e->getType(), "A") == 0) handleArrival(e);
        else if (strcmp(e->getType(), "X") == 0) handleCancellation(e);
        else if (strcmp(e->getType(), "P") == 0) handlePromotion(e);
        else if (strcmp(e->getType(), "ACC") == 0) handleAccident(e);
        else if (strcmp(e->getType(), "RC") == 0) handleRoadClosure(e);
        else if (strcmp(e->getType(), "UAC") == 0)
            intersections[e->getIntersection() - 1]->setLaneBlocked(e->getLane() - 1, false);
        else if (strcmp(e->getType(), "URC") == 0)
            intersections[e->getIntersection() - 1]->setBlocked(false);

        delete e;
    }
}

void TrafficControlCenter::handleArrival(Event* e) {
    Vehicle* v = new Vehicle();
    v->setAT(e->getTime()); v->setID(e->getVehicleID());
    v->setTYPE(e->getVehType()); v->setINT(e->getIntersection());
    v->setLN(e->getLane()); v->setXD(e->getXD());
    v->setPR(e->getPR()); v->setWT(0);
    allVehicles[vehicleCount++] = v;

    int intIdx = e->getIntersection() - 1;
    intersections[intIdx]->addVehicle(v);

    if (strcmp(e->getVehType(), "EV") == 0)
        handleEVInterrupt(intIdx);
}

void TrafficControlCenter::handleEVInterrupt(int intIdx) {
    Intersection* inter = intersections[intIdx];

    // Check if we can interrupt current crossing
    if (inter->getCrossingVehicle() != NULL) {
        Vehicle* crossing = inter->getCrossingVehicle();

        // FV cannot be interrupted
        if (strcmp(crossing->getTYPE(), "FV") == 0) {
            return;
        }

        //Clear crossing state BEFORE re-enqueueing
        inter->setCrossingVehicle(NULL);
        inter->setCrossingRemaining(0);
        inter->getTrafficLight()->reset();

        // Put crossing vehicle back in its queue
        const char* type = crossing->getTYPE();
        if (strcmp(type, "EV") == 0) {
            inter->getEVQueue()->enqueue(crossing);
        }
        else if (strcmp(type, "PT") == 0) {
            inter->getPTQueue()->enqueue(crossing);
        }
        else if (strcmp(type, "NC") == 0) {
            inter->getNCQueue()->enqueue(crossing);
        }
    }
}
void TrafficControlCenter::handlePromotion(Event* e) {
    for (int i = 0; i < vehicleCount; i++) {
        if (allVehicles[i] != NULL && allVehicles[i]->getID() == e->getVehicleID()) {
            Vehicle* v = allVehicles[i];

            // Ignore if already crossed
            if (v->getCT() > 0) break;

            // Remove from PT queue
            int intIdx = v->getINT() - 1;
            intersections[intIdx]->removeVehicle(e->getVehicleID());

            // Change type and add to EV queue
            v->setTYPE("EV");
            v->setPromoted(true);
            intersections[intIdx]->addVehicle(v);

            // Handle EV interrupt
            handleEVInterrupt(intIdx);
            break;
        }
    }
}
void TrafficControlCenter::handleCancellation(Event* e) {

    for (int i = 0; i < vehicleCount; i++) {
        if (allVehicles[i] != NULL && allVehicles[i]->getID() == e->getVehicleID()) {
            // Ignore if already crossed
            if (allVehicles[i]->getCT() > 0) {
                break; 
            }

            allVehicles[i]->setCanceled(true);
            allVehicles[i]->setWT(e->getTime() - allVehicles[i]->getAT());

            int intIdx = allVehicles[i]->getINT() - 1;
            intersections[intIdx]->removeVehicle(e->getVehicleID());
            break;
        }
    }
}


void TrafficControlCenter::handleAccident(Event* e) {
    int intIdx = e->getIntersection() - 1;
    int laneIdx = e->getLane() - 1;
    intersections[intIdx]->setLaneBlocked(laneIdx, true);

    // If vehicle is crossing in this lane, interrupt it
    Vehicle* crossing = intersections[intIdx]->getCrossingVehicle();
    if (crossing != NULL && crossing->getLN() == e->getLane()) {
        // Return to queue
        intersections[intIdx]->setCrossingVehicle(NULL);
        intersections[intIdx]->setCrossingRemaining(0);
        intersections[intIdx]->addVehicle(crossing);
        intersections[intIdx]->getTrafficLight()->reset();
    }

    // Create unblock event
    Event* unblock = new Event();
    unblock->setType("UAC");
    unblock->setTime(e->getTime() + e->getDuration());
    unblock->setIntersection(e->getIntersection());
    unblock->setLane(e->getLane());
    insertEvent(unblock);
}
void TrafficControlCenter::handleRoadClosure(Event* e) {
    int intIdx = e->getIntersection() - 1;
    intersections[intIdx]->setBlocked(true);

    // Interrupt any crossing vehicle
    Vehicle* crossing = intersections[intIdx]->getCrossingVehicle();
    if (crossing != NULL) {
        intersections[intIdx]->setCrossingVehicle(NULL);
        intersections[intIdx]->setCrossingRemaining(0);
        intersections[intIdx]->addVehicle(crossing);
        intersections[intIdx]->getTrafficLight()->reset();
    }

    // If rerouting enabled, reroute all waiting vehicles
    if (reroutingEnabled) {
        rerouteVehiclesFromIntersection(intIdx);
    }

    // Create reopen event
    Event* reopen = new Event();
    reopen->setType("URC");
    reopen->setTime(e->getTime() + e->getDuration());
    reopen->setIntersection(e->getIntersection());
    insertEvent(reopen);
}

void TrafficControlCenter::updateAllWaitingTimes() {
    for (int i = 0;i < numIntersections;i++)
        intersections[i]->updateWaitingTimes(currentTime, autoP, cancelT);
}

void TrafficControlCenter::assignGreenLanes() {
    for (int i = 0;i < numIntersections;i++)
        intersections[i]->assignGreenLane();
}

void TrafficControlCenter::processAllCrossing() {
    for (int i = 0; i < numIntersections; i++) {
        Vehicle* v = intersections[i]->getCrossingVehicle();
        int remainingBefore = intersections[i]->getCrossingRemaining();

        intersections[i]->processCrossing();

        // If vehicle just completed crossing clear it
        if (v != NULL && remainingBefore == 1 &&
            intersections[i]->getCrossingRemaining() == 0) {
            v->setCT(currentTime);
            v->setNext(completedVehicles);
            completedVehicles = v;
            intersections[i]->setCrossingVehicle(NULL); 
        }
    }
}

bool TrafficControlCenter::hasActiveVehicles() {
    if (eventHead) return true;
    for (int i = 0;i < numIntersections;i++)
        if (intersections[i]->hasVehicles()) return true;
    return false;
}

void TrafficControlCenter::simulate(int mode) {
    if (mode == 3) ui->printSimulationStart();

    while (hasActiveVehicles()) {
        if (mode == 1 || mode == 2) ui->printTimestep(currentTime);

        updateAllWaitingTimes(); 
        checkAutoPromotions();   
        checkAutoCancellations();
        processEvents();          
        assignGreenLanes();       
        processAllCrossing();    

        //Increment time active per timestep
        for (int i = 0; i < numIntersections; i++) {
            intersections[i]->incrementTimeActive();
        }

        // Display intersection state
        if (mode == 1 || mode == 2) {
            for (int i = 0; i < numIntersections; i++) {
                ui->printIntersection(intersections[i], i + 1);
            }
        }

        if (mode == 1) ui->waitForEnter();
        currentTime++;
    }

    if (mode == 3) ui->printSimulationEnd();
}

void TrafficControlCenter::sortVehiclesByCT() {
    // Separate canceled and completed vehicles
    Vehicle* completed[10000];
    Vehicle* canceled[10000];
    int completedCount = 0;
    int canceledCount = 0;

    // Separate vehicles
    for (int i = 0; i < vehicleCount; i++) {
        if (allVehicles[i] != NULL) {
            if (allVehicles[i]->isCanceled()) {
                canceled[canceledCount++] = allVehicles[i];
            }
            else {
                completed[completedCount++] = allVehicles[i];
            }
        }
    }

    // Sort completed vehicles by CT 
    for (int i = 0; i < completedCount - 1; i++) {
        for (int j = 0; j < completedCount - i - 1; j++) {
            if (completed[j]->getCT() > completed[j + 1]->getCT()) {
                Vehicle* temp = completed[j];
                completed[j] = completed[j + 1];
                completed[j + 1] = temp;
            }
        }
    }

    // Sort canceled vehicles by ID
    for (int i = 0; i < canceledCount - 1; i++) {
        for (int j = 0; j < canceledCount - i - 1; j++) {
            if (canceled[j]->getID() > canceled[j + 1]->getID()) {
                Vehicle* temp = canceled[j];
                canceled[j] = canceled[j + 1];
                canceled[j + 1] = temp;
            }
        }
    }

    int idx = 0;
    for (int i = 0; i < completedCount; i++) {
        allVehicles[idx++] = completed[i];
    }
    for (int i = 0; i < canceledCount; i++) {
        allVehicles[idx++] = canceled[i];
    }
}


void TrafficControlCenter::writeOutput(const char* filename) {
    //Sort vehicles before writing the output
    sortVehiclesByCT();

    std::ofstream outfile(filename);
    if (!outfile) {
        ui->printError("Cannot open output file");
        return;
    }

    int totalEV = 0, totalPT = 0, totalNC = 0, totalFV = 0;
    int totalPromoted = 0, totalCanceled = 0;
    int totalWT = 0, totalXD = 0;
    int wtEV = 0, wtPT = 0, wtNC = 0, wtFV = 0;
    int countEV = 0, countPT = 0, countNC = 0, countFV = 0;
    int totalSwitches = 0;

    //Calculate intersection utilization
    int totalTimeActive = 0;
    int totalPossibleTime = currentTime * numIntersections;

    for (int i = 0; i < numIntersections; i++) {
        totalSwitches += intersections[i]->getTrafficLight()->getSwitchCount();
        totalTimeActive += intersections[i]->getTimeActive();
    }
    if (totalPossibleTime > 0) {
        double utilization = ((double)totalTimeActive / totalPossibleTime) * 100.0;
        outfile << "Intersection Utilization: " << utilization << "%\n";
    }


    outfile << "CT ID AT WT XD TYPE INT LN\n";

    // Write vehicle data
    for (int i = 0; i < vehicleCount; i++) {
        Vehicle* v = allVehicles[i];
        if (!v) continue;

        if (v->isCanceled()) {
            outfile << "-- " << v->getID() << " " << v->getAT() << " "
                << v->getWT() << " " << v->getXD() << " " << v->getTYPE()
                << " " << v->getINT() << " " << v->getLN() << " CANCELED\n";
            totalCanceled++;
        }
        else {
            outfile << v->getCT() << " " << v->getID() << " " << v->getAT()
                << " " << v->getWT() << " " << v->getXD() << " "
                << v->getTYPE() << " " << v->getINT() << " " << v->getLN() << "\n";
        }

        const char* type = v->getTYPE();
        if (strcmp(type, "EV") == 0) {
            totalEV++;
            wtEV += v->getWT();
            countEV++;
            if (v->isPromoted()) totalPromoted++;
        }
        else if (strcmp(type, "PT") == 0) {
            totalPT++;
            wtPT += v->getWT();
            countPT++;
        }
        else if (strcmp(type, "NC") == 0) {
            totalNC++;
            wtNC += v->getWT();
            countNC++;
        }
        else if (strcmp(type, "FV") == 0) {
            totalFV++;
            wtFV += v->getWT();
            countFV++;
        }

        totalWT += v->getWT();
        totalXD += v->getXD();
    }

    // Write statistics summary
    outfile << "\nTotal Vehicles: " << vehicleCount << "\n";

    outfile << "Total EV: " << totalEV;
    if (totalPromoted > 0) outfile << " (including promotions)";
    outfile << "\n";

    outfile << "Total PT: " << totalPT;
    if (totalPromoted > 0) outfile << " (" << totalPromoted << " promoted)";
    outfile << "\n";

    outfile << "Total NC: " << totalNC;
    if (totalCanceled > 0) {
        int canceledNC = 0;
        for (int i = 0; i < vehicleCount; i++) {
            if (allVehicles[i] && allVehicles[i]->isCanceled() &&
                strcmp(allVehicles[i]->getTYPE(), "NC") == 0) {
                canceledNC++;
            }
        }
        if (canceledNC > 0) outfile << " (" << canceledNC << " canceled)";
    }
    outfile << "\n";

    outfile << "Total FV: " << totalFV << "\n";

    // Average waiting times
    double avgWT = (vehicleCount > 0) ? ((double)totalWT / vehicleCount) : 0.0;
    outfile << "Average WT (all): " << avgWT << "\n";

    if (countEV > 0)
        outfile << "Average WT (EV): " << ((double)wtEV / countEV) << "\n";
    if (countPT > 0)
        outfile << "Average WT (PT): " << ((double)wtPT / countPT) << "\n";
    if (countNC > 0)
        outfile << "Average WT (NC): " << ((double)wtNC / countNC) << "\n";
    if (countFV > 0)
        outfile << "Average WT (FV): " << ((double)wtFV / countFV) << "\n";

    // Average crossing duration
    double avgXD = (vehicleCount > 0) ? ((double)totalXD / vehicleCount) : 0.0;
    outfile << "Average XD: " << avgXD << "\n";

    // Auto-promotion percentage
    int totalPTOriginal = totalPT + totalPromoted;
    double promotionPercent = (totalPTOriginal > 0) ?
        ((double)totalPromoted / totalPTOriginal * 100.0) : 0.0;
    outfile << "% Auto-Promoted PT: " << promotionPercent << "%\n";

    // Signal switches
    outfile << "Signal Switches: " << totalSwitches << "\n";

    // Cancellation percentage
    double cancelPercent = (vehicleCount > 0) ?
        ((double)totalCanceled / vehicleCount * 100.0) : 0.0;
    outfile << "% Vehicles Canceled: " << cancelPercent << "%\n";

    if (totalPossibleTime > 0) {
        double utilization = ((double)totalTimeActive / totalPossibleTime) * 100.0;
        outfile << "Intersection Utilization: " << utilization << "%\n";
    }

    outfile.close();
}