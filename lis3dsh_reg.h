/* 
 * File:   lis3dsh_reg.h
 * Author: Andrea Cirigliano
 * Author: Michele Liscio
 *
 * Created on December 22, 2014, 2:20 PM
 */

#ifndef LIS3DSH_REG_H
#define LIS3DSH_REG_H

#define CTRL_REG4   0x20
#define CTRL_REG1   0x21
#define CTRL_REG2   0x22
#define CTRL_REG3   0x23
#define CTRL_REG5   0x24
#define CTRL_REG6   0x25
#define TIM1_1L     0x55
#define THRS2_1     0x56
#define MASK1_A     0x5A
#define MASK1_B     0x59
#define SETT1       0x5B
#define ST1_1       0x40
#define ST1_2       0x41
#define ST1_3       0x42

// bit mask of CTRL_REG1
#define CTRL_REG1_XEN   0x01
#define CTRL_REG1_YEN   0x02
#define CTRL_REG1_ZEN   0x04
#define CTRL_REG1_LPEN  0x08
#define CTRL_REG1_ODR0  0x10
#define CTRL_REG1_ODR1  0x20
#define CTRL_REG1_ODR2  0x40
#define CTRL_REG1_ODR3  0x80
#define REFERENCE (*((volatile uint8_t*)0x26));

struct INT_struct
{
    volatile uint8_t CFG;
    volatile uint8_t SRC;
    volatile uint8_t THS;
    volatile uint8_t DURATION;
};

#define INT ((INT_struct*)0x30);

#endif /* LIS3DSH_REG_H */

