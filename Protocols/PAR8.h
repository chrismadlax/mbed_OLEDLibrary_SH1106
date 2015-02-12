#ifndef PAR8_H
#define PAR8_H

#include "mbed.h"
#include "Protocols.h"
//#include "GraphicsDisplay.h"

class PAR8 : public Protocols
{
 public:

    /** Create a PAR8 display interface with a GPIO port and 5 control pins
    *
    * @param port GPIO port to use
    * @param CS pin connected to CS of display
    * @param reset pin connected to RESET of display
    * @param DC pin connected to data/command of display
    * @param WR pin connected to SDI of display
    * @param RD pin connected to RS of display 
    */ 
    PAR8(PortName port, PinName CS, PinName reset, PinName DC, PinName WR, PinName RD);

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
  
   

private:

    PortInOut _port;
    DigitalOut _CS; 
    DigitalOut _reset;
    DigitalOut _DC;
    DigitalOut _WR;
    DigitalOut _RD;
  
};
#endif