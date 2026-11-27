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