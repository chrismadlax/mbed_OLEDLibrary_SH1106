#ifndef MBED_ST7920_H
#define MBED_ST7920_H

#include "mbed.h"
#include "LCD.h"

/** Class for ST7920 and similar display controllers
* for Controller-details see http://www.alldatasheet.com/datasheet-pdf/pdf/326219/SITRONIX/ST7920.html
* to be copypasted and adapted for other controllers
*/
class ST7920 : public LCD
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
    * @param LCDSIZE_X x size in pixel 
    * @param LCDSIZE_Y y size in pixel 
    */
    ST7920(proto_t displayproto, PortName port, PinName CS, PinName reset, PinName DC, PinName WR, PinName RD, const char* name, unsigned int LCDSIZE_X = 132, unsigned  int LCDSIZE_Y = 64);

    /** Create a BUS display interface
    * @param displayproto only supports BUS_8
    * @param buspins array of PinName to group as Bus
    * @param CS pin connected to CS of display
    * @param reset pin connected to RESET of display
    * @param DC pin connected to data/command of display
    * @param WR pin connected to SDI of display
    * @param RD pin connected to RS of display
    * @param name The name used by the parent class to access the interface
    * @param LCDSIZE_X x size in pixel 
    * @param LCDSIZE_Y y size in pixel 
    */
    ST7920(proto_t displayproto, PinName* buspins, PinName CS, PinName reset, PinName DC, PinName WR, PinName RD, const char* name, unsigned int LCDSIZE_X = 132, unsigned  int LCDSIZE_Y = 64);

    /** Create an SPI display interface
    * @param displayproto SPI_8 or SPI_16
    * @param Hz SPI speed in Hz
    * @param mosi SPI pin
    * @param miso SPI pin
    * @param sclk SPI pin
    * @param CS pin connected to CS of display
    * @param reset pin connected to RESET of display
    * @param DC pin connected to data/command of display
    * @param name The name used by the parent class to access the interface
    * @param LCDSIZE_X x size in pixel 
    * @param LCDSIZE_Y y size in pixel 
    */
    ST7920(proto_t displayproto, int Hz, PinName mosi, PinName miso, PinName sclk, PinName CS, PinName reset, PinName DC, const char* name, unsigned int LCDSIZE_X = 132, unsigned  int LCDSIZE_Y = 64);



protected:

    //WriteInstructionRegister IR
    void wir(unsigned char data);

    //WriteDataRegister DR
    void wdr(unsigned char data);

    /** Init command sequence
    */
    void init();

public:
    /** Mirror
    // not needed, not supported
    */
    virtual void mirrorXY(mirror_t mode);

    // Clear Screen
    virtual void cls(void);

    //Copy screenbuffer to LCD
    virtual void copy_to_lcd(void);
    
    //not supported by hardware
    virtual void set_contrast(int o);
    
    //not supported by hardware
    void invert(unsigned char o);
    
    //not supported by hardware
    virtual void wr_grambuf(unsigned short* data, unsigned int lenght);
    
    //not supported by hardware
    virtual void wr_gram(unsigned short data);
    
    //not supported by hardware
    virtual void wr_gram(unsigned short data, unsigned int count);
    
    //not supported by hardware
    virtual void wr_data16(unsigned short data);
    
    //not supported by hardware
    virtual void wr_cmd16(unsigned short cmd);
    
    //not supported by hardware
    virtual void wr_cmd8(unsigned char cmd);
    
    //not supported by hardware
    virtual unsigned short rd_gram(bool convert);
    
    //not supported by hardware
    virtual unsigned int rd_reg_data32(unsigned char reg);
    
    //not supported by hardware
    virtual unsigned int rd_extcreg_data32(unsigned char reg, unsigned char SPIreadenablecmd);

};
#endif