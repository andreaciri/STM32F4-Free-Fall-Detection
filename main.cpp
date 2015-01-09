
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
    toSend[1] = CTRL_REG4_XEN | CTRL_REG4_YEN | CTRL_REG4_ZEN ; //accelerometer axis enabled
    toSend[1] |= CTRL_REG4_ODR0 | CTRL_REG4_ODR1 | CTRL_REG4_ODR2 ; //output data rate at 400 Hz
    spi.writeAndRead(toSend, write);
    toSend[0] = (CTRL_REG3);
    toSend[1] = CTRL_REG3_INT1EN ; //interrupt1 enabled, signals when data are ready
    toSend[1] |= CTRL_REG3_IEA ; //interrupt signals active high
    spi.writeAndRead(toSend, write);
    uint8_t toRead = (CTRL_REG4);
    uint8_t *response = spi.writeAndRead(&toRead, read);
    uint8_t tempResponse = (uint8_t)*response;
    
    testLed();
}