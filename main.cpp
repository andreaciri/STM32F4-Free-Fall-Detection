
#include <pthread.h>
#include <miosix.h>
#include <unistd.h>
#include <cstdio>

using namespace miosix;

typedef Gpio<GPIOD_BASE,15> blueLed;

void testLed()
{
        blueLed::high();
        usleep(1000000);  //Blink led
        blueLed::low();
}

void configureSpi() {
    
}

int main()
{
    blueLed::mode(Mode::OUTPUT);
    
    //configureSpi();
    testLed();
}