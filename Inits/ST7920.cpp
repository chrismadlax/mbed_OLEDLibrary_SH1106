/* mbed UniGraphic library - Device specific class
* for ST7920 by Karl Zweimueller https://developer.mbed.org/users/charly/
* Copyright (c) 2015 Giuliano Dianda
* Released under the MIT License: http://mbed.org/license/mit
*/
#include "Protocols.h"
#include "ST7920.h"

/*this is a quite standard config for ST7920 and similars*/

//////////////////////////////////////////////////////////////////////////////////
// display settings ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
#define IC_X_SEGS    256 // ST7920 SEG has range 64-256
#define IC_Y_COMS    32  // ST7920 COM has 32 COMS fix
// see ST7920_v4.0 page 1/49



ST7920::ST7920(proto_t displayproto, PortName port, PinName CS, PinName reset, PinName DC, PinName WR, PinName RD, const char *name, unsigned int LCDSIZE_X, unsigned  int LCDSIZE_Y)
    : LCD(displayproto, port, CS, reset, DC, WR, RD, LCDSIZE_X, LCDSIZE_Y, IC_X_SEGS, IC_Y_COMS, name)
{
    hw_reset();
    BusEnable(true);
    init();
    cls();
    set_orientation(1);
    locate(0,0);
}
ST7920::ST7920(proto_t displayproto, PinName* buspins, PinName CS, PinName reset, PinName DC, PinName WR, PinName RD, const char *name, unsigned int LCDSIZE_X, unsigned  int LCDSIZE_Y)
    : LCD(displayproto, buspins, CS, reset, DC, WR, RD, LCDSIZE_X, LCDSIZE_Y, IC_X_SEGS, IC_Y_COMS, name)
{
    hw_reset();
    BusEnable(true);
    init();
    cls();
    set_orientation(1);
    locate(0,0);
}
ST7920::ST7920(proto_t displayproto, int Hz, PinName mosi, PinName miso, PinName sclk, PinName CS, PinName reset, PinName DC, const char *name, unsigned int LCDSIZE_X, unsigned  int LCDSIZE_Y)
    : LCD(displayproto, Hz, mosi, miso, sclk, CS, reset, DC, LCDSIZE_X, LCDSIZE_Y, IC_X_SEGS, IC_Y_COMS, name)
{
    hw_reset();
    BusEnable(true);
    init();
    cls();
    set_orientation(1);
    locate(0,0);
}

//WriteInstructionRegister IR
void ST7920::wir(unsigned char data)
{

    /*
    Timing Diagram of Serial Mode Data Transfer on p.20

    |-------------------|------------------------|------------------------|
    |  Synchronizing    | 1-st Byte Higher data  |  2-nd Byte Lower data  |
    |-------------------|------------------------|------------------------|
    | 1 1 1 1 1 RW RS 0 | D7 D6 D5 D4  0  0  0  0| D3 D2 D1 D0  0  0  0  0|
    |-------------------|------------------------|------------------------|
    */

    wr_data8(0xf8);        // Sync.    - 1  1  1  1  1  0  0  0  RW=0,RS=0
    wr_data8(data & 0xf0); // HighData - D7 D6 D5 D4 0  0  0  0
    wr_data8(data << 4);   // LowData  - D3 D2 D1 D0 0  0  0  0
    if (data == 0x01) {
        // Display Clear
        wait_ms(1.6);
    } else {
        //everything else
        wait_us(72);
    }
}

//WriteDataRegister DR
void ST7920::wdr(unsigned char data)
{
    wr_data8(0xfa);        // Sync.    - 1  1  1  1  1  0  1  0 RW=0 RS=1
    wr_data8(data & 0xf0); // HighData - D7 D6 D5 D4 0  0  0  0
    wr_data8(data << 4);   // LowData  - D3 D2 D1 D0 0  0  0  0
    wait_us(72);
}


