#ifndef MBED_ILI9341_H
#define MBED_ILI9341_H



#include "mbed.h"
#include "TFT.h"

/** Class for ILI9341 tft display controller
* to be copypasted and adapted for other controllers
*/
class ILI9341 : public TFT
{
 
 public:

    /** Create a PAR display interface
    * @param displayproto only supports PAR_8
    * @param port GPIO port name to use
    * @param CS pin connected to CS of display
    * @param reset pin connected to RESET of display
    * @param DC pin connected to data/command of display
    * @param WR pin connected to SDI of display
    * @param RD pin connected to RS of display
    * @param name The name used by the parent class to access the interface
    */ 
    ILI9341(proto_t displayproto, PortName port, PinName CS, PinName reset, PinName DC, PinName WR, PinName RD, const char* name);
  
    /** Create an SPI display interface
    * @param displayproto only supports SPI_8
    * @param Hz SPI speed in Hz
    * @param mosi SPI pin
    * @param miso SPI pin
    * @param sclk SPI pin
    * @param CS pin connected to CS of display
    * @param reset pin connected to RESET of display
    * @param DC pin connected to data/command of display
    * @param name The name used by the parent class to access the interface
    */ 
    ILI9341(proto_t displayproto, int Hz, PinName mosi, PinName miso, PinName sclk, PinName CS, PinName reset, PinName DC, const char* name);
  

  
protected:
    
    
    /** Init command sequence  
    */
    void init();



};
#endif