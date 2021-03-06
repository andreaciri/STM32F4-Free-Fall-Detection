/* 
 * File:   spi.cpp
 * Author: Andrea Cirigliano
 * Author: Michele Liscio
 * 
 * @brief : this class configures the spi.
 * 
 * Created on December 19, 2014, 3:48 PM
 */

#include <miosix.h>
#include "spi.h"

using namespace miosix;

typedef Gpio<GPIOA_BASE,5> SCK;
typedef Gpio<GPIOA_BASE,6> MISO;
typedef Gpio<GPIOA_BASE,7> MOSI;
typedef Gpio<GPIOE_BASE,3> CS;

/* CS ready */
void Spi::csOn(){
    CS::low();
    usleep(1);
}

/* CS not ready */
void Spi::csOff(){
    CS::high();
    usleep(1);
}

/**
 *  @brief Sets up all the registers to enable the SPI1 interface;
 *  @param void
 *  @return void
 */
void Spi::config(){
    
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN ; //pheripheral clock enabled
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOEEN; //enable clock on GPIO that will be used
    
    /* ======================= PIN CONFIGURATION ============================ */
    SCK::mode(Mode::ALTERNATE) ;
    SCK::alternateFunction(5) ;
    
    MISO::mode(Mode::ALTERNATE) ;
    MISO::alternateFunction(5) ;
    
    MOSI::mode(Mode::ALTERNATE) ;
    MOSI::alternateFunction(5) ;
    
    CS::mode(Mode::OUTPUT) ;
    csOff();
    /* ======================= END PIN CONFIGURATION =========================*/
    
    /* reset the SPI registers */
    RCC->APB2RSTR |= RCC_APB2RSTR_SPI1RST; 
    RCC->APB2RSTR &= !(RCC_APB2RSTR_SPI1RST);
    
    /* SPI protocol configuration */
    SPI1->CR1 |= SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2; //set the lowest baud rate (328kHz)
    SPI1->CR1 &= ~SPI_CR1_CPHA ; //sampling on clock rising edge
    SPI1->CR1 &= ~SPI_CR1_CPOL ; //idle clock low
    SPI1->CR1 &= ~SPI_CR1_DFF ; //8-bit frame
    SPI1->CR1 &= ~SPI_CR1_LSBFIRST ; //msb trasmission
    SPI1->CR1 |= SPI_CR1_SSM ; //software CS management
    SPI1->CR1 |= SPI_CR1_SSI ; //avoid master mode fault
    SPI1->CR2 &= ~SPI_CR2_FRF ; //select the motorola protocol instead of TI
    SPI1->CR1 |= SPI_CR1_MSTR ; //master config
    SPI1->CR1 |= SPI_CR1_SPE ; //spi enabled
}

/**
 *  @brief performs a write or a read depending on the boolean parameter
 *  @param dataToSend : dataToSend[0] contains the address where to write or where to read,
 *                      dataToSend[1] contains the data to write or it will be used to return the byte in case of read.
 *  @return void
 */
void Spi::writeAndRead(uint8_t *dataToSend, SpiDirection direction){
    while(SPI1->SR & SPI_SR_BSY){};
    
    while((SPI1->SR & SPI_SR_TXE) == 0){};
    
    csOn(); //start transmission
    uint8_t tempDR = dataToSend[ADDR];
    if(direction == READ) // it's a read
    {
        tempDR |= SPI_READ;
    }
    SPI1->DR = tempDR;

    while((SPI1->SR & SPI_SR_RXNE) == 0){};
    
    uint8_t dummyRead;
    dummyRead = SPI1->DR; //dummy read
    
    while((SPI1->SR & SPI_SR_TXE) == 0){};
    
    if(direction == WRITE) //it is a write
    {
        SPI1->DR = dataToSend[DATA];
    }
    else //it is a read
    {
        SPI1->DR = (uint8_t) 0x0000; //dummy write
    }
    
    while((SPI1->SR & SPI_SR_RXNE) == 0){};
    
    dataToSend[DATA] = SPI1->DR;
    
    
    while ((SPI1->SR & SPI_SR_TXE) == 0){};
    
    while(SPI1->SR & SPI_SR_BSY){};
    
    csOff(); //end transmission
    
    return;
}