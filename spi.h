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
    uint8_t writeAndRead(uint8_t *dataToSend, int lenght);
    
private:
    void csOn();
    void csOff();
};

#endif	/* SPI_H */

