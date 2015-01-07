
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
    toSend[0] = (0x20); // writing on CNTRL REG4
    toSend[1] = CTRL_REG1_XEN | CTRL_REG1_YEN | CTRL_REG1_ZEN | CTRL_REG1_ODR0 | CTRL_REG1_ODR1 | CTRL_REG1_ODR2; // writing on CNTRL REG4
    spi.writeAndRead(toSend,2);
    toSend[0] = (0x23);
    toSend[1] = (0x48);
    spi.writeAndRead(toSend,2);
    uint8_t toRead = (0xA0); // try to read CNTRL REG4
    uint8_t *response = spi.writeAndRead(&toRead,1);
    uint8_t tempResponse = (uint8_t)*response;
    //if(*response == 255)
    //{
        testLed();
    //}
}