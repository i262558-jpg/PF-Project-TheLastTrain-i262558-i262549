#include <iostream>
#include <string>
#include "level.h"
#include "util.h"
#include <fstream>

using namespace std;
// Modular implementation using primitive arrays for map parsing and verification

bool loadBuildingMap(const char* filename,
                     char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                     char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                     int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR])
{
    ifstream file(filename);
    if(!file.is_open()){
        return false;
    }

    string lines[20];
    int lineCount = 0;

    string line;
    while (getline(file, line) && lineCount < 20) {
        lines[lineCount] = line;
        lineCount++;
    }

    for (int r = 0; r < NUM_FLOORS; r++) {
        for (int c = 0; c < ROOMS_PER_FLOOR - 1; c++) {
            horizontalConn[r][c] = lines[14 - 3 * r][20 + 12 * c];
        }
    }

    for (int r = 0; r < NUM_FLOORS; r++) {
        for (int c = 0; c < ROOMS_PER_FLOOR; c++) {
            for (int k = 9 + 12 * c; k < 20 + 12 * c; k++) {
                char ch = lines[15 - 3 * r][k];
                if (ch != ' ' && entityCounts[r][c] < MAX_ENTITIES_PER_ROOM) {
                    roomEntities[r][c][entityCounts[r][c]] = ch;
                    entityCounts[r][c]++;
                }
            }
        }
    }

    file.close();
    return true;
}

const char* getEntityName(char entitySymbol)
{
    if (entitySymbol == '<') {
        return "Master Key";
    }
    if (entitySymbol == ')') {
        return "Energy Pack";
    }
    if (entitySymbol == '\\') {
        return "Emergency Toolkit";
    }
    if (entitySymbol == '4') {
        return "Clue Digit 4";
    }
    if (entitySymbol == '2') {
        return "Clue Digit 2";
    }
    if (entitySymbol == '7') {
        return "Clue Digit 7";
    }
    if (entitySymbol == '*') {
        return "Fuse";
    }
    if (entitySymbol == '~') {
        return "Cat";
    }
    if (entitySymbol == ';') {
        return "Control Panel";
    }
    if (entitySymbol == '(') {
        return "Lift Pass";
    }
    if (entitySymbol == '!') {
        return "Injured Passenger";
    }
    if (entitySymbol == '%') {
        return "Medical Kit";
    }
    if (entitySymbol == '$') {
        return "Emergency Access Card";
    }
    if (entitySymbol == '^') {
        return "Cabinet";
    }
    if (entitySymbol == '@') {
        return "Cat Carrier";
    }
    if (entitySymbol == '&') {
        return "Observation Token";
    }
    return "Unknown";
}

bool validateStartingRoom(int startFloor, int startRoom,
                          const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1])
{
    (void)horizontalConn;

    if (startFloor < 0 || startFloor >= NUM_FLOORS) {
        return false;
    }
    if(startRoom < 0 || startRoom >= ROOMS_PER_FLOOR){
        return false;
    }
    return true;
}

bool loadInitialState(const char* filename,
                      int& playerFloor, int& playerRoom,
                      int& energy, int& time, int& score,
                      char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                      bool objectives[NUM_OBJECTIVES], int& emergencyTripsLeft)
{
    (void)playerFloor;
    (void)playerRoom;
    (void)inventory;

    ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    inventoryCount = 0;
    emergencyTripsLeft = 0;
    for (int i = 0; i < NUM_OBJECTIVES; i++) {
        objectives[i] = false;
    }

    string line;
    while (getline(file, line)) {
        if (line.find("INITIAL_ENERGY:") == 0) {
            energy = stoi(line.substr(15));
        }
        if (line.find("INITIAL_TIME:") == 0) {
            time = stoi(line.substr(13));
        }
        if (line.find("INITIAL_SCORE:") == 0) {
            score = stoi(line.substr(14));
        }
    }

    file.close();
    return true;
}