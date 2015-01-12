
#include <pthread.h>
#include <miosix.h>
#include <unistd.h>
#include <cstdio>
#include "lis3dsh.h"

using namespace miosix;

#define MIN_DURATION    100
#define THRESHOLD       300

Lis3dsh accelerometer;
    
/* for the integrability, we decided to define the purpose for which the accelerometer
 is intended for in our project; other possible purposes are for instance the position recognition,
 the double-turn rilevation or the double-tap rilevation, not implemented here.*/

int main()
{
    accelerometer.freeFallConfig(MIN_DURATION, THRESHOLD);

}