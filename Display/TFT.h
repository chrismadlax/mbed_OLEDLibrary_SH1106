#ifndef MBED_TFT_H
#define MBED_TFT_H

#include "GraphicsDisplay.h"
#include "PAR8.h"
#include "PAR16.h"
#include "SPI8.h"
#include "SPI16.h"
#include "Protocols.h"






/** A common base class for monochrome Display
*/
class TFT : public GraphicsDisplay
{

public:         
          
    /** Create a monochrome LCD Parallel interface
    * @param name The name used by the parent class to access the interface
    */
    TFT(proto_t displayproto,PortName port, PinName CS, PinName reset, PinName DC, PinName WR, PinName RD, const int lcdsize_x, const int lcdsize_y, const char* name);
    
    /** Create a monochrome LCD SPI interface
    * @param name The name used by the parent class to access the interface
    */
    TFT(proto_t displayproto, int Hz, PinName mosi, PinName miso, PinName sclk, PinName CS, PinName reset, PinName DC, const int lcdsize_x, const int lcdsize_y, const char* name);
    
    /////// functions that come for free, but can be overwritten///////////////////////////////////////////////////
/////// ----------------------------------------------------///////////////////////////////////////////////////

    /** Draw a pixel in the specified color.
    * @param x is the horizontal offset to this pixel.
    * @param y is the vertical offset to this pixel.
    * @param color defines the color for the pixel.
    */
    virtual void pixel(int x, int y, unsigned short color);        
    
    /** Set the window, which controls where items are written to the screen.
    * When something hits the window width, it wraps back to the left side
    * and down a row. If the initial write is outside the window, it will
    * be captured into the window when it crosses a boundary.
    * @param x is the left edge in pixels.
    * @param y is the top edge in pixels.
    * @param w is the window width in pixels.
    * @param h is the window height in pixels.
    */
    virtual void window(int x, int y, int w, int h);

    /** Push a single pixel into the window and increment position.
    * You must first call window() then push pixels.
    * @param color is the pixel color.
    */
    virtual void window_pushpixel(unsigned short color);
    
    /** Push some pixels of the same color into the window and increment position.
    * You must first call window() then push pixels.
    * @param color is the pixel color.
    * @param count: how many
    */
    virtual void window_pushpixel(unsigned short color, unsigned int count);
    
    /** Push array of pixel colors into the window and increment position.
    * You must first call window() then push pixels.
    * @param color is the pixel color.
    */
    virtual void window_pushpixelbuf(unsigned short* color, unsigned int lenght);
 
    /** Framebuffer is not used for TFT
    */
    virtual void copy_to_lcd(){ };

    /** invert the screen
      *
      * @param o = 0 normal, 1 invert
      */
    void invert(unsigned char o);

    /** clear the entire screen
    * The inherited one sets windomax then fill with background color
    * We override it to speedup
    */
    virtual void cls();
    
    /** Set the orientation of the screen
    *  x,y: 0,0 is always top left 
    *
    * @param o direction to use the screen (0-3)
    * 0 = default 0° portrait view
    * 1 = +90° landscape view
    * 2 = +180° portrait view
    * 3 = -90° landscape view
    *
    */  
    virtual void set_orientation(int o);
    
    /** Set ChipSelect high or low
    * @param enable 0/1   
    */
    virtual void BusEnable(bool enable);
    
    
protected:


////// functions needed by parent class ///////////////////////////////////////
////// -------------------------------- ///////////////////////////////////////

    /** Send 8bit command to display controller 
    *
    * @param cmd: byte to send  
    * @note if protocol is SPI16, it will insert NOP cmd before, so if cmd is a 2byte cmd, the second cmd will be broken. Use wr_cmd16 for 2bytes cmds
    */   
    void wr_cmd8(unsigned char cmd);
    
    /** Send 8bit data to display controller 
    *
    * @param data: byte to send   
    *
    */   
    void wr_data8(unsigned char data);
    
    /** Send 2x8bit data to display controller 
    *
    * @param data: halfword to send   
    *
    */   
    void wr_data16(unsigned short data);
    
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
    void hw_reset();
    
    unsigned int scrollbugfix;
    bool mipistd;
    
private:

    Protocols* proto;
    const int LCDSIZE_X;
    const int LCDSIZE_Y;
 //   const int LCDPAGES;
 //   const int IC_X_SEGS;
 //   const int IC_Y_COMS;
 //   const int IC_PAGES;
    
 //   int page_offset;
 //   int col_offset;
    // pixel location
    int cur_x;
    int cur_y;
    // window location
    int win_x1;
    int win_x2;
    int win_y1;
    int win_y2;
    int orientation;
    unsigned int tftID;
    bool useNOP;
};

#endif