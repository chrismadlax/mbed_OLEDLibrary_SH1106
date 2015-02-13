#include "LCD.h"

//#include "mbed_debug.h"
//#define LCDPAGES        (LCDSIZE_Y>>3) // 8raws per page
//#define IC_PAGES        (IC_Y_COMS>>3) // max pages in IC ddram, 8raws per page
#define SWAP(a, b)  { a ^= b; b ^= a; a ^= b; }
//#define USEFRAMEBUFFER
#define NOPCMD 0xE300
//#define FRAMEBUFSIZE    (LCDSIZE_X*LCDPAGES)
Protocols* proto;

LCD::LCD(proto_t displayproto, PortName port, PinName CS, PinName reset, PinName DC, PinName WR, PinName RD, const int lcdsize_x, const int lcdsize_y, const int ic_x_segs, const int ic_y_coms, const char *name)
    : /*PAR8(port, CS, reset, DC, WR, RD),*/ GraphicsDisplay(name), LCDSIZE_X(lcdsize_x), LCDSIZE_Y(lcdsize_y), LCDPAGES(lcdsize_y>>3), IC_X_SEGS(ic_x_segs), IC_Y_COMS(ic_y_coms), IC_PAGES(ic_y_coms>>3)
{
 //   LCDPAGES = LCDSIZE_Y>>3;
 //   IC_PAGES = IC_Y_COMS>>3;
  //  buffer = new unsigned char [LCDSIZE_X*LCDPAGES];
  //  PAR8 par8proto(port, CS, reset, DC, WR, RD);
    if(displayproto==PAR_8) proto = new PAR8(port, CS, reset, DC, WR, RD);
    useNOP=false;
    buffer = (unsigned char*) malloc (LCDSIZE_X*LCDPAGES);
    buffer16 = (unsigned short*)buffer;
    draw_mode = NORMAL;
    set_orientation(1);
  //  cls();
  //  locate(0,0);
}
LCD::LCD(proto_t displayproto, int Hz, PinName mosi, PinName miso, PinName sclk, PinName CS, PinName reset, PinName DC, const int lcdsize_x, const int lcdsize_y, const int ic_x_segs, const int ic_y_coms, const char *name)
    : GraphicsDisplay(name), LCDSIZE_X(lcdsize_x), LCDSIZE_Y(lcdsize_y), LCDPAGES(lcdsize_y>>3), IC_X_SEGS(ic_x_segs), IC_Y_COMS(ic_y_coms), IC_PAGES(ic_y_coms>>3)
{
 //   LCDPAGES = LCDSIZE_Y>>3;
 //   IC_PAGES = IC_Y_COMS>>3;
  //  buffer = new unsigned char [LCDSIZE_X*LCDPAGES];
  //  PAR8 par8proto(port, CS, reset, DC, WR, RD);
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
    buffer = (unsigned char*) malloc (LCDSIZE_X*LCDPAGES);
    buffer16 = (unsigned short*)buffer;
    draw_mode = NORMAL;
  //  cls();
    set_orientation(1);
  //  locate(0,0);

}
LCD::~LCD()
{
    free(buffer);
}

void LCD::wr_cmd8(unsigned char cmd)
    {
        if(useNOP) proto->wr_cmd16(NOPCMD|cmd);
        else proto->wr_cmd8(cmd);
    }
void LCD::wr_data8(unsigned char data)
    {
        proto->wr_data8(data);
    }
void LCD::wr_data8(unsigned char data, unsigned int count)
    {
        proto->wr_data8(data, count);
    }
void LCD::wr_data8buf(unsigned char* data, unsigned int lenght)
    {
        proto->wr_data8buf(data, lenght);
    }
void LCD::wr_cmd16(unsigned short cmd)
    {
        proto->wr_cmd16(cmd);
    }
void LCD::wr_data16(unsigned short data, unsigned int count)
    {
        proto->wr_data16(data, count);
    }
void LCD::wr_data16buf(unsigned short* data, unsigned int lenght)
    {
        proto->wr_data16buf(data, lenght);
    }
void LCD::hw_reset()
    {
        proto->hw_reset();
    }
void LCD::BusEnable(bool enable)
    {
        proto->BusEnable(enable);
    }



