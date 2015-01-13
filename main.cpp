#include <miosix.h>
#include <pthread.h>
#include "lis3dsh.h"

using namespace miosix;

#define MIN_DURATION    100
#define THRESHOLD       300
#define PRIORITY_MAX    2

/* ============================= LEDS UTILITY =============================== */
typedef Gpio<GPIOD_BASE, 12> greenLed;

void blinkMainLed() 
{
    greenLed::high();
    usleep(1000000);
    greenLed::low();
    usleep(1000000);
}
/* ============================ END LEDS UTILITY =============================*/

Lis3dsh accelerometer;

/**
 * @brief free dall detection thread;
 */
void *freeFallDetection(void *arg)
{
    miosix::Thread::getCurrentThread()->setPriority(PRIORITY_MAX-1);
    accelerometer.freeFallDetectionInit();
}

int main()
{
    pthread_t freeFallDetectionThread;

    accelerometer.freeFallConfig(MIN_DURATION, THRESHOLD);
    pthread_create(&freeFallDetectionThread, NULL, &freeFallDetection, NULL);
    
    for(;;)
    {
        blinkMainLed();
    }
    
    pthread_join(freeFallDetectionThread,NULL);
    
    return 0;
}