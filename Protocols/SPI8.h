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
    SPI8(int Hz, PinName mosi, PinName miso, PinName sclk, PinName CS, PinName reset, PinName DC);
 
protected:
   
    /** Send 8bit command to display controller 
    *
    * @param cmd: byte to send  
    *
    */   
    virtual void wr_cmd8(unsigned char cmd);
    
    /** Send 8bit data to display controller 
    *
    * @param data: byte to send   
    *
    */   
    virtual void wr_data8(unsigned char data);
    
    /** Send 2x8bit command to display controller 
    *
    * @param cmd: halfword to send  
    */   
    virtual void wr_cmd16(unsigned short cmd);
    
    /** Send 2x8bit data to display controller 
    *
    * @param data: halfword to send   
    *
    */   
    virtual void wr_data16(unsigned short data);
    
    /** Send 16bit pixeldata to display controller 
    *
    * @param data: halfword to send   
    *
    */   
    virtual void wr_gram(unsigned short data);
    
    /** Send same 16bit pixeldata to display controller multiple times
    *
    * @param data: halfword to send
    * @param count: how many
    *
    */   
    virtual void wr_gram(unsigned short data, unsigned int count);
    
    /** Send array of pixeldata shorts to display controller
    *
    * @param data: unsigned short pixeldata array
    * @param lenght: lenght (in shorts)
    *
    */   
    virtual void wr_grambuf(unsigned short* data, unsigned int lenght);
    
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