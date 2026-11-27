#include "physics.h"
#include "util.h"

// Movement validation and resource adjustments without structures
void applyResourceDelta(int& energy, int& time, int deltaEnergy, int deltaTime)
{
    energy = energy + deltaEnergy;
    time = time + deltaTime;
    if (energy > MAX_ENERGY) {
        energy = MAX_ENERGY;
    }
    if (energy < 0) {
        energy = 0;
    }
    if (time > MAX_TIME) {
        time = MAX_TIME;
    }
    if (time < 0) {
        time = 0;
    }
}

void applyScoreDelta(int& score, int deltaScore)
{
    score = score + deltaScore;
}

bool hasItem(const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount, char itemSymbol)
{
    for (int i = 0; i < inventoryCount; i++) {
        if (inventory[i] == itemSymbol) {
            return true;
        }
    }
    return false;
}

bool addItem(char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount, char itemSymbol)
{
    if (inventoryCount >= MAX_INVENTORY_CAPACITY) {
        return false;
    }
    inventory[inventoryCount] = itemSymbol;
    inventoryCount++;
    return true;
}

bool removeItem(char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount, char itemSymbol)
{
    for (int i = 0; i < inventoryCount; i++) {
        if (inventory[i] == itemSymbol) {
            for(int j = i; j < inventoryCount - 1; j++){
                inventory[j] = inventory[j + 1];
            }
            inventoryCount--;
            return true;
        }
    }
    return false;
}

bool isDirectlyConnected(int fromF, int fromR, int toF, int toR,
                         const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                         char& outConnType)
{
    outConnType = CONN_NONE;

    if (fromF < 0 || fromF >= NUM_FLOORS || toF < 0 || toF >= NUM_FLOORS ||
        fromR < 0 || fromR >= ROOMS_PER_FLOOR || toR < 0 || toR >= ROOMS_PER_FLOOR) {
        return false;
    }

    if (fromF == toF && (fromR - toR == 1 || toR - fromR == 1)) {
        int left = fromR;
        if (toR < fromR) {
            left = toR;
        }
        char conn = horizontalConn[fromF][left];
        if (conn == CONN_DOOR || conn == CONN_WINDOW || conn == CONN_LOCKED_DOOR) {
            outConnType = conn;
            return true;
        }
    }
    if (fromR == 0 && toR == 0) {
        int low = fromF;
        if (toF < fromF) {
            low = toF;
        }
        if ( (fromF - toF == 1 || toF - fromF == 1) && (low == 0 || low ==1 || low == 3) ) {
            outConnType = 'S';
            return true;
        }
    }

    if ( ((fromF == 0 && toF == 1) || (fromF == 1 && toF == 0)) && ( fromR == 1 || fromR == 2) && ( toR == 1 || toR == 2) ) {
        outConnType = '1';
        return true;
    }

    if ( ((fromF == 1 && toF == 3) || (fromF == 1 && toF == 4) || (fromF == 3 && toF == 1) || (fromF == 3 && toF == 4) || (fromF == 4 && toF == 1) || (fromF == 4 && toF == 3)) && fromR == 3 && toR == 3 ) {
        outConnType = '2';
        return true;
    }

    return false;
}

