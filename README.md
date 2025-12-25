# 🚦 Smart City Traffic Management System  
**Data Structures & Algorithms Project – Fall 2025**

---

## 📌 Project Overview
This project simulates a **Traffic Control Center (TCC)** for a smart city.  
The system manages traffic at multiple intersections using an **event-driven simulation** model and **custom data structures** (Queue, Priority Queue, Stack) implemented from scratch **without using STL**.

The simulator handles different vehicle types, dynamic traffic events, prioritization rules, and generates detailed statistics about system performance.

---

## 🎯 Project Objectives
- Design and implement a non-trivial **object-oriented C++ system**
- Select and justify suitable **data structures**
- Apply **event-driven simulation**
- Handle prioritization, fairness, and dynamic events
- Generate a comprehensive **output statistics file**

---

## 🚗 Vehicle Types
- **EV (Emergency Vehicles)** – highest priority, can preempt traffic signals  
- **PT (Public Transport)** – medium priority, auto-promoted to EV after a threshold  
- **NC (Normal Cars)** – lowest priority, may cancel after long waiting  
- **FV (Freight Vehicles)** – long crossing duration, locks lane until finished  

---

## ⏱️ Supported Events
- **Arrival (A)** – vehicle enters the system  
- **Cancellation (X)** – vehicle leaves before being served  
- **Promotion (P)** – PT vehicle promoted to EV  
- **Accident (ACC)** – blocks a lane temporarily  
- **Road Closure (RC)** – blocks an entire intersection  

Events are processed using a **Priority Queue sorted by timestep**.

---

## 🧱 Data Structures Used

| System Component | Data Structure | Justification |
|-----------------|---------------|---------------|
| EV Vehicles | Priority Queue | Based on urgency and waiting time |
| PT Vehicles | Queue | First-Come-First-Served |
| NC Vehicles | Queue | First-Come-First-Served |
| FV Vehicles | Queue | Preserves order, lane locking |
| Event List | Priority Queue | Sorted by event time |
| Intersections | Array of Objects | Fixed system size |
| Lane Queues | Queue of `Vehicle*` | Pointer-based movement |
| Rerouting Map | Adjacency List (Array) | Efficient graph representation |

---

## ⚠️ Project Constraints
- ❌ No C++ STL  
- ❌ No global variables  
- ❌ No friendship  
- ✅ All lists store **pointers only** (no object copying)  
- ✅ All data structures are derived from **Stack, Queue, or Priority Queue**

---

## 🏗️ System Architecture

### Main Classes
- `Vehicle`
- `Event`
- `Intersection`
- `TrafficLight`
- `TrafficControlCenter` (Organizer)
- `UI`

### TrafficControlCenter Responsibilities
- Load input file and initialize system
- Handle events at each timestep
- Manage vehicle queues and intersections
- Apply promotions, cancellations, accidents, and rerouting
- Collect statistics
- Generate final output file

---

## 🧪 Project Phases

### Phase 1.1 – Data Structure Selection
- Identified all required system lists
- Selected appropriate data structures
- Provided full justification

### Phase 1.2 – Implementation & Testing
- Implemented all custom queues and priority queues
- Implemented file loading
- Built a simple simulator to test:
  - Arrivals
  - Promotions
  - Cancellations
- Console output for debugging only

### Phase 2 – Full Simulation
- Traffic light scheduling and switching cost
- Lane assignment logic
- Accidents and road closures
- Rerouting using adjacency maps
- Multiple simulation modes
- Output file generation with statistics

---

## 🖥️ Simulation Modes
1. **Interactive Mode** – step-by-step with user input  
2. **Step-by-Step Mode** – automatic delay between timesteps  
3. **Silent Mode** – no console output, output file only  

---

## 📂 Input & Output

### Input File
Includes:
- System configuration
- Rerouting adjacency map
- List of traffic events

### Output File
Includes:
- Per-vehicle completion or cancellation records
- Waiting time and crossing statistics
- Overall system performance metrics

---

## ▶️ How to Run
```bash
g++ *.cpp -o dsds
./dsds
`````
## Then select:
### Simulation mode
### Input file name
### Output file name


## 👥 Team Members
See **`ID.txt`** for team members’ names, IDs, and emails.
