/* mbed UniGraphic library - Device specific class
 * SH1106 by Karl Zweimüller, based on
 * SSD1306 by Copyright (c) 2015 Peter Drescher
 * Released under the MIT License: http://mbed.org/license/mit
 */

#include "Protocols.h"
#include "SH1106.h"

//////////////////////////////////////////////////////////////////////////////////
// display settings ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////


#define IC_X_SEGS    132 // 132 SEG
#define IC_Y_COMS    64  // 64  COM

#define SH1106_SETCONTRAST         0x81
#define SH1106_DISPLAYALLON_RESUME 0xA4
#define SH1106_DISPLAYALLON        0xA5
#define SH1106_NORMALDISPLAY       0xA6
#define SH1106_INVERTDISPLAY       0xA7
#define SH1106_DISPLAYOFF          0xAE
#define SH1106_DISPLAYON           0xAF
#define SH1106_SETDISPLAYOFFSET    0xD3
#define SH1106_SETCOMPINS          0xDA
#define SH1106_SETVCOMDETECT       0xDB
#define SH1106_SETDISPLAYCLOCKDIV  0xD5
#define SH1106_SETPRECHARGE        0xD9
#define SH1106_SETMULTIPLEX        0xA8
#define SH1106_SETLOWCOLUMN        0x00
#define SH1106_SETHIGHCOLUMN       0x10
#define SH1106_SETSTARTLINE        0x40
#define SH1106_MEMORYMODE          0x20
#define SH1106_COMSCANINC          0xC0
#define SH1106_COMSCANDEC          0xC8
#define SH1106_SEGREMAP            0xA0
#define SH1106_CHARGEPUMP          0x8D

SH1106::SH1106(proto_t displayproto, PortName port, PinName CS, PinName reset, PinName DC, PinName WR, PinName RD, const char *name , unsigned int LCDSIZE_X, unsigned  int LCDSIZE_Y)
    : LCD(displayproto, port, CS, reset, DC, WR, RD, LCDSIZE_X, LCDSIZE_Y, IC_X_SEGS, IC_Y_COMS, name)
{
    hw_reset();
    BusEnable(true);
    init();
    cls();
    set_orientation(1);
    locate(0,0);
    copy_to_lcd();
}
SH1106::SH1106(proto_t displayproto, int Hz, PinName mosi, PinName miso, PinName sclk, PinName CS, PinName reset, PinName DC, const char *name, unsigned int LCDSIZE_X, unsigned  int LCDSIZE_Y)
    : LCD(displayproto, Hz, mosi, miso, sclk, CS, reset, DC, LCDSIZE_X, LCDSIZE_Y, IC_X_SEGS, IC_Y_COMS, name)
{
    hw_reset();
    BusEnable(true);
    init();
    cls();
    set_orientation(1);
    locate(0,0);
    copy_to_lcd();
}

SH1106::SH1106(proto_t displayproto, int Hz, int address, PinName sda, PinName scl, PinName reset, const char* name , unsigned int LCDSIZE_X, unsigned  int LCDSIZE_Y)
    : LCD(displayproto, Hz, address, sda, scl,reset, LCDSIZE_X, LCDSIZE_Y, IC_X_SEGS, IC_Y_COMS, name)
{
    hw_reset();
    init();
    cls();
    set_orientation(1);
    locate(0,0);
    copy_to_lcd();
}


