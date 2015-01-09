
#include <pthread.h>
#include <miosix.h>
#include <unistd.h>
#include <cstdio>
#include <spi.h>
#include "lis3dsh_reg.h"

using namespace miosix;
Spi spi;

/* Leds utility */
typedef Gpio<GPIOD_BASE,15> blueLed;
typedef Gpio<GPIOD_BASE,14> redLed;
typedef Gpio<GPIOD_BASE,13> orangeLed;
typedef Gpio<GPIOD_BASE,12> greenLed;

void blinkLeds()
{
    blueLed::high(); redLed::high(); orangeLed::high(); greenLed::high();
    usleep(1000000);
    blueLed::low(); redLed::low(); orangeLed::low(); greenLed::low();
    usleep(1000000);
}

int main()
{
    blueLed::mode(Mode::OUTPUT); redLed::mode(Mode::OUTPUT);
    orangeLed::mode(Mode::OUTPUT); greenLed::mode(Mode::OUTPUT);
    bool write = true, read = false;
    
    spi.config();
    uint8_t toSend[2];
    
    toSend[0] = CTRL_REG4;
    toSend[1] = CTRL_REG4_XEN | CTRL_REG4_YEN | CTRL_REG4_ZEN ; //accelerometer axis enabled
    toSend[1] |= CTRL_REG4_ODR0 | CTRL_REG4_ODR1 | CTRL_REG4_ODR2 ; //output data rate at 400 Hz
    spi.writeAndRead(toSend, write);
    
    toSend[0] = CTRL_REG3;
    toSend[1] = CTRL_REG3_INT1EN ; //interrupt1 enabled, signals when data are ready
    toSend[1] |= CTRL_REG3_IEA ; //interrupt signals active high
    spi.writeAndRead(toSend, write);
    
    toSend[0] = TIM1_1L; //timer 1
    toSend[1] = 0x28 ; //free fall duration 100 ms
    spi.writeAndRead(toSend, write);
    
    toSend[0] = THRS2_1;
    toSend[1] = 0x18 ; //free-fall threshold 375 mg (threshold 2)
    spi.writeAndRead(toSend, write);
    
    toSend[0] = MASK1_B;
    toSend[1] = MASK1_B_P_X | MASK1_B_P_Y | MASK1_B_P_Z; //enable positive X, Y and Z in mask B
    spi.writeAndRead(toSend, write);
    
    toSend[0] = MASK1_A;
    toSend[1] = MASK1_A_P_X | MASK1_A_P_Y | MASK1_A_P_Z; //enable positive X, Y and Z in mask A
    spi.writeAndRead(toSend, write);
    
    toSend[0] = SETT1;
    toSend[1] = SETT1_SITR; //program flow can be modified by STOP and CONT commands
    toSend[1] |= SETT1_R_TAM; //valid condition found and RESET
    spi.writeAndRead(toSend, write);
    
    /* ========================== STATE MACHINE CONFIG =======================*/
    toSend[0] = ST1_1; //state machine 1, state 1
    
    /* set NOP (no-operation) in the RESET condition; 
     * also set the NEXT condition (how to go in the next state) : if LLTH2 (the acc 
     * of axis are less than or equal to threshold 2) */
    toSend[1] = 0x0A;
    spi.writeAndRead(toSend, write);
    toSend[0] = ST1_2; //state machine 1, state 2
    
    /* set the RESET condition : if GNTH2 (the acc of axis become greater than threshold 2) ;
     * also set the NEXT condition : TI1 (check if 100 ms passed) */
    toSend[1] = 0x61;
    spi.writeAndRead(toSend, write);
    
    toSend[0] = ST1_3; //state machine 1, state 3
    toSend[1] = 0x11; //set CONTt (the final state where the freefall condition is verified)
    spi.writeAndRead(toSend, write);
    
    toSend[0] = CTRL_REG1;
    toSend[1] = CTRL_REG1_SM1_EN ; //enabled State Machine 1
    spi.writeAndRead(toSend, write);
    /*====================== END OF STATE MACHINE CONFIG =====================*/
    
    uint8_t toRead = (CTRL_REG1);
    uint8_t *response = spi.writeAndRead(&toRead, read);
    uint8_t tempResponse = (uint8_t)*response;
    
    for(;;)
        blinkLeds();
}