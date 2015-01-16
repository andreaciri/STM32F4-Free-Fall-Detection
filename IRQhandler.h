/* 
 * File:   IRQhandler.h
 * Author: Michele Liscio
 * Author : Andrea Cirigliano
 *
 * Created on January 12, 2015, 2:38 PM
 */

#ifndef IRQHANDLER_H
#define	IRQHANDLER_H

class IRQhandler {
public:    
    void configureAccInterrupt();
    void waitForInt1();
};

#endif	/* IRQHANDLER_H */

