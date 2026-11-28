#include "viewer_iface.h"
#include "util.h"
#include "level.h"
#include <iostream>
using namespace std;

// Terminal UI and dashboard rendering using primitive parameters

void renderDashboard(int playerFloor, int playerRoom, int energy, int time, int score,
                     const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                     int emergencyTripsLeft)
{
    printHeader("STATUS");
    cout << "Location: F" << playerFloor + 1 << "-R" << playerRoom + 1 << endl;
    cout << "Energy: " << energy << " / " << MAX_ENERGY << endl;
    cout << "Time: " << time << " / " << MAX_TIME << endl;
    cout << "Score: " << score << endl;
    cout << "Emergency lift trips left: " << emergencyTripsLeft << endl;

    cout << "Inventory: ";
    if (inventoryCount == 0) {
        cout << "(empty)";
    }
    for (int i = 0; i < inventoryCount; i++) {
        if (i > 0) {
            cout << ", ";
        }
        cout << getEntityName(inventory[i]);
    }
    cout << endl;
}

void renderRoomDetails(int floor, int room,
                       const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                       int entityCount)
{
    cout << "=====================================" << endl << "ROOM F" << floor + 1 << "-R" << room + 1 << endl << "=====================================" << endl;
    if (entityCount == 0) {
        cout << "Nothing here." << endl;
    }
    else {
        cout << "Items and objects here: " << endl;
        for (int i = 0; i < entityCount; i++) {
            cout << "- " << getEntityName(roomEntities[floor][room][i]) << endl;
        }
    }
}

void renderObjectivesChecklist(const bool objectives[NUM_OBJECTIVES])
{
    const char* names[NUM_OBJECTIVES] = {
        "Master Key collected",
        "Clue digit 4 found",
        "Clue digit 2 found",
        "Clue digit 7 found",
        "Cabinet unlocked",
        "Train Key acquired",
        "Fuse collected",
        "Fuse installed",
        "Lift Pass collected",
        "Lift 2 active",
        "Passenger rescued",
        "Cat Carrier collected",
        "Cat secured",
        "Train prepared",
        "Mission won",
        "Mission failed"
    };

    printHeader("OBJECTIVES");
    for (int i = 0; i < NUM_OBJECTIVES; i++) {
        if (objectives[i] == true) {
            cout << "[X] " << names[i] << endl;
        }
        else {
            cout << "[ ] " << names[i] << endl;
        }
    }
}

void renderAsciiBuilding(int playerFloor, int playerRoom)
{
    printHeader("BUILDING");
    for (int f = NUM_FLOORS - 1; f >= 0; f--) {
        cout << "F" << f + 1 << "  ";
        for (int r = 0; r < ROOMS_PER_FLOOR; r++) {
            if (f == playerFloor && r == playerRoom) {
                cout << "[P] ";
            }
            else {
                cout << "[ ] ";
            }
        }
        cout << endl;
    }
}