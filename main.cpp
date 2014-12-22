
#include <pthread.h>
#include <miosix.h>
#include <unistd.h>
#include <cstdio>
#include <spi.h>
#include "lis3dsh_reg.h"

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
    uint8_t toSend[2];
    toSend[0] = (0x20 << 2);
    toSend[1] = (0x57);
    spi.writeAndRead(toSend,2);
    uint8_t toRead = (0x0F << 2) | 1;
    uint8_t *response = spi.writeAndRead(&toRead,1);
    if(*response == 0x33)
    {
        testLed();
    }
}