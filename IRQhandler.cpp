/* 
 * File:   IRQhandler.cpp
 * Author: es1
 * 
 * Created on January 12, 2015, 2:38 PM
 */

#include "IRQhandler.h"
#include <miosix.h>
#include <miosix/kernel/scheduler/scheduler.h>

using namespace miosix;

typedef Gpio<GPIOE_BASE, 0> int1Signal;

static Thread *waiting=0;

IRQhandler::IRQhandler() {
}


IRQhandler::~IRQhandler() {
}

void __attribute__((naked)) EXTI0_IRQHandler()
{
    saveContext();
    asm volatile("bl _Z16EXTI0HandlerImplv");
    restoreContext();
}

void __attribute__((used)) EXTI0HandlerImpl()
{
    EXTI->PR=EXTI_PR_PR0;
    
    if(waiting==0) return;
    waiting->IRQwakeup();
    if(waiting->IRQgetPriority()>Thread::IRQgetCurrentThread()->IRQgetPriority())
		Scheduler::IRQfindNextThread();
    waiting=0;
}

void IRQhandler::configureAccInterrupt()
{
	//create interrupt on rising edge of the button bit (when the button is pressed)
    int1Signal::mode(Mode::INPUT);
    EXTI->IMR |= EXTI_IMR_MR0;
    EXTI->RTSR |= EXTI_RTSR_TR0;
    NVIC_EnableIRQ(EXTI0_IRQn);
    NVIC_SetPriority(EXTI0_IRQn,15);  
}

void IRQhandler::waitForInt1()
{
    FastInterruptDisableLock dLock;
    waiting=Thread::IRQgetCurrentThread();
    while(waiting)
    {
        Thread::IRQwait();
        FastInterruptEnableLock eLock(dLock);
        Thread::yield();
    }
}