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
    IRQhandler();
    virtual ~IRQhandler();
    
    void configureAccInterrupt();
    void waitForInt1();
private:
    void __attribute__((naked)) EXTI0_IRQHandler();
    void __attribute__((used)) EXTI0HandlerImpl();

};

#endif	/* IRQHANDLER_H */

