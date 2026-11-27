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
    for (int i = 0; i < inventoryCount - 1; i++) {
        if (inventory[i] == itemSymbol) {
            for(int j = i; j < inventoryCount; j++){
                inventory[j] = inventory[j + 1];
            }
            inventoryCount--;
            return true;
        }
    }
    return false;
}