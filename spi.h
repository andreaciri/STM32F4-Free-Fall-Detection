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

enum SpiDirection {READ, WRITE};

class Spi {
public:
    Spi();
    virtual ~Spi();
    
    void config();
    void writeAndRead(uint8_t *dataToSend, SpiDirection direction);
    bool spiBusy();
    
private:
    void csOn();
    void csOff();
};

#endif	/* SPI_H */

