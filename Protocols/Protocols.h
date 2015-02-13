/** @file Protocols.h
*/
#ifndef Protocols_H
#define Protocols_H

#include "mbed.h"

/** Protocol types
*/
enum proto_t {
    PAR_8   /**< Parallel 8bit, pins 0 to 7 */
    ,PAR_16 /**< Parallel 16bit, pins 0 to 15 */
    ,SPI_8  /**< SPI 8bit */
    ,SPI_16 /**< SPI 16bit */
};


/** Abstract class for spi and parallel protocols
*/
class Protocols
{
 public:
    
    /** Send 8bit command to display controller 
    *
    * @param cmd: byte to send  
    *
    */   
    virtual void wr_cmd8(unsigned char cmd) = 0;
    
    /** Send 8bit data to display controller 
    *
    * @param data: byte to send   
    *
    */   
    virtual void wr_data8(unsigned char data) = 0;
    
    /** Send same 8bit data to display controller multiple times
    *
    * @param data: byte to send
    * @param count: how many
    *
    */   
    virtual void wr_data8(unsigned char data, unsigned int count) = 0;
    
    /** Send array of data bytes to display controller
    *
    * @param data: unsigned char data array
    * @param lenght: lenght
    *
    */   
    virtual void wr_data8buf(unsigned char* data, unsigned int lenght) = 0;
    
    /** Send 16bit command to display controller 
    *
    * @param cmd: halfword to send  
    *
    */   
    virtual void wr_cmd16(unsigned short cmd) = 0;
    
    /** Send 16bit data to display controller 
    *
    * @param data: halfword to send   
    *
    */   
    virtual void wr_data16(unsigned short data) = 0;
    
    /** Send same 16bit data to display controller multiple times
    *
    * @param data: halfword to send
    * @param count: how many
    *
    */   
    virtual void wr_data16(unsigned short data, unsigned int count) = 0;
    
    /** Send array of data shorts to display controller
    *
    * @param data: unsigned short data array
    * @param lenght: lenght (in shorts)
    *
    */   
    virtual void wr_data16buf(unsigned short* data, unsigned int lenght) = 0;
    
    /** HW reset sequence (without display init commands)   
    */
    virtual void hw_reset() = 0;
    
    /** Set ChipSelect high or low
    * @param enable 0/1   
    */
    virtual void BusEnable(bool enable) = 0;

};
#endif