/* 
 * File:   spi.cpp
 * Author: Andrea Cirigliano
 * Author: Michele Liscio
 * 
 * Created on December 19, 2014, 3:48 PM
 */

#include <miosix.h>
#include <unistd.h>
#include <cstdio>
#include <spi.h>

using namespace miosix;

typedef Gpio<GPIOA_BASE,5> SCK;
typedef Gpio<GPIOA_BASE,6> MISO;
typedef Gpio<GPIOA_BASE,7> MOSI;
typedef Gpio<GPIOE_BASE,3> CS;

/**
 * \brief class constructor
 */
Spi::Spi() {
}

/**
 * \brief class distructor
 */
Spi::~Spi() {
}


/* CS ready */
void Spi::csOn(){
    CS::low();
}

/* CS not ready */
void Spi::csOff(){
    CS::high();
}

/**
 *  \brief Sets up all the registers to enable the SPI1 interface;
 *  @param void
 *  @return void
 */
void Spi::config(){
    
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN ; //pheripheral clock enabled
    RCC->AHB1ENR |=RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOEEN; //enable clock on GPIO that will be used
    
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
    
        /* reset the SPI registers */
    RCC->APB2RSTR |= RCC_APB2ENR_SPI1EN;
    RCC->APB2RSTR &= !(RCC_APB2ENR_SPI1EN);
    
    /* SPI protocol configuration */
    SPI1->CR1 |= SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2; //set the lowest baud rate (328kHz)
    SPI1->CR1 &= ~SPI_CR1_CPHA ; //sampling on clock rising edge
    SPI1->CR1 &= ~SPI_CR1_CPOL ; //idle clock low
    SPI1->CR1 &= ~SPI_CR1_DFF ; //8-bit frame
    SPI1->CR1 &= ~SPI_CR1_LSBFIRST ; //msb trasmission
    SPI1->CR1 |= SPI_CR1_MSTR ; //master config
    SPI1->CR1 |= SPI_CR1_SPE ; //spi enabled
    
    //SPI1->CR2 &= ~SPI_CR2_FRF; //set motorola mode instead of 'ti' mode
    //SPIg->I2SCFGR &= (uint16_t)!((uint16_t)SPI_I2SCFGR_I2SMOD); //Activate the SPI mode (Reset I2SMOD bit in I2SCFGR register)
    

}

uint8_t* Spi::writeAndRead(uint8_t *dataToSend, int lenght){
    while(Spi::spiBusy());
    while((SPI1->SR & SPI_SR_TXE) == 0);
    csOn();
    uint16_t temp = *dataToSend;
    SPI1->DR = temp;
    while((SPI1->SR & SPI_SR_TXE) == 0);
    if(lenght>1)
    {
        SPI1->DR = *(++dataToSend);
    }
    while((SPI1->SR & SPI_SR_RXNE) == 0);
    
    uint8_t *receivedData = (uint8_t*) malloc(sizeof(uint8_t));
    *receivedData = SPI1->DR;
    csOff();
    while(Spi::spiBusy());
    return receivedData;
    
    
}

bool Spi::spiBusy()
{
    return (SPI1->SR & SPI_SR_BSY);
}