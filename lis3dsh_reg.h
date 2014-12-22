/* 
 * File:   accelerometer.h
 * Author: es1
 *
 * Created on December 22, 2014, 2:20 PM
 */

#ifndef ACCELEROMETER_H
#define	ACCELEROMETER_H

typedef struct CTRL_struct
{
    volatile uint8_t REG1;
    volatile uint8_t REG2;
    volatile uint8_t REG3;
    volatile uint8_t REG4;
    volatile uint8_t REG5;
    volatile uint8_t REG6;
};

#define CTRL ((CTRL_struct*)0x20);
// bit mask of CTRL_REG1
#define CTRL_REG1_XEN 0x01
#define CTRL_REG1_YEN 0x02
#define CTRL_REG1_ZEN 0x04
#define CTRL_REG1_LPEN 0x08
#define CTRL_REG1_ODR0 0x10
#define CTRL_REG1_ODR1 0x20
#define CTRL_REG1_ODR2 0x40
#define CTRL_REG1_ODR3 0x80
#define REFERENCE (*((volatile uint8_t*)0x26));

typedef struct INT_struct
{
    volatile uint8_t CFG;
    volatile uint8_t SRC;
    volatile uint8_t THS;
    volatile uint8_t DURATION;
};

#define INT ((INT_struct*)0x30);

#endif	/* ACCELEROMETER_H */

