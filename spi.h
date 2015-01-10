/* 
 * File:   spi.h
 * Author: Andrea Cirigliano
 * Author: Michele Liscio
 * 
 * Created on December 19, 2014, 3:48 PM
 */

#ifndef SPI_H
#define	SPI_H
#define SPI_READ 1<<7
#define ADDR 0
#define DATA 1

class Spi {
public:
    Spi();
    virtual ~Spi();
    
    void config();
    uint8_t *writeAndRead(uint8_t *dataToSend, bool write);
    bool spiBusy();
    
private:
    void csOn();
    void csOff();
};

#endif	/* SPI_H */

