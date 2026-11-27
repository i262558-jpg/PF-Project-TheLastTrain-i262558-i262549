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