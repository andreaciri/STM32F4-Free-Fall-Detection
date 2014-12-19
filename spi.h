/* 
 * File:   spi.h
 * Author: es1
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

