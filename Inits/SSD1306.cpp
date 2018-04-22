/* mbed UniGraphic library - Device specific class
 * Copyright (c) 2015 Peter Drescher
 * Released under the MIT License: http://mbed.org/license/mit
 */
  
#include "Protocols.h"
#include "SSD1306.h"

//////////////////////////////////////////////////////////////////////////////////
// display settings ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////


#define IC_X_SEGS    132 // 132 SEG
#define IC_Y_COMS    64  // 64  COM

#define SSD1306_SETCONTRAST         0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON        0xA5
#define SSD1306_NORMALDISPLAY       0xA6
#define SSD1306_INVERTDISPLAY       0xA7
#define SSD1306_DISPLAYOFF          0xAE
#define SSD1306_DISPLAYON           0xAF
#define SSD1306_SETDISPLAYOFFSET    0xD3
#define SSD1306_SETCOMPINS          0xDA
#define SSD1306_SETVCOMDETECT       0xDB
#define SSD1306_SETDISPLAYCLOCKDIV  0xD5
#define SSD1306_SETPRECHARGE        0xD9
#define SSD1306_SETMULTIPLEX        0xA8
#define SSD1306_SETLOWCOLUMN        0x00
#define SSD1306_SETHIGHCOLUMN       0x10
#define SSD1306_SETSTARTLINE        0x40
#define SSD1306_MEMORYMODE          0x20
#define SSD1306_COMSCANINC          0xC0
#define SSD1306_COMSCANDEC          0xC8
#define SSD1306_SEGREMAP            0xA0
#define SSD1306_CHARGEPUMP          0x8D

SSD1306::SSD1306(proto_t displayproto, PortName port, PinName CS, PinName reset, PinName DC, PinName WR, PinName RD, const char *name , unsigned int LCDSIZE_X, unsigned  int LCDSIZE_Y)
    : LCD(displayproto, port, CS, reset, DC, WR, RD, LCDSIZE_X, LCDSIZE_Y, IC_X_SEGS, IC_Y_COMS, name)
{
    hw_reset();
    BusEnable(true);
    init();
    cls();
    set_orientation(1);
    locate(0,0);
}
SSD1306::SSD1306(proto_t displayproto, int Hz, PinName mosi, PinName miso, PinName sclk, PinName CS, PinName reset, PinName DC, const char *name, unsigned int LCDSIZE_X, unsigned  int LCDSIZE_Y)
    : LCD(displayproto, Hz, mosi, miso, sclk, CS, reset, DC, LCDSIZE_X, LCDSIZE_Y, IC_X_SEGS, IC_Y_COMS, name)
{
    hw_reset();
    BusEnable(true);
    init();
    cls();
    set_orientation(1);
    locate(0,0);
}

SSD1306::SSD1306(proto_t displayproto, int Hz, int address, PinName sda, PinName scl, PinName reset, const char* name , unsigned int LCDSIZE_X, unsigned  int LCDSIZE_Y)
    : LCD(displayproto, Hz, address, sda, scl,reset, LCDSIZE_X, LCDSIZE_Y, IC_X_SEGS, IC_Y_COMS, name)
{
    hw_reset();
    init();
    cls();
    set_orientation(1);
    locate(0,0);
}


