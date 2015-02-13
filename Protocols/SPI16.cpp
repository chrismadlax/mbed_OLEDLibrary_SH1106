#include "SPI16.h"
//#define USE_CS

SPI16::SPI16(int Hz, PinName mosi, PinName miso, PinName sclk, PinName CS, PinName reset, PinName DC)
    : _CS(CS), _spi(mosi, miso, sclk), _reset(reset), _DC(DC)
{
    _reset = 1;
    _DC=1;
    _CS=1;
    _spi.format(16,0);                  // 8 bit spi mode 0
 //   _spi.frequency(12000000);          // 10 Mhz SPI clock, 12mhz for F411
    _spi.frequency(Hz);
    hw_reset();    
}

void SPI16::wr_cmd8(unsigned char cmd)
{   
#ifdef USE_CS
    _CS = 0;
#endif
    _spi.format(8,0); // it takes time, better use wr_cmd16 with NOP cmd
    _DC.write(0); // 0=cmd
    _spi.write(cmd);      // write 8bit
    _spi.format(16,0);
#ifdef USE_CS
    _CS = 1;
#endif
}
void SPI16::wr_data8(unsigned char data)
{
#ifdef USE_CS
    _CS = 0;
#endif
    _spi.format(8,0); // it takes time, check prev cmd parameter, in case use wr_data16 with repeated byte
    _DC.write(1); // 1=data
    _spi.write(data);    // write 8bit
    _spi.format(16,0);
#ifdef USE_CS
    _CS = 1;
#endif
}
void SPI16::wr_data8(unsigned char data, unsigned int count)
{
#ifdef USE_CS
    _CS = 0;
#endif
    _spi.format(8,0);
    _DC.write(1); // 1=data
    while(count)
    {
        _spi.write(data);    // write 8bit
        count--;
    }
    _spi.format(16,0);
#ifdef USE_CS
    _CS = 1;
#endif
}
void SPI16::wr_data8buf(unsigned char* data, unsigned int lenght)
{
#ifdef USE_CS
    _CS = 0;
#endif
    _spi.format(8,0);
    _DC.write(1); // 1=data
    while(lenght)
    {
        _spi.write(*data++);    // write 8bit
       // data++;
        lenght--;
    }
    _spi.format(16,0);
#ifdef USE_CS
    _CS = 1;
#endif
}
void SPI16::wr_cmd16(unsigned short cmd)
{   
#ifdef USE_CS
    _CS = 0;
#endif    
    _DC.write(0); // 0=cmd
    _spi.write(cmd);      // write 16bit
#ifdef USE_CS
    _CS = 1;
#endif
}
void SPI16::wr_data16(unsigned short data)
{
#ifdef USE_CS
    _CS = 0;
#endif
    _DC.write(1); // 1=data
    _spi.write(data);    // write 16bit
#ifdef USE_CS
    _CS = 1;
#endif
}
void SPI16::wr_data16(unsigned short data, unsigned int count)
{
#ifdef USE_CS
    _CS = 0;
#endif
    _DC.write(1); // 1=data
    while(count)
    {
        _spi.write(data);
        count--;
    }
#ifdef USE_CS
    _CS = 1;
#endif
}
void SPI16::wr_data16buf(unsigned short* data, unsigned int lenght)
{
#ifdef USE_CS
    _CS = 0;
#endif
    _DC.write(1); // 1=data
    while(lenght)
    {
        _spi.write(*data);
        data++;
        lenght--;
    }
#ifdef USE_CS
    _CS = 1;
#endif
}
void SPI16::hw_reset()
{
    wait_ms(15);
    _DC = 1;
 //   _CS = 1;
    _CS = 0;
    _reset = 0;                        // display reset
    wait_us(50);
    _reset = 1;                       // end reset
    wait_ms(15);
#ifndef USE_CS
    _CS=0;      // put CS low now and forever
#endif
}
void SPI16::BusEnable(bool enable)
{
    _CS = enable ? 0:1;
}