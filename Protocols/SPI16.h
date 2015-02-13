#ifndef SPI16_H
#define SPI16_H

#include "mbed.h"
#include "Protocols.h"
//#include "GraphicsDisplay.h"

class SPI16 : public Protocols
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
    SPI16(int Hz, PinName mosi, PinName miso, PinName sclk, PinName CS, PinName reset, PinName DC);
 
protected:
   
    /** Send 8bit command to display controller 
    *
    * @note switches spi format 16->8->16, better use wr_cmd16 with NOP in front 
    * @param cmd: byte to send  
    *
    */   
    virtual void wr_cmd8(unsigned char cmd);
    
    /** Send 8bit data to display controller 
    *
    * @note switches spi format 16->8->16, better use wr_data16 with repeated byte (if does not hurt)
    * @param data: byte to send   
    *
    */   
    virtual void wr_data8(unsigned char data);
    
    /** Send same 8bit data to display controller multiple times
    *
    * @note switches spi format 16->8->16, better use wr_data16
    * @param data: byte to send
    * @param count: how many
    *
    */   
    virtual void wr_data8(unsigned char data, unsigned int count);
    
    /** Send array of data bytes to display controller
    *
    * @note switches spi format 16->8->16, better use wr_data16
    * @param data: unsigned char data array
    * @param lenght: lenght of array
    *
    */   
    virtual void wr_data8buf(unsigned char* data, unsigned int lenght);
    
    /** Send 16bit command to display controller 
    *
    * @param cmd: halfword to send  
    *
    */   
    virtual void wr_cmd16(unsigned short cmd);
    
    /** Send 16bit data to display controller 
    *
    * @param data: halfword to send   
    *
    */   
    virtual void wr_data16(unsigned short data);
    
    /** Send same 16bit data to display controller multiple times
    *
    * @param data: halfword to send
    * @param count: how many
    *
    */   
    virtual void wr_data16(unsigned short data, unsigned int count);
    
    /** Send array of data shorts to display controller
    *
    * @param data: unsigned short data array
    * @param lenght: lenght (in shorts)
    *
    */   
    virtual void wr_data16buf(unsigned short* data, unsigned int lenght);
    
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