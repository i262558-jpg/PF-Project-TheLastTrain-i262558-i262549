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

    cout << lines[8] << "\n";
    cout << lines[9] << "\n";
    
    file.close();
    return true;
}