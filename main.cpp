
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
    bool write = true, read = false;
    
    spi.config();
    uint8_t toSend[2];
    toSend[0] = (CTRL_REG4);
    toSend[1] = CTRL_REG1_XEN | CTRL_REG1_YEN | CTRL_REG1_ZEN | CTRL_REG1_ODR0 | CTRL_REG1_ODR1 | CTRL_REG1_ODR2; // writing on CNTRL REG4
    spi.writeAndRead(toSend, write);
    toSend[0] = (CTRL_REG3);
    toSend[1] = (0x48);
    spi.writeAndRead(toSend, write);
    uint8_t toRead = (CTRL_REG4);
    uint8_t *response = spi.writeAndRead(&toRead, read);
    uint8_t tempResponse = (uint8_t)*response;
    
    testLed();
}