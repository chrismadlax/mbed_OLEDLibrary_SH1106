#ifndef PAR16_H
#define PAR16_H

#include "mbed.h"
#include "Protocols.h"
//#include "GraphicsDisplay.h"

class PAR16 : public Protocols
{
 public:

    /** Create a PAR16 display interface with a GPIO port and 5 control pins
    *
    * @param port GPIO port to use
    * @param CS pin connected to CS of display
    * @param reset pin connected to RESET of display
    * @param DC pin connected to data/command of display
    * @param WR pin connected to SDI of display
    * @param RD pin connected to RS of display 
    */ 
    PAR16(PortName port, PinName CS, PinName reset, PinName DC, PinName WR, PinName RD);

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
    * @note 2cycles using pins[7:0]
    */   
    virtual void wr_cmd16(unsigned short cmd);
    
    /** Send 2x8bit data to display controller 
    *
    * @param data: halfword to send   
    * @note 2cycles using pins[7:0], only gram write cmd uses pins[15:8]
    */   
    virtual void wr_data16(unsigned short data);
    
    /** Send 16bit pixeldata to display controller 
    *
    * @param data: halfword to send   
    * @note here using all pins[15:0]
    */   
    virtual void wr_gram(unsigned short data);
    
    /** Send same 16bit pixeldata to display controller multiple times
    *
    * @param data: halfword to send
    * @param count: how many
    * @note here using all pins[15:0]
    */   
    virtual void wr_gram(unsigned short data, unsigned int count);
    
    /** Send array of pixeldata shorts to display controller
    *
    * @param data: unsigned short pixeldata array
    * @param lenght: lenght (in shorts)
    * @note here using all pins[15:0]
    */   
    virtual void wr_grambuf(unsigned short* data, unsigned int lenght);
    
    /** HW reset sequence (without display init commands)   
    */
    virtual void hw_reset();
    
    /** Set ChipSelect high or low
    * @param enable 0/1   
    */
    virtual void BusEnable(bool enable);
  
   

private:

    PortInOut _port;
    DigitalOut _CS; 
    DigitalOut _reset;
    DigitalOut _DC;
    DigitalOut _WR;
    DigitalOut _RD;
  
};
#endif