#include <miosix.h>
#include "lis3dsh.h"

using namespace miosix;

#define MIN_DURATION    100
#define THRESHOLD       300

Lis3dsh accelerometer;

int main()
{
    accelerometer.freeFallConfig(MIN_DURATION, THRESHOLD);
    accelerometer.freeFallDetectionInit();
}