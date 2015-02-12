#include "PAR8.h"

PAR8::PAR8(PortName port, PinName CS, PinName reset, PinName DC, PinName WR, PinName RD)
    : _port(port,0xFF), _CS(CS), _reset(reset), _DC(DC), _WR(WR), _RD(RD)
{
    _reset = 1;
    _DC=1;
    _WR=1;
    _RD=1;
    _CS=1;
#ifdef STMPORTDEBUG
    findport(port); //on return, GPIO get disabled
#endif
    _port.mode(PullNone);
    _port.output(); // will re-enable our GPIO port
    hw_reset();    
}

#ifdef STMPORTDEBUG
// create a port obj with STM HAL drivers, just to collect memorymapped regs
void PAR8::findport(PortName port)
{
    port_t tempport;
    port_init(&tempport, port, 0xFF, PIN_INPUT);
    outreg = tempport.reg_out;
    inreg = tempport.reg_in;
 //   debug("out 0x%.8X  in 0x%.8X\r\n", outreg, inreg);
}
#endif
void PAR8::wr_cmd(unsigned char cmd)
{   
#ifdef USE_CS
    _CS = 0;
#endif    
    _DC = 0; // 0=cmd
    _WR=0;
    _port.write(cmd);      // write 8bit
    _WR=1;
#ifdef USE_CS
    _CS = 1;
#endif
}
void PAR8::wr_data8(unsigned char data8)
{
#ifdef USE_CS
    _CS = 0;
#endif
    _DC = 1; // 1=data
    _WR=0;
    _port.write(data8);    // write 8bit
    _WR=1;
#ifdef USE_CS
    _CS = 1;
#endif
}
void PAR8::wr_data8(unsigned char data8, unsigned int count)
{
#ifdef USE_CS
    _CS = 0;
#endif
    _DC = 1; // 1=data
    _port.write(data8);    // write 8bit
    while(count)
    {
        _WR=0;
  //      _WR=0;
        _WR=1;
        count--;
    }
#ifdef USE_CS
    _CS = 1;
#endif
}
void PAR8::wr_data8buf(unsigned char* data8, unsigned int lenght)
{
#ifdef USE_CS
    _CS = 0;
#endif
    _DC = 1; // 1=data
    while(lenght)
    {
        _WR=0;
        _port.write(*data8++);    // write 8bit
        _WR=1;
        lenght--;
    }
#ifdef USE_CS
    _CS = 1;
#endif
}
void PAR8::hw_reset()
{
    wait_ms(15);
    _DC = 1;
    _CS = 1;
    _WR = 1;
    _RD = 1;
    _reset = 0;                        // display reset
    wait_us(50);
    _reset = 1;                       // end reset
    wait_ms(15);
#ifndef USE_CS
    _CS=0;      // put CS low now and forever
#endif
}
void PAR8::BusEnable(bool enable)
{
    _CS = enable ? 0:1;
}