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
    
    /** Send command byte to display controller 
    *
    * @param cmd: byte to send  
    *
    */   
    virtual void wr_cmd(unsigned char cmd) = 0;
    
    /** Send data byte to display controller 
    *
    * @param data8: byte to send   
    *
    */   
    virtual void wr_data8(unsigned char data8) = 0;
    
    /** Send same data byte to display controller multiple times
    *
    * @param data8: byte to send
    * @param count: how many
    *
    */   
    virtual void wr_data8(unsigned char data8, unsigned int count) = 0;
    
    /** Send array of data bytes to display controller
    *
    * @param data8: unsigned char data array
    * @param lenght: lenght of array
    *
    */   
    virtual void wr_data8buf(unsigned char* data8, unsigned int lenght) = 0;
    
    /** HW reset sequence (without display init commands)   
    */
    virtual void hw_reset() = 0;
    
    /** Set ChipSelect high or low
    * @param enable 0/1   
    */
    virtual void BusEnable(bool enable) = 0;

};
#endif