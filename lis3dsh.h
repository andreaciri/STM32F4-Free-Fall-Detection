/* 
 * File:   lis3dsh.h
 * Author: Michele Liscio
 * Author: Andrea Cirigliano
 *
 * Created on 10 gennaio 2015, 12.14
 */

#ifndef LIS3DSH_H
#define	LIS3DSH_H
#include <unistd.h>

class Lis3dsh {
public:
    Lis3dsh();
    virtual ~Lis3dsh();
    
    void freeFallConfig(float minDuration, float threshold);
    
private:
    void blinkLeds();
    uint8_t convertTime(float milliseconds);
    uint8_t convertThreshold(float milliG);
};

#endif	/* LIS3DSH_H */