// reset and init the lcd controller
void SSD1306::init()
{
    /* Start Initial Sequence ----------------------------------------------------*/
 /* SSD1306   
  //  wr_cmd8(0xE2);   //  sw reset
    wait_ms(15);
    
    wr_cmd8(SSD1306_DISPLAYOFF);    // no problem in SPI_16 for single byte cmds
    wr_cmd16((SSD1306_SETDISPLAYCLOCKDIV<<8)|0x80);   // wr_cmd16 for multibyte cmds issue in SPI16 mode
  //  wr_cmd8(0x80); // in SPI_16 it would become 0xE380 and will break things up
    wr_cmd16((SSD1306_SETMULTIPLEX<<8)|63);
 //   wr_cmd8(63);
    
    wr_cmd16((SSD1306_SETDISPLAYOFFSET<<8)|0x00);
 //   wr_cmd8(0x0);
    
    wr_cmd8(SSD1306_SETSTARTLINE | 0x0);            // line #0

    wr_cmd16((SSD1306_CHARGEPUMP<<8)|0x14);
  //  wr_cmd8(0x14);                         // 0x10 

    wr_cmd16((SSD1306_MEMORYMODE<<8)|0x00);
 //   wr_cmd8(0x00);                                  // 0x0 act like ks0108

    wr_cmd8(SSD1306_SEGREMAP ); //| 0x1);

    wr_cmd8(SSD1306_COMSCANDEC);

    wr_cmd16((SSD1306_SETCOMPINS<<8)|0x12);
  //  wr_cmd8(0x12);                           //        LCDSIZE_Y == 32 ? 0x02 : 0x12);        

    wr_cmd16((SSD1306_SETCONTRAST<<8)|0xCF);
 //   wr_cmd8(0xCF);                              //  _rawHeight == 32 ? 0x8F : ((vccstate == SSD1306_EXTERNALVCC) ? 0x9F : 0xCF) );

    wr_cmd16((SSD1306_SETPRECHARGE<<8)|0xF1);
 //   wr_cmd8(0xF1);                               // : 0x22);

    wr_cmd16((SSD1306_SETVCOMDETECT<<8)|0x40);
 //   wr_cmd8(0x40);

    wr_cmd8(SSD1306_DISPLAYALLON_RESUME);

    //wr_cmd8(SSD1306_NORMALDISPLAY);
    wr_cmd8(SSD1306_INVERTDISPLAY);
    
    wr_cmd8(SSD1306_DISPLAYON);
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

    wr_cmd8(0xA1); // reverse order
    //wr_cmd8(0xC0);//Set COM/Row Scan Direction
    wr_cmd8(0xC8); //reverse direction    
    wr_cmd8(0xAF);//--turn on oled panel
}

////////////////////////////////////////////////////////////////////
// functions that overrides the standard ones implemented in LCD.cpp
////////////////////////////////////////////////////////////////////

void SSD1306::copy_to_lcd(void)
{

    for(int page=0; page<8 /*_LCDPAGES */; page++)
    {
        wr_cmd8(0x02);             // set column low nibble
        wr_cmd8(0x10);             // set column hi  nibble
        wr_cmd8(0xB0+page);        // set page
        wr_grambuf(buffer16+(page*screensize_X>>1), /*screensize_X*/ 128>>1);   // send whole page pixels as shorts(16bit) not bytes!
    }
}

void SSD1306::cls(void)
{
    unsigned short tmp = _background^0xFFFF;
    //memset(buffer,tmp,/*screensize_X*_LCDPAGES*/ 128*64/8);  // clear display buffer
    memset(buffer,0x00,screensize_X*(screensize_Y>>3));  // clear display buffer
    copy_to_lcd();
}

void SSD1306::mirrorXY(mirror_t mode)
{
    switch (mode)
    {
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

void SSD1306::set_contrast(int o)
{
    contrast = o;
  
    wr_cmd16(0x8100|(o&0xFF));
}

void SSD1306::pixel(int x, int y, unsigned short color)
{
    // first check parameter
    if((x >= screensize_X) || (y >= screensize_Y) || (x<0) || (y<0)) return;

    //Buffer[(Xpoint + (Ypoint / 8) * sOLED_DIS.OLED_Dis_Column)] |= 1 << (Ypoint % 8);
    //Buffer[(Xpoint + (Ypoint / 8) * sOLED_DIS.OLED_Dis_Column)] |= 0 << (Ypoint % 8);
    if (color) {buffer[x+((y>>3)*screensize_X)] |=  0 << (y % 8);}
    else       {buffer[x+((y>>3)*screensize_X)] |=  1 << (y % 8);}
    

}


const uint8_t scroll_speed[8]={3,2,1,6,0,5,4,7};

////////////////////////////////////////////////////////////////////
// functions addon  to LCD.cpp
////////////////////////////////////////////////////////////////////
void SSD1306::horizontal_scroll(int l_r,int s_page,int e_page,int speed){
    wr_cmd8(0x2E);                      // deactivate scroll before change
    if(l_r == 1){
        wr_cmd16(0x2700);               // horizontal scroll left
    }
    else {
        wr_cmd16(0x2600);
    }
    wr_cmd16((s_page & 0x07)<<8 | (scroll_speed[speed & 0x07]));
    wr_cmd16((e_page & 0x07)<<8 );
    wr_cmd16(0xFF2F);
}

void SSD1306::horiz_vert_scroll(int l_r,int s_page,int e_page,int v_off,int speed){
    wr_cmd8(0x2E);                      // deactivate scroll before change
    if(l_r == 1){
            wr_cmd16(0x2A00);               // horizontal scroll left
        }
        else {
            wr_cmd16(0x2900);
        }
        wr_cmd16((s_page & 0x07)<<8 | (scroll_speed[speed & 0x07]));
        wr_cmd16((e_page & 0x07)<<8 | (v_off & 0x3F) );
        wr_cmd8(0x2F);

}

void SSD1306::end_scroll(){
    wr_cmd8(0x2E);
}
