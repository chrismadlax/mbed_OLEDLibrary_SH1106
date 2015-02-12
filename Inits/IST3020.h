#ifndef MBED_IST3020_H
#define MBED_IST3020_H



#include "mbed.h"
#include "LCD.h"

/** Class for IST3020 display controller
* to be copypasted and adapted for other controllers
*/
class IST3020 : public LCD
{
 
 public:

    /** Create a PAR_8 display interface
    * @param displayproto PAR_8 or PAR_16
    * @param port GPIO port name to use
    * @param CS pin connected to CS of display
    * @param reset pin connected to RESET of display
    * @param DC pin connected to data/command of display
    * @param WR pin connected to SDI of display
    * @param RD pin connected to RS of display
    * @param name The name used by the parent class to access the interface
    */ 
    IST3020(proto_t displayproto, PortName port, PinName CS, PinName reset, PinName DC, PinName WR, PinName RD, const char* name);
  
    /** Create an SPI_8 display interface
    * @param displayproto SPI_8 or SPI_16
    * @param mosi SPI pin
    * @param miso SPI pin
    * @param sclk SPI pin
    * @param CS pin connected to CS of display
    * @param reset pin connected to RESET of display
    * @param DC pin connected to data/command of display
    * @param name The name used by the parent class to access the interface
    */ 
    IST3020(proto_t displayproto, PinName mosi, PinName miso, PinName sclk, PinName CS, PinName reset, PinName DC, const char* name);
  

  
protected:
    
    
    /** Init command sequence  
    */
    void init();



};
#endif