/* 
 * File:   spi.h
 * Author: Andrea Cirigliano
 * Author: Michele Liscio
 * 
 * Created on December 19, 2014, 3:48 PM
 */

#ifndef SPI_H
#define	SPI_H

class Spi {
public:
    Spi();
    virtual ~Spi();
    void config();
    
private:
    void csOn();
    void csOff();
};

#endif	/* SPI_H */

