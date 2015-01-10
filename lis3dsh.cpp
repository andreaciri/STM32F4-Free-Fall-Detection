/* 
 * File:   lis3dsh.cpp
 * Author: Michele Liscio
 * Author: Andrea Cirigliano
 * 
 * brief : this class configures the accelerometer for the specified purpose; in the
 *      case of our project, it's the freefall purpose.
 * 
 * Created on 10 gennaio 2015, 12.14
 */

#include <miosix.h>
#include <unistd.h>
#include <string>
#include <cstdio>
#include "spi.h"
#include "lis3dsh_reg.h"
#include "lis3dsh.h"

using namespace miosix;

/* SPI istance here defined because of integrability (someone interested in the
 freefall detection doesn't have to call the spi.config but only the accelerometer.config */
Spi spi;
    
Lis3dsh::Lis3dsh() {
}

Lis3dsh::~Lis3dsh() {
}

/* ============================= LEDS UTILITY =============================== */
typedef Gpio<GPIOD_BASE,15> blueLed;
typedef Gpio<GPIOD_BASE,14> redLed;
typedef Gpio<GPIOD_BASE,13> orangeLed;
typedef Gpio<GPIOD_BASE,12> greenLed;

void Lis3dsh::blinkLeds()
{
    blueLed::mode(Mode::OUTPUT); redLed::mode(Mode::OUTPUT);
    orangeLed::mode(Mode::OUTPUT); greenLed::mode(Mode::OUTPUT);
    
    for(;;)
    {
        blueLed::high(); redLed::high(); orangeLed::high(); greenLed::high();
        usleep(1000000);
        blueLed::low(); redLed::low(); orangeLed::low(); greenLed::low();
        usleep(1000000);
    }
}
/* ============================ END LEDS UTILITY =============================*/

/**
 * \brief configures the accelerometer for the specified purpose.
 * @param purpose : the purpose for which the accelerometer has to be utilized;
 *      in our project, we consider only the freefall purpose.
 */
void Lis3dsh::config(const char* purpose)
{
    bool write = true, read = false;
    
    spi.config();
    
    if (strcmp(purpose, "freefall")==0)
    {
        uint8_t toSend[2];
        uint8_t toRead[1];
        
        toSend[ADDR] = CTRL_REG4;
        toSend[DATA] = CTRL_REG4_XEN | CTRL_REG4_YEN | CTRL_REG4_ZEN ; //accelerometer axis enabled
        toSend[DATA] |= CTRL_REG4_ODR0 | CTRL_REG4_ODR1 | CTRL_REG4_ODR2 ; //output data rate at 400 Hz
        spi.writeAndRead(toSend, write);

        toSend[ADDR] = CTRL_REG3;
        toSend[DATA] = CTRL_REG3_INT1EN ; //interrupt1 enabled, signals when freefall is detected
        toSend[DATA] |= CTRL_REG3_IEA ; //interrupt signal active high
        spi.writeAndRead(toSend, write);

        toSend[ADDR] = TIM1_1L; //timer 1
        toSend[DATA] = 0x28 ; //free fall duration 100 ms
        spi.writeAndRead(toSend, write);

        toSend[ADDR] = THRS2_1;
        toSend[DATA] = 0x18 ; //free-fall threshold 375 mg (threshold 2)
        spi.writeAndRead(toSend, write);

        toSend[ADDR] = MASK1_B;
        toSend[DATA] = MASK1_B_P_X | MASK1_B_P_Y | MASK1_B_P_Z; //enable positive X, Y and Z in mask B
        spi.writeAndRead(toSend, write);

        toSend[ADDR] = MASK1_A;
        toSend[DATA] = MASK1_A_P_X | MASK1_A_P_Y | MASK1_A_P_Z; //enable positive X, Y and Z in mask A
        spi.writeAndRead(toSend, write);

        toSend[ADDR] = SETT1;
        toSend[DATA] = SETT1_SITR; //STOP and CONT commands generate an interrupt and perform output actions as OUTC command
        toSend[DATA] |= SETT1_R_TAM; //NEXT condition validation : standard mask always evaluated 
        spi.writeAndRead(toSend, write);

        /* ========================== STATE MACHINE CONFIG =======================*/
        toSend[ADDR] = ST1_1; //state machine 1, state 1

        /* set NOP (no-operation) in the RESET condition; 
         * also set the NEXT condition (how to go in the next state) : if LLTH2 (the acc 
         * of axis are less than or equal to threshold 2) */
        toSend[DATA] = 0x0A;
        spi.writeAndRead(toSend, write);
        toSend[ADDR] = ST1_2; //state machine 1, state 2

        /* set the RESET condition : if GNTH2 (the acc of axis become greater than threshold 2) ;
         * also set the NEXT condition : TI1 (check if 100 ms passed) */
        toSend[DATA] = 0x61;
        spi.writeAndRead(toSend, write);

        toSend[ADDR] = ST1_3; //state machine 1, state 3
        toSend[DATA] = 0x11; //set CONTt (the final state where the freefall condition is verified)
        spi.writeAndRead(toSend, write);

        toSend[ADDR] = CTRL_REG1;
        toSend[DATA] = CTRL_REG1_SM1_EN ; //enabled State Machine 1
        spi.writeAndRead(toSend, write);
        /*====================== END OF STATE MACHINE CONFIG =====================*/
        
        toRead[ADDR] = (CTRL_REG1);
        uint8_t *response = spi.writeAndRead(toRead, read);
        uint8_t tempResponse = (uint8_t)*response;
        
        for(;;)
        {
            toRead[ADDR] = (0x18);
            do
            {
                response = spi.writeAndRead(toRead, read);
                tempResponse = (uint8_t)*response;
            } while ((tempResponse & (1<<3)) ==0);
            blinkLeds();
        }
    }
}