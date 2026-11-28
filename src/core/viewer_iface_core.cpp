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