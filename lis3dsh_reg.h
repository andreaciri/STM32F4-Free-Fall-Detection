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
#define TIM1_1L     0x54
#define THRS2_1     0x56
#define MASK1_A     0x5A
#define MASK1_B     0x59
#define SETT1       0x5B
#define ST1_1       0x40
#define ST1_2       0x41
#define ST1_3       0x42

/* bitmask of CTRL_REG4 */
#define CTRL_REG4_XEN   0x01
#define CTRL_REG4_YEN   0x02
#define CTRL_REG4_ZEN   0x04
#define CTRL_REG4_BDU   0x08
#define CTRL_REG4_ODR0  0x10
#define CTRL_REG4_ODR1  0x20
#define CTRL_REG4_ODR2  0x40
#define CTRL_REG4_ODR3  0x80
#define REFERENCE (*((volatile uint8_t*)0x26));

/* bitmask of CTRL_REG1 */
#define CTRL_REG1_SM1_EN    0x01
#define CTRL_REG1_SM1_PIN   0x08
#define CTRL_REG1_HYST0_1   0x20
#define CTRL_REG1_HYST1_1   0x40
#define CTRL_REG1_HYST2_1   0x80

/* bitmask of CTRL_REG3 */
#define CTRL_REG3_STRT      0x01
#define CTRL_REG3_VFILT     0x04
#define CTRL_REG3_INT1EN    0x08
#define CTRL_REG3_INT2EN    0x10
#define CTRL_REG3_IEL       0x20
#define CTRL_REG3_IEA       0x40
#define CTRL_REG3_DR_EN     0x80

/* bitmask of MASK1_A */
#define MASK1_A_N_V         0x01
#define MASK1_A_P_V         0x02
#define MASK1_A_N_Z         0x04
#define MASK1_A_P_Z         0x08
#define MASK1_A_N_Y         0x10
#define MASK1_A_P_Y         0x20
#define MASK1_A_N_X         0x40
#define MASK1_A_P_X         0x80

/* bitmask of MASK1_B */
#define MASK1_B_N_V         0x01
#define MASK1_B_P_V         0x02
#define MASK1_B_N_Z         0x04
#define MASK1_B_P_Z         0x08
#define MASK1_B_N_Y         0x10
#define MASK1_B_P_Y         0x20
#define MASK1_B_N_X         0x40
#define MASK1_B_P_X         0x80

/* bitmask of SETT1 */
#define SETT1_SITR          0x01
#define SETT1_R_TAM         0x02
#define SETT1_THR3_MA       0x04
#define SETT1_ABS           0x20
#define SETT1_THR3_SA       0x40
#define SETT1_P_DET         0x80

/* bit mask for state machine operation codes */
#define NEXT                <<0
#define RESET               <<4
#define NOP                 0x0
#define LLTH2               0xA
#define GNTH2               0x6
#define TI1                 0x1
#define CONT                0x11

struct INT_struct
{
    volatile uint8_t CFG;
    volatile uint8_t SRC;
    volatile uint8_t THS;
    volatile uint8_t DURATION;
};

#define INT ((INT_struct*)0x30);

#endif /* LIS3DSH_REG_H */

