/**
 * @file      MCP7940.h
 * @brief     Driver for MCP7940N RTC (I2C) with flexible address initialization
 */
#ifndef __MCP7940_H__
#define __MCP7940_H__

#include <Wire.h>

struct DateTime
{
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t date;
    uint8_t month;
    uint16_t year;
    uint8_t weekday;
};

class MCP7940
{
public:
    // Constructor with optional Wire interface
    MCP7940(TwoWire &wire = Wire) : _wire(wire), _address(0x6F) {}

    // Initialization methods
    void begin()
    {
        _wire.begin();
        printDateTime();

    } // Default address (0x6F)

    void begin(uint8_t address)
    {
        _address = address;
        _wire.begin();
    }

    // Core functionality
    void setDateTime(bool &_isRtcInit, uint8_t _date, uint8_t _month, uint16_t _year,
                     uint8_t _hour, uint8_t _minute, uint8_t _second,
                     uint8_t _weekday = 1);
    DateTime getDateTime(); // Returns DateTime struct
    void printDateTime();   // Just prints to Serial

private:
    TwoWire &_wire;
    uint8_t _address;

    // Helper methods
    uint8_t _bcdToDec(uint8_t bcd);
    uint8_t _decToBcd(uint8_t dec);
    void _enableOscillator();
};
#endif