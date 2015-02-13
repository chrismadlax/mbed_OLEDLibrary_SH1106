#include "TFT.h"

//#include "mbed_debug.h"

#define SWAP(a, b)  { a ^= b; b ^= a; a ^= b; }

TFT::TFT(proto_t displayproto, PortName port, PinName CS, PinName reset, PinName DC, PinName WR, PinName RD, const int lcdsize_x, const int lcdsize_y, const char *name)
    : GraphicsDisplay(name), LCDSIZE_X(lcdsize_x), LCDSIZE_Y(lcdsize_y)
{
    if(displayproto==PAR_8) proto = new PAR8(port, CS, reset, DC, WR, RD);
    useNOP=false;
    set_orientation(0);
    foreground(White);
    background(Black);
    set_auto_up(false); //we don't have framebuffer
    mipistd=false;
  //  cls();
  //  locate(0,0);
}
TFT::TFT(proto_t displayproto, int Hz, PinName mosi, PinName miso, PinName sclk, PinName CS, PinName reset, PinName DC, const int lcdsize_x, const int lcdsize_y, const char *name)
    : GraphicsDisplay(name), LCDSIZE_X(lcdsize_x), LCDSIZE_Y(lcdsize_y)
{
    if(displayproto==SPI_8)
    {
        proto = new SPI8(Hz, mosi, miso, sclk, CS, reset, DC);
        useNOP=false;
    }
    else if(displayproto==SPI_16)
    {
        proto = new SPI16(Hz, mosi, miso, sclk, CS, reset, DC);
        useNOP=true;
    }
  //  cls();
    set_orientation(0);
    foreground(White);
    background(Black);
    set_auto_up(false);
    mipistd=false;
  //  locate(0,0);
}
void TFT::wr_cmd8(unsigned char cmd)
    {
        if(useNOP) proto->wr_cmd16(cmd); // 0x0000|cmd, 00 is NOP cmd for TFT
        else proto->wr_cmd8(cmd);
    }
void TFT::wr_data8(unsigned char data)
    {
        proto->wr_data8(data);
    }
void TFT::wr_data8(unsigned char data, unsigned int count)
    {
        proto->wr_data8(data, count);
    }
void TFT::wr_data8buf(unsigned char* data, unsigned int lenght)
    {
        proto->wr_data8buf(data, lenght);
    }
void TFT::wr_cmd16(unsigned short cmd)
    {
        proto->wr_cmd16(cmd);
    }
void TFT::wr_data16(unsigned short data)
    {
        proto->wr_data16(data);
    }
void TFT::wr_data16(unsigned short data, unsigned int count)
    {
        proto->wr_data16(data, count);
    }
void TFT::wr_data16buf(unsigned short* data, unsigned int lenght)
    {
        proto->wr_data16buf(data, lenght);
    }
void TFT::hw_reset()
    {
        proto->hw_reset();
    }
void TFT::BusEnable(bool enable)
    {
        proto->BusEnable(enable);
    }
// color TFT can rotate in hw (swap raw<->columns) for landscape views
void TFT::set_orientation(int o)
{
    orientation = o;
    wr_cmd8(0x36);
    switch (orientation) {
        case 0:// default, portrait view 0°
            if(mipistd) wr_data8(0x0A); // this is in real a vertical flip enabled, seems most displays are vertical flipped
            else wr_data8(0x48); //48 for 9341
            set_width(LCDSIZE_X);
            set_height(LCDSIZE_Y);
            break;
        case 1:// landscape view +90°
            if(mipistd) wr_data8(0x28); 
            else wr_data8(0x29);//28 for 9341
            set_width(LCDSIZE_Y);
            set_height(LCDSIZE_X);
            break;
        case 2:// portrait view +180°
            if(mipistd) wr_data8(0x09); 
            else wr_data8(0x99);//88 for 9341
            set_width(LCDSIZE_X);
            set_height(LCDSIZE_Y);
            break;
        case 3:// landscape view -90°
            if(mipistd) wr_data8(0x2B); 
            else wr_data8(0xF8);//E8 for 9341
            set_width(LCDSIZE_Y);
            set_height(LCDSIZE_X);
            break;
    }
}
// TFT have both column and raw autoincrement inside a window, with internal counters
void TFT::window(int x, int y, int w, int h)
{
    //ili9486 does not like truncated 2A/2B cmds, at least in par mode
    //setting only start column/page would speedup, but needs a windowmax() before, maybe implement later
    wr_cmd8(0x2A);
    wr_data16(x);   //start column
    wr_data16(x+w-1);//end column

    wr_cmd8(0x2B);
    wr_data16(y);   //start page
    wr_data16(y+h-1);//end page
    
    wr_cmd8(0x2C);  //write mem, just send pixels color next
}
//for TFT, just send data, position counters are in hw
void TFT::window_pushpixel(unsigned short color)
{
    proto->wr_data16(color);
}
void TFT::window_pushpixel(unsigned short color, unsigned int count)
{
    proto->wr_data16(color, count);
}
void TFT::window_pushpixelbuf(unsigned short* color, unsigned int lenght)
    {
        proto->wr_data16buf(color, lenght);
    }
void TFT::pixel(int x, int y, unsigned short color)
{
    window(x,y,1,1);
    wr_data16(color);   // 2C expects 16bit parameters
    //proto->wr_data16(color);
}
void TFT::cls (void)
{
    WindowMax();
  //  wr_data16(_background,pixels);
    wr_data16(0,LCDSIZE_X*LCDSIZE_Y);
    //proto->wr_data16(_background,pixels);
}