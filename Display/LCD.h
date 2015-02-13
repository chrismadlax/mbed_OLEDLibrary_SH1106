
#ifndef MBED_LCD_H
#define MBED_LCD_H

#include "GraphicsDisplay.h"
#include "PAR8.h"
#include "SPI8.h"
#include "SPI16.h"
#include "Protocols.h"

#define Black           1
#define White           0



/** Draw mode
  * NORMAl
  * XOR set pixel by xor the screen
  */
enum {NORMAL,XOR};

/** Mirror mode */
enum mirror_t {X,Y,XY,NONE};


/** A common base class for monochrome Display
*/
class LCD : public GraphicsDisplay
{

public:         
          
    /** Create a monochrome LCD Parallel interface
    * @param name The name used by the parent class to access the interface
    */
    LCD(proto_t displayproto,PortName port, PinName CS, PinName reset, PinName DC, PinName WR, PinName RD, const int lcdsize_x, const int lcdsize_y, const int ic_x_segs, const int ic_y_coms, const char* name);
    
    /** Create a monochrome LCD SPI interface
    * @param name The name used by the parent class to access the interface
    */
    LCD(proto_t displayproto, int Hz, PinName mosi, PinName miso, PinName sclk, PinName CS, PinName reset, PinName DC, const int lcdsize_x, const int lcdsize_y, const int ic_x_segs, const int ic_y_coms, const char* name);
    
    /** Destructor
    * will free framebuffer
    */
    virtual ~LCD();
    

    
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
    * You must first call window() then push pixels in loop.
    * @param color is the pixel color.
    */
    virtual void window_pushpixel(unsigned short color);
 
    /** Framebuffer is used, it needs to be sent to LCD from time to time
    */
    virtual void copy_to_lcd();
    
    /** set the contrast of the screen
      *
      * @param o contrast 0-63
      * @note may be overrided in case of not standard command
      */
    virtual void set_contrast(int o);

    /** read the contrast level
      *
      */
    int get_contrast(void);

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
    
    /** setup auto update of screen 
      *
      * @param up 1 = on , 0 = off
      * if switched off the program has to call copy_to_lcd() 
      * to update screen from framebuffer
      */
    void set_auto_up(bool up);
 
    /** get status of the auto update function
      *
      *  @returns if auto update is on
      */
    bool get_auto_up(void);
    
    
    
    /** Set the orientation of the screen
    *  x,y: 0,0 is always top left 
    *
    * @param o direction to use the screen (0-3)
    * 0 = -90°
    * 1 = default 0°
    * 2 = +90°
    * 3 = +180°
    *
    */  
    void set_orientation(int o);
    
    /** Set ChipSelect high or low
    * @param enable 0/1   
    */
    virtual void BusEnable(bool enable);
    
    
protected:

    /** set mirror mode
      * @note may be overridden by specific display init class in case of not standard cmds or inverted wiring 
      * @param mode NONE, X, Y, XY 
      */
    virtual void mirrorXY(mirror_t mode);

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
    
    /** Send same 8bit data to display controller multiple times
    *
    * @param data: byte to send
    * @param count: how many
    *
    */   
    void wr_data8(unsigned char data, unsigned int count);
    
    /** Send array of data bytes to display controller
    *
    * @param data: unsigned char data array
    * @param lenght: lenght of array
    *
    */   
    void wr_data8buf(unsigned char* data, unsigned int lenght);
    
    /** Send 16bit command to display controller 
    *
    * @param cmd: halfword to send  
    *
    */   
    void wr_cmd16(unsigned short cmd);
    
    /** Send 16bit data to display controller 
    *
    * @param data: halfword to send   
    *
    */   
    //void wr_data16(unsigned short data);
    
    /** Send same 16bit data to display controller multiple times
    *
    * @param data: halfword to send
    * @param count: how many
    *
    */   
    void wr_data16(unsigned short data, unsigned int count);
    
    /** Send array of data shorts to display controller
    *
    * @param data: unsigned short data array
    * @param lenght: lenght (in shorts)
    *
    */   
    void wr_data16buf(unsigned short* data, unsigned int lenght);
    
    /** HW reset sequence (without display init commands)   
    */
    void hw_reset();
  
    int draw_mode;
    int contrast;
    
private:


    unsigned char *buffer;
    unsigned short *buffer16;
    const int LCDSIZE_X;
    const int LCDSIZE_Y;
    const int LCDPAGES;
    const int IC_X_SEGS;
    const int IC_Y_COMS;
    const int IC_PAGES;
    
    int page_offset;
    int col_offset;
    // pixel location
    int cur_x;
    int cur_y;
    // window location
    int win_x1;
    int win_x2;
    int win_y1;
    int win_y2;
    int orientation;
    bool useNOP;
};

#endif