// reset and init the lcd controller
void SH1106::init()
{
    /* Start Initial Sequence ----------------------------------------------------*/
    /* SH1106
     //  wr_cmd8(0xE2);   //  sw reset
       wait_ms(15);

       wr_cmd8(SH1106_DISPLAYOFF);    // no problem in SPI_16 for single byte cmds
       wr_cmd16((SH1106_SETDISPLAYCLOCKDIV<<8)|0x80);   // wr_cmd16 for multibyte cmds issue in SPI16 mode
     //  wr_cmd8(0x80); // in SPI_16 it would become 0xE380 and will break things up
       wr_cmd16((SH1106_SETMULTIPLEX<<8)|63);
    //   wr_cmd8(63);

       wr_cmd16((SH1106_SETDISPLAYOFFSET<<8)|0x00);
    //   wr_cmd8(0x0);

       wr_cmd8(SH1106_SETSTARTLINE | 0x0);            // line #0

       wr_cmd16((SH1106_CHARGEPUMP<<8)|0x14);
     //  wr_cmd8(0x14);                         // 0x10

       wr_cmd16((SH1106_MEMORYMODE<<8)|0x00);
    //   wr_cmd8(0x00);                                  // 0x0 act like ks0108

       wr_cmd8(SH1106_SEGREMAP ); //| 0x1);

       wr_cmd8(SH1106_COMSCANDEC);

       wr_cmd16((SH1106_SETCOMPINS<<8)|0x12);
     //  wr_cmd8(0x12);                           //        LCDSIZE_Y == 32 ? 0x02 : 0x12);

       wr_cmd16((SH1106_SETCONTRAST<<8)|0xCF);
    //   wr_cmd8(0xCF);                              //  _rawHeight == 32 ? 0x8F : ((vccstate == SH1106_EXTERNALVCC) ? 0x9F : 0xCF) );

       wr_cmd16((SH1106_SETPRECHARGE<<8)|0xF1);
    //   wr_cmd8(0xF1);                               // : 0x22);

       wr_cmd16((SH1106_SETVCOMDETECT<<8)|0x40);
    //   wr_cmd8(0x40);

       wr_cmd8(SH1106_DISPLAYALLON_RESUME);

       //wr_cmd8(SH1106_NORMALDISPLAY);
       wr_cmd8(SH1106_INVERTDISPLAY);

       wr_cmd8(SH1106_DISPLAYON);
    */

//SH1106
    wr_cmd8(0xAE);//--turn off oled panel
    wr_cmd8(0x02);//---set low column address
    wr_cmd8(0x10);//---set high column address
    wr_cmd8(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    wr_cmd8(0x81);//--set contrast control register
    wr_cmd8(0xA0);//--Set contrast
    wr_cmd8(0xA8);//--set multiplex ratio(1 to 64)
    wr_cmd8(0x3F);//--1/64 duty
    wr_cmd8(0xD3);//-set display offset    Shift Mapping RAM Counter (0x00~0x3F)
    wr_cmd8(0x00);//-not offset
    wr_cmd8(0xd5);//--set display clock divide ratio/oscillator frequency
    wr_cmd8(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
    wr_cmd8(0xD9);//--set pre-charge period
    wr_cmd8(0xF1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    wr_cmd8(0xDA);//--set com pins hardware configuration
    wr_cmd8(0x12); // or 0x02
    wr_cmd8(0xDB);//--set vcomh
    wr_cmd8(0x40);//Set VCOM Deselect Level
    //wr_cmd8(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02) // not on SH1106
    wr_cmd8(0xA4);// Disable Entire Display On (0xa4/0xa5)
    wr_cmd8(0xA6);// Disable Inverse Display On (0xa6/a7)

    wr_cmd8(0xA0); // normal order
    //wr_cmd8(0xA1); // reverse order
    wr_cmd8(0xC0);//Set COM/Row Scan Direction normal
    //wr_cmd8(0xC8); //reverse direction
    wr_cmd8(0xAF);//--turn on oled panel
}

////////////////////////////////////////////////////////////////////
// functions that overrides the standard ones implemented in LCD.cpp
////////////////////////////////////////////////////////////////////

void SH1106::copy_to_lcd(void)
{

    for(uint8_t page=0; page<8 /*_LCDPAGES */; page++) {
        wr_cmd8(0xB0+page);        // set page
        wr_cmd8(0x02);             // set column low nibble My Display starts at column 2 (up to column 130 of 132)
        wr_cmd8(0x10);             // set column hi  nibble
        wr_grambuf(buffer16+(page*screensize_X>>1), screensize_X>>1);   // send whole page pixels as shorts(16bit) not bytes!
    }
}

void SH1106::cls(void)
{
    unsigned short tmp = _background^0xFFFF;
    //memset(buffer,tmp,/*screensize_X*_LCDPAGES*/ 128*64/8);  // clear display buffer
    memset(buffer,0x00,screensize_X*(screensize_Y>>3));  // clear display buffer
    if (get_auto_up()) {
        copy_to_lcd();
    }
}

void SH1106::mirrorXY(mirror_t mode)
{
    switch (mode) {
        case(NONE):
            wr_cmd16(0xA0C0);
            break;
        case(X):
            wr_cmd16(0xA1C0);
            break;
        case(Y):
            wr_cmd16(0xA0C8);
            break;
        case(XY):
            wr_cmd16(0xA1C8);
            break;
    }
}

void SH1106::set_contrast(int o)
{
    contrast = o;

    wr_cmd16(0x8100|(o&0xFF));
}

void SH1106::pixel(int x, int y, unsigned short color)
{
    // first check parameter
    if((x >= screensize_X) || (y >= screensize_Y) || (x<0) || (y<0)) return;

    //Buffer[(Xpoint + (Ypoint / 8) * sOLED_DIS.OLED_Dis_Column)] |= 1 << (Ypoint % 8);
    //Buffer[(Xpoint + (Ypoint / 8) * sOLED_DIS.OLED_Dis_Column)] |= 0 << (Ypoint % 8);
    if (color) {
        buffer[x+((y>>3)*screensize_X)] |=  1 << (y % 8);
    } else       {
        buffer[x+((y>>3)*screensize_X)] |=  0 << (y % 8);
    }
}


unsigned short SH1106::pixelread(int x, int y)
{
    // first check parameter
    if((x >= screensize_X) || (y >= screensize_Y) || (x<0) || (y<0)) return 0;

    //Buffer[(Xpoint + (Ypoint / 8) * sOLED_DIS.OLED_Dis_Column)] |= 1 << (Ypoint % 8);
    //Buffer[(Xpoint + (Ypoint / 8) * sOLED_DIS.OLED_Dis_Column)] |= 0 << (Ypoint % 8);
    if (buffer[x+((y>>3)*screensize_X)] &  1 << (y % 8) == 0)
        return Black ;  // pixel not set, Black
    else return White; // pixel set, White
}
