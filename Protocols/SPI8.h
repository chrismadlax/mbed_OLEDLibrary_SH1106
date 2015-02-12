#ifndef SPI8_H
#define SPI8_H

#include "mbed.h"
#include "Protocols.h"
//#include "GraphicsDisplay.h"

class SPI8 : public Protocols
{
 public:

    /** Create an SPI 8bit display interface with 3 control pins
    *
    * @param SPI mosi
    * @param SPI miso
    * @param SPI sclk
    * @param CS pin connected to CS of display
    * @param reset pin connected to RESET of display
    * @param DC pin connected to data/command of display
    */ 
    SPI8(PinName mosi, PinName miso, PinName sclk, PinName CS, PinName reset, PinName DC);
 
protected:
   
    /** Send command byte to display controller 
    *
    * @param cmd: byte to send  
    *
    */   
    virtual void wr_cmd(unsigned char cmd);
    
    /** Send data byte to display controller 
    *
    * @param data8: byte to send   
    *
    */   
    virtual void wr_data8(unsigned char data8);
    
    /** Send same data byte to display controller multiple times
    *
    * @param data8: byte to send
    * @param count: how many
    *
    */   
    virtual void wr_data8(unsigned char data8, unsigned int count);
    
    /** Send array of data bytes to display controller
    *
    * @param data8: unsigned char data array
    * @param lenght: lenght of array
    *
    */   
    virtual void wr_data8buf(unsigned char* data8, unsigned int lenght);
    
    /** HW reset sequence (without display init commands)   
    */
    virtual void hw_reset();
    
    /** Set ChipSelect high or low
    * @param enable 0/1   
    */
    virtual void BusEnable(bool enable);
  
    DigitalOut _CS;

private:

    SPI _spi;
    DigitalOut _reset;
    DigitalOut _DC;
  
};
#endif