#include "Protocols.h"
#include "ILI9341.h"

//////////////////////////////////////////////////////////////////////////////////
// display settings ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

#define LCDSIZE_X       240 // display X pixels, TFTs are usually portrait view
#define LCDSIZE_Y       320  // display Y pixels



ILI9341::ILI9341(proto_t displayproto, PortName port, PinName CS, PinName reset, PinName DC, PinName WR, PinName RD, const char *name)
    : TFT(displayproto, port, CS, reset, DC, WR, RD, LCDSIZE_X, LCDSIZE_Y, name)
{
    hw_reset();
    BusEnable(true);
    init();
    cls();
    set_orientation(0);
    locate(0,0); 
}
ILI9341::ILI9341(proto_t displayproto, int Hz, PinName mosi, PinName miso, PinName sclk, PinName CS, PinName reset, PinName DC, const char *name)
    : TFT(displayproto, Hz, mosi, miso, sclk, CS, reset, DC, LCDSIZE_X, LCDSIZE_Y, name)
{
    hw_reset(); //TFT class forwards to Protocol class
    BusEnable(true); //TFT class forwards to Protocol class
    init(); // per display custom init cmd sequence, implemented here
    cls();
    set_orientation(1); //TFT class does for MIPI standard and some ILIxxx
    locate(0,0); 
}
// reset and init the lcd controller
void ILI9341::init()
{
    /* Start Initial Sequence ----------------------------------------------------*/
    
    wr_cmd8(0xCB);  // POWER_ON_SEQ_CONTROL             
     wr_data8(0x39);
     wr_data8(0x2C);
     wr_data8(0x00);
     wr_data8(0x34);
     wr_data8(0x02);
     
    wr_cmd8(0xCF);  // POWER_CONTROL_B              
     wr_data8(0x00);
     wr_data8(0xC1);  // Applic Notes 81, was 83, C1 enables PCEQ: PC and EQ operation for power saving
     wr_data8(0x30);
     
     wr_cmd8(0xE8);  // DRIVER_TIMING_CONTROL_A               
     wr_data8(0x85);
     wr_data8(0x00);  // AN 10, was 01
     wr_data8(0x78);  // AN 7A, was 79
     
     wr_cmd8(0xEA);  // DRIVER_TIMING_CONTROL_B                    
     wr_data8(0x00);
     wr_data8(0x00);
     
     wr_cmd8(0xED);                     
     wr_data8(0x64);
     wr_data8(0x03);
     wr_data8(0x12);
     wr_data8(0x81);
     
     wr_cmd8(0xF7);  // PUMP_RATIO_CONTROL                   
     wr_data8(0x20);
     
     wr_cmd8(0xC0);                     // POWER_CONTROL_1
     wr_data8(0x23);  // AN 21, was 26
     
     wr_cmd8(0xC1);                     // POWER_CONTROL_2
     wr_data8(0x10);  // AN 11, was 11
     
     wr_cmd8(0xC5);                     // VCOM_CONTROL_1
     wr_data8(0x3E);  // AN 3F, was 35
     wr_data8(0x28);  // AN 3C, was 3E
     
     wr_cmd8(0xC7);                     // VCOM_CONTROL_2
     wr_data8(0x86);  // AN A7, was BE
     
     wr_cmd8(0x36);                     // MEMORY_ACCESS_CONTROL
     wr_data8(0x48);
     
     wr_cmd8(0x3A);                     // COLMOD_PIXEL_FORMAT_SET, not present in AN
     wr_data8(0x55);                 // 16 bit pixel 
     
     wr_cmd8(0xB1);                     // Frame Rate
     wr_data8(0x00);
     wr_data8(0x18);  // AN 1B, was 1B  1B=70hz             
     
     wr_cmd8(0xB6);                       // display function control, INTERESTING
     wr_data8(0x08);  // AN 0A, was 0A
     wr_data8(0x82);  // AN A2
     wr_data8(0x27);  // AN not present
  //   wr_data8(0x00);  // was present
     
     wr_cmd8(0xF2);                     // Gamma Function Disable
     wr_data8(0x00);  // AN 00, was 08
     
     wr_cmd8(0x26);                     
     wr_data8(0x01);                 // gamma set for curve 01/2/04/08
     
     wr_cmd8(0xE0);                     // positive gamma correction
     wr_data8(0x0F); 
     wr_data8(0x31); 
     wr_data8(0x2B); 
     wr_data8(0x0C); 
     wr_data8(0x0E); 
     wr_data8(0x08); 
     wr_data8(0x4E); 
     wr_data8(0xF1); 
     wr_data8(0x37); 
     wr_data8(0x07); 
     wr_data8(0x10); 
     wr_data8(0x03); 
     wr_data8(0x0E);
     wr_data8(0x09); 
     wr_data8(0x00);
     
     wr_cmd8(0xE1);                     // negativ gamma correction
     wr_data8(0x00); 
     wr_data8(0x0E); 
     wr_data8(0x14); 
     wr_data8(0x03); 
     wr_data8(0x11); 
     wr_data8(0x07); 
     wr_data8(0x31); 
     wr_data8(0xC1); 
     wr_data8(0x48); 
     wr_data8(0x08); 
     wr_data8(0x0F); 
     wr_data8(0x0C); 
     wr_data8(0x31);
     wr_data8(0x36); 
     wr_data8(0x0F);
     
     //wr_cmd8(0x34);                     // tearing effect off
     
     //wr_cmd8(0x35);                     // tearing effect on
      
  //   wr_cmd8(0xB7);                       // ENTRY_MODE_SET
  //   wr_data8(0x07);
         
     wr_cmd8(0x11);                     // sleep out
     
     wait_ms(150);
     
     wr_cmd8(0x29);                     // display on
     
     wait_ms(150);

}