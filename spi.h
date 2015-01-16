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

const short ADDR = 0;
const short DATA = 1;

enum SpiDirection {READ, WRITE};

class Spi {
public:    
    void config();
    void writeAndRead(uint8_t *dataToSend, SpiDirection direction);
    bool spiBusy();
    
private:
    void csOn();
    void csOff();
};

#endif	/* SPI_H */