// init the lcd controller
// init sequence is manufacturer specific
void ST7920::init()
{
    /* Start Initial Sequence ----------------------------------------------------*/
    wait_ms(40);

    // this is for a 128x64 LCD with SPI.
    // parallel-mode not implemented!
    
    //Function set [DL=1 8-bit interface; DL=0 4-bit interface;
    //              RE=1: extended instruction; RE=0: basic instruction]
    // RS RW DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
    //  0  0   0   0   1  DL   X  RE   0   0
    // DisplayClear
    wir(0x01);
    // ReturnHome
    wir(0x02);
    // FunctionSet: BasicIR
    wir(0x30);
    // FunctionSet: ExtendedIR
    //wir(0x34);

    //Function set [DL=1 8-bit interface; DL=0 4-bit interface;
    //              RE=1: extended instruction; RE=0: basic instruction]
    // RS RW DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
    //  0  0   0   0   1  DL   X  RE   0   0
    //
    // ExtFunctionSet Display OFF
    //wir(0x30);

    // DisplayControl [D=1: Display ON; C=1: Cursor ON; B=1: Character Blink ON]
    // RS RW DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
    //  0  0   0   0   0   0   1   D   C   B
    // DisplayControl: Display ON CharacterBlink ON Cursor=On
    //wir(0x0f);
    // DisplayControl: Display ON CharacterBlink OFF Cursor=Off
    wir(0x0c);

    // Display Clear
    wir(0x01);
    //wait_ms(10);
    //Set cursor position and display shift when doing write or read operation
    // RS RW DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
    //  0  0   0   0   0   0   0   1 I/D   S
    // EntryModeSet: Increase, Shift right
    wir(0x06);

}

void ST7920::mirrorXY(mirror_t mode)
{
    //not supported by hardware
}

void ST7920::cls(void)
{
    //DisplayClear
    wir(0x01);
    //ReturnHome
    wir(0x02);
    
    memset(buffer,0x00,sizeX()*(sizeY()>>3));  // clear display buffer
}

// write the full screenbuffer to the LCD GDRAM
void ST7920::copy_to_lcd(void)
{
    int i,j;

    // Change to Extended InstructionSet
    //FunctionSet: Extended
    wir(0x36);
    for(i=0; i<32; i++) { //32 Doppelzeilen
        // SetGDRAM-Adress AC: 0
        //Vertical-Adr
        wir(0x80+i);
        //Horizontal-Adr
        wir(0x80);

        //write 16 Double-Bytes of data
        //upper part of Display
        for (j=0; j<8; j++) {
            wdr(buffer[(i*16) + (j*2)]);
            wdr(buffer[(i*16) + (j*2) +1]);
            //if (i==0) {wdr(0xAA); wdr(0xAA);}else {wdr(0x00); wdr(0x00);};
        }
        //lower part of Display
        for (j=0; j<8; j++) {
            wdr(buffer[(i*16)+512 + (j*2)]);
            wdr(buffer[(i*16)+512 + (j*2) +1]);
            //if (i==31) {wdr(0xAA); wdr(0xAA);}else {wdr(0x00); wdr(0x00);};
        }
        

        /*
        //obere Displayhälfte
        wdr(0xFF);
        wdr(0x00);
        wdr(0x00);
        wdr(0xAA);
        wdr(0xF0);
        wdr(0xAA);
        wdr(0xF0);
        wdr(0xAA);
        wdr(0xF0);
        wdr(0xAA);
        wdr(0xF0);
        wdr(0xAA);
        wdr(0xF0);
        wdr(0xAA);
        wdr(0xFF);

        //Zeile fertig

        // untere Displayhälfte

        wdr(0x00);
        wdr(0x00);
        wdr(0xFF);
        wdr(0xFF);
        wdr(0xAA);
        wdr(0xF0);
        wdr(0xAA);
        wdr(0xF0);
        wdr(0xAA);
        wdr(0xF0);
        wdr(0xAA);
        wdr(0xF0);
        wdr(0xAA);
        wdr(0xF0);
        wdr(0x00);
        wdr(0xFF);
        */
    }

    //back to BasicInstructionSet
    wir(0x30);

}

void ST7920::set_contrast(int o)
{
    //not supported by hardware
}

void ST7920::invert(unsigned char o)
{
    //not supported by hardware
}

void ST7920::wr_grambuf(unsigned short* data, unsigned int lenght)
{
    // not needed, not supported
}

void ST7920::wr_gram(unsigned short data)
{
    // not needed, not supported
}

void ST7920::wr_gram(unsigned short data, unsigned int count)
{
    // not needed, not supported
}

void ST7920::wr_data16(unsigned short data)
{
    // not needed, not supported
}

void ST7920::wr_cmd16(unsigned short cmd)
{
    // not needed, not supported
}

void ST7920::wr_cmd8(unsigned char cmd)
{
    // not needed, not supported
}

unsigned short ST7920::rd_gram(bool convert)
{
    // not needed, not supported
    return(0);
}

unsigned int ST7920::rd_reg_data32(unsigned char reg)
{
    // not needed, not supported
    return(0);
}

unsigned int ST7920::rd_extcreg_data32(unsigned char reg, unsigned char SPIreadenablecmd)
{
    // not needed, not supported
    return(0);
}