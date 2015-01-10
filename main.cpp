
#include <pthread.h>
#include <miosix.h>
#include <unistd.h>
#include <cstdio>
#include "lis3dsh.h"

using namespace miosix;

Lis3dsh accelerometer;
    
/* for the integrability, we decided to define the purpose for which the accelerometer
 is intended for in our project; other possible purposes are for instance the position recognition,
 the double-turn rilevation or the double-tap rilevation, not implemented here.*/
const char purpose[] = "freefall";

int main()
{       
    accelerometer.config(purpose);

}