/* 
 * File:   IRQhandler.cpp
 * Author: Michele Liscio
 * Author: Andrea Cirigliano
 * 
 * Created on January 12, 2015, 2:38 PM
 */

#include "IRQhandler.h"
#include <miosix.h>
#include <miosix/kernel/scheduler/scheduler.h>

using namespace miosix;

typedef Gpio<GPIOE_BASE, 0> int1Signal;

static Thread *waiting=0;

void __attribute__((naked)) EXTI0_IRQHandler()
{
    saveContext();
    asm volatile("bl _Z16EXTI0HandlerImplv");
    restoreContext();
}

void __attribute__((used)) EXTI0HandlerImpl()
{
    /* is set by HW when the selected edge event arrives on the external interrupt line. It's cleared writing 1 */
    EXTI->PR=EXTI_PR_PR0;
    
    if(waiting==0) return;
    waiting->IRQwakeup();
    if(waiting->IRQgetPriority()>Thread::IRQgetCurrentThread()->IRQgetPriority())
        Scheduler::IRQfindNextThread();
    waiting=0;
}

/**
 *  @brief configures the interrupt in line 0 for the accelerometer;
 */
void IRQhandler::configureAccInterrupt()
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN; //enable GPIOE clock
    int1Signal::mode(Mode::INPUT); //create interrupt on rising edge of the accelerometer interrupt
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; // system configuration controller clock enable
    SYSCFG->EXTICR[0] = SYSCFG_EXTICR1_EXTI0_PE; //use PE0 for EXTI_Line0
    EXTI->IMR |= EXTI_IMR_MR0; //interrupt request from line 0 is not masked
    EXTI->RTSR |= EXTI_RTSR_TR0; //rising trigger enabled
    
    /* enables a device-specific interrupt in the NVIC interrupt controller; 
    EXTI0_IRQn = 6 : EXTI Line0 Interrupt = STM32 specific Interrupt Number */
    NVIC_EnableIRQ(EXTI0_IRQn);
    NVIC_SetPriority(EXTI0_IRQn,15); //low priority
}

/**
 *  @brief puts the current thread in wait condition until the interrupt is received;
 */
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