// monochrome LCD driver ICs does not have ram rotate in hw (swap raw<->columns) like TFT displays
// for portrait views, XY swap will be done in sw in pixel() function
void LCD::set_orientation(int o)
{
    orientation = o;
    switch (o) {
        case (0):// portrait view -90°
            mirrorXY(Y);
            col_offset = 0;
            page_offset = IC_PAGES-LCDPAGES;
            set_width(LCDSIZE_Y);
            set_height(LCDSIZE_X);
        //    portrait = true;
            break;
        case (1): // default, landscape view 0°
            mirrorXY(NONE);
            col_offset = 0;
            page_offset = 0;
            set_width(LCDSIZE_X);
            set_height(LCDSIZE_Y);
       //     portrait = false;
            break;
        case (2):// portrait view +90°
            mirrorXY(X);
            col_offset = IC_X_SEGS-LCDSIZE_X; // some displays have less pixels than IC ram
            page_offset = 0;
            set_width(LCDSIZE_Y);
            set_height(LCDSIZE_X);
       //     portrait = true;
            break;
        case (3):// landscape view +180°
            mirrorXY(XY);
            col_offset = IC_X_SEGS-LCDSIZE_X;
            page_offset = IC_PAGES-LCDPAGES;
            set_width(LCDSIZE_X);
            set_height(LCDSIZE_Y);
       //     portrait = false;
            break;
    }
}
void LCD::mirrorXY(mirror_t mode)
{
    switch (mode)
    {
        case(NONE):
         //   wr_cmd8(0xA0);
            wr_cmd16(0xA0C8); // this is in real Y mirror command, but seems most displays have COMs wired inverted, so assume this is the default no-y-mirror
            break;
        case(X):
        //    wr_cmd8(0xA1);
            wr_cmd16(0xA1C8);
            break;
        case(Y):
        //    wr_cmd8(0xA0);
            wr_cmd16(0xA0C0);
            break;
        case(XY):
        //    wr_cmd8(0xA1);
            wr_cmd16(0xA1C0);
            break;
    }
}
void LCD::invert(unsigned char o)
{
    if(o == 0) wr_cmd8(0xA6);
    else wr_cmd8(0xA7);
}

void LCD::set_contrast(int o)
{
    contrast = o;
 //   wr_cmd8(0x81);      //  set volume
    wr_cmd16(0x8100|(o&0x3F));
}
void LCD::set_auto_up(bool up)
{
    if(up) auto_up = true;
    else auto_up = false;
}
bool LCD::get_auto_up(void)
{
    return (auto_up);
}
int LCD::get_contrast(void)
{
    return(contrast);
}
void LCD::window(int x, int y, int w, int h) {
    // current pixel location
    cur_x = x;
    cur_y = y;
    // window settings
    win_x1 = x;
    win_x2 = x + w - 1;
    win_y1 = y;
    win_y2 = y + h - 1;
}
void LCD::window_pushpixel(unsigned short color) {
    pixel(cur_x, cur_y, color);
    cur_x++;
    if(cur_x > win_x2) {
        cur_x = win_x1;
        cur_y++;
        if(cur_y > win_y2) {
            cur_y = win_y1;
        }
    }
}
void LCD::pixel(int x, int y, unsigned short color)
{
    if(!(orientation&1)) SWAP(x,y);
    // first check parameter
    if((x >= LCDSIZE_X) || (y >= LCDSIZE_Y)) return;

//    if(draw_mode == NORMAL)
//    {
        if(color == 0) buffer[(x + ((y>>3)*LCDSIZE_X))^1] &= ~(1 << (y&7));  // erase pixel
        else buffer[(x + ((y>>3)*LCDSIZE_X))^1] |= (1 << (y&7));   // set pixel
//    }
//    else
//    { // XOR mode
//        if(color == 1) buffer[x + ((y>>3) * LCDSIZE_X)] ^= (1 << (y&7));   // xor pixel
//    }
}
void LCD::copy_to_lcd(void)
{
    unsigned short i=0;
    unsigned short setcolcmd = 0x0010 | ((col_offset&0xF)<<8) | (col_offset>>4);
    for(int page=0; page<LCDPAGES; page++)
    {
      //  wr_cmd8(col_offset&0xF);              // set column low nibble
      //  wr_cmd8(0x10|(col_offset>>4));      // set column hi  nibble
        wr_cmd16(setcolcmd);
        wr_cmd8(0xB0|(page+page_offset));      // set page
        wr_data16buf(buffer16+i, LCDSIZE_X>>1);   // send whole page pixels
        i+=LCDSIZE_X>>1;
    }
}
void LCD::cls(void)
{
    memset(buffer,0x00,LCDSIZE_X*LCDPAGES);  // clear display buffer
    unsigned short setcolcmd = 0x0010 | ((col_offset&0xF)<<8) | (col_offset>>4);
    for(int page=0; page<LCDPAGES; page++)
    {
     //   wr_cmd8((unsigned char)col_offset&0xF);              // set column low nibble
     //   wr_cmd8(0x10|(col_offset>>4));      // set column hi  nibble
        wr_cmd16(setcolcmd);
        wr_cmd8(0xB0|(page+page_offset));      // set page
        wr_data16(0, LCDSIZE_X>>1);   // send whole page pixels =0
    }
}