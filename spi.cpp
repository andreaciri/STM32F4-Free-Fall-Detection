/* 
 * File:   spi.cpp
 * Author: andrea_cirigliano
 * 
 * Created on December 19, 2014, 3:48 PM
 */

#include <spi.h>
#include <miosix/miosix.h>
#include <cstdio>

typedef Gpio<GPIOA_BASE,5> SCK;
typedef Gpio<GPIOA_BASE,6> MISO;
typedef Gpio<GPIOA_BASE,7> MOSI;
typedef Gpio<GPIOE_BASE,3> CS;

Spi::Spi() {
}


Spi::~Spi() {
}

void Spi::config(){
    
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN ; //pheripheral clock enabled
    
    /* pin configuration */
    SCK::mode(Mode::ALTERNATE) ;
    SCK::alternateFunction(5) ;
    //SCK::speed(Speed::_50Mhz) ;
    
    MISO::mode(Mode::ALTERNATE) ;
    MISO::alternateFunction(5) ;
    //SCK::speed(Speed::_50Mhz) ;
    
    MOSI::mode(Mode::ALTERNATE) ;
    MOSI::alternateFunction(5) ;
    //SCK::speed(Speed::_50Mhz) ;
    
    CS::mode(Mode::OUTPUT) ;
    csOff();
    //SCK::speed(Speed::_50Mhz) ;
    
    /* SPI protocol configuration */
    SPI1->CR1 &= ~SPI_CR1_CPHA ; //sampling on clock rising edge
    SPI1->CR1 &= ~SPI_CR1_CPOL ; //idle clock low
    SPI1->CR1 &= ~SPI_CR1_DFF ; //8-bit frame
    SPI1->CR1 &= ~SPI_CR1_LSBFIRST ; //msb trasmission
    SPI1->CR1 |= SPI_CR1_MSTR ; //master config
    SPI1->CR1 |= SPI_CR1_SPE ; //spi enabled
    
    /* reset the SPI registers */
    RCC->APB2RSTR |= RCC_APB2ENR_SPI1EN;
    RCC->APB2RSTR &= !(RCC_APB2ENR_SPI1EN);
}

/* CS ready */
void Spi::csOn(){
    CS::low();
}

/* CS not ready */
void Spi::csOff(){
    CS::high();
}