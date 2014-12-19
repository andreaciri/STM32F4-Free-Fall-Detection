
#include <pthread.h>
#include <miosix.h>
#include <unistd.h>
#include <cstdio>
#include <spi.h>

using namespace miosix;
Spi spi;

typedef Gpio<GPIOD_BASE,15> blueLed;

void testLed()
{
        // commento prova
        blueLed::high();
        usleep(1000000);  //Blink led
        blueLed::low();
}

int main()
{
    blueLed::mode(Mode::OUTPUT);
    
    spi.config();
    
    testLed();
}