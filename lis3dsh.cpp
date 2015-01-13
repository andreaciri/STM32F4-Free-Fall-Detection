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
#include "spi.h"
#include "lis3dsh_reg.h"
#include "lis3dsh.h"
#include "IRQhandler.h"

using namespace miosix;

typedef Gpio<GPIOE_BASE, 0> int1Signal;

/* SPI istance here defined because of integrability (someone interested in the
 freefall detection doesn't have to call the spi.config but only the accelerometer.config */
Spi spi;
IRQhandler irqHandler;

Lis3dsh::Lis3dsh() {
}

Lis3dsh::~Lis3dsh() {
}

/* ============================= LEDS UTILITY =============================== */
typedef Gpio<GPIOD_BASE, 15> blueLed;
typedef Gpio<GPIOD_BASE, 14> redLed;
typedef Gpio<GPIOD_BASE, 13> orangeLed;
typedef Gpio<GPIOD_BASE, 12> greenLed;

void Lis3dsh::blinkLeds() {
    blueLed::high(); redLed::high(); orangeLed::high(); greenLed::high();
    usleep(1000000);
    blueLed::low(); redLed::low(); orangeLed::low(); greenLed::low();
    usleep(1000000);
}
/* ============================ END LEDS UTILITY =============================*/

/**
 * @brief Configures the accelerometer for the free-fall detection.
 * @param minDuration : The minimum duration (in milliseconds) of subthreshold accelerations for recognize a free-fall condition.
 *                      Allowed range [2,5 - 637,5]ms.
 * @param threshold :   The maximum acceleration (in milli-g) that is recognized as free-fall condition.
 *                      The lower is the threshold, more accurate is the recognition.
 *                      Allowed range [15,625 - 3984]mg.
 */
void Lis3dsh::freeFallConfig(float minDuration, float threshold) {
    blueLed::mode(Mode::OUTPUT); redLed::mode(Mode::OUTPUT);
    orangeLed::mode(Mode::OUTPUT); greenLed::mode(Mode::OUTPUT);
    bool write = true;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN; //enable GPIOE clock
    int1Signal::mode(Mode::INPUT);

    spi.config();
    
    uint8_t timer1 = convertTime(minDuration);
    uint8_t threshold2 = convertThreshold(threshold);
    
    uint8_t toSend[2];

    toSend[ADDR] = CTRL_REG4;
    toSend[DATA] = CTRL_REG4_XEN | CTRL_REG4_YEN | CTRL_REG4_ZEN; //accelerometer axis enabled
    toSend[DATA] |= CTRL_REG4_ODR0 | CTRL_REG4_ODR1 | CTRL_REG4_ODR2; //output data rate at 400 Hz
    spi.writeAndRead(toSend, write);

    toSend[ADDR] = CTRL_REG3;
    toSend[DATA] = CTRL_REG3_INT1EN; //interrupt1 enabled, signals when freefall is detected
    toSend[DATA] |= CTRL_REG3_IEA; //interrupt signal active high
    spi.writeAndRead(toSend, write);

    toSend[ADDR] = TIM1_1L; //timer 1 for State Machine 1
    toSend[DATA] = timer1; //free fall duration
    spi.writeAndRead(toSend, write);

    toSend[ADDR] = THRS2_1; // threshold 2 for State Machine 1
    toSend[DATA] = threshold2; //free-fall threshold
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
    toSend[DATA] = (NOP RESET) | (LLTH2 NEXT);
    spi.writeAndRead(toSend, write);
    toSend[ADDR] = ST1_2; //state machine 1, state 2

    /* set the RESET condition : if GNTH2 (the acc of axis become greater than threshold 2) ;
     * also set the NEXT condition : TI1 (check if 100 ms passed) */
    toSend[DATA] = (GNTH2 RESET) | (TI1 NEXT);
    spi.writeAndRead(toSend, write);

    toSend[ADDR] = ST1_3; //state machine 1, state 3
    toSend[DATA] = CONT; //set CONT (the final state where the freefall condition is verified)
    spi.writeAndRead(toSend, write);

    toSend[ADDR] = CTRL_REG1;
    toSend[DATA] = CTRL_REG1_SM1_EN; //enabled State Machine 1
    spi.writeAndRead(toSend, write);
    /*====================== END OF STATE MACHINE CONFIG =====================*/

    irqHandler.configureAccInterrupt(); //configure interrupt 1 handler
    
}

/**
 * @brief the function that loops waiting for the freefall detection interrupt
 */
void Lis3dsh::freeFallDetectionInit() {
    uint8_t toReceive[1];
    bool read = false;
    for (;;) 
    {        
        irqHandler.waitForInt1();
        
        blinkLeds();
        
        toReceive[ADDR] = OUTS1; // OUTS1 register: reading this, the interrupt signal is reset 
        spi.writeAndRead(toReceive, read);
    } 
}

/**
 * @brief converts time from milliseconds to the corresponding byte
 * @param milliseconds : the freefall detection time interval expressed in milliseconds
 * @return byte : the time interval converted in byte
 */
uint8_t Lis3dsh::convertTime(float milliseconds){
    // 1 LSB = 1/ODR = 1/400 Hz
    float temp = milliseconds / (2.5);
    int byte = (int) temp;
    if (byte < 0)
        return 0;
    else if (byte > 255)
        return 255;
    else
        return (uint8_t) byte;
}

/**
 * @brief converts acceleration from milli-g to the corresponding byte
 * @param milliG : the threshold expressed in milliG
 * @return byte : the threshold converted in byte
 */
uint8_t Lis3dsh::convertThreshold(float milliG) {
    // 1 LSB = 2g/(2^7)
    float temp = milliG / (15.625);
    int byte = (int) temp;
    if (byte < 0)
        return 0;
    else if (byte > 255)
        return 255;
    else
        return (uint8_t) byte;
}