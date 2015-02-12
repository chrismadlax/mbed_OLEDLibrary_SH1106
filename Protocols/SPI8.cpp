#include "SPI8.h"

//#define USE_CS

SPI8::SPI8(PinName mosi, PinName miso, PinName sclk, PinName CS, PinName reset, PinName DC)
    : _CS(CS), _spi(mosi, miso, sclk), _reset(reset), _DC(DC)
{
    _reset = 1;
    _DC=1;
    _CS=1;
    _spi.format(8,0);                  // 8 bit spi mode 0
 //   _spi.frequency(12000000);          // 10 Mhz SPI clock, 12mhz for F411
    _spi.frequency(10000000);
    hw_reset();    
}

void SPI8::wr_cmd(unsigned char cmd)
{   
#ifdef USE_CS
    _CS = 0;
#endif    
    _DC.write(0); // 0=cmd
    _spi.write(cmd);      // write 8bit
#ifdef USE_CS
    _CS = 1;
#endif
}
void SPI8::wr_data8(unsigned char data8)
{
#ifdef USE_CS
    _CS = 0;
#endif
    _DC.write(1); // 1=data
    _spi.write(data8);    // write 8bit
#ifdef USE_CS
    _CS = 1;
#endif
}
void SPI8::wr_data8(unsigned char data8, unsigned int count)
{
#ifdef USE_CS
    _CS = 0;
#endif
    _DC.write(1); // 1=data
    while(count)
    {
        _spi.write(data8);    // write 8bit
        count--;
    }
#ifdef USE_CS
    _CS = 1;
#endif
}
void SPI8::wr_data8buf(unsigned char* data8, unsigned int lenght)
{
#ifdef USE_CS
    _CS = 0;
#endif
    _DC.write(1); // 1=data
    while(lenght)
    {
        _spi.write(*data8++);    // write 8bit
       // data8++;
        lenght--;
    }
#ifdef USE_CS
    _CS = 1;
#endif
}
void SPI8::hw_reset()
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
void SPI8::BusEnable(bool enable)
{
    _CS = enable ? 0:1;
}