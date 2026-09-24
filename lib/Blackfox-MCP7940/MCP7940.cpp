#include "MCP7940.h"
#include <Arduino.h>

void MCP7940::setDateTime(bool &_isRtcInit, uint8_t _date, uint8_t _month, uint16_t _year, uint8_t _hour, uint8_t _minute, uint8_t _second, uint8_t _weekday)
{
    if (!_isRtcInit)
    {
        DateTime dt = {
            .second = _second,
            .minute = _minute,
            .hour = _hour,
            .date = _date,
            .month = _month,
            .year = _year,
            .weekday = _weekday};

        _enableOscillator();

        _wire.beginTransmission(_address);
        _wire.write(0x00);
        _wire.write(_decToBcd(dt.second) | 0x80);
        _wire.write(_decToBcd(dt.minute));
        _wire.write(_decToBcd(dt.hour));
        _wire.write((_decToBcd(dt.weekday) & 0x07) | 0x08);
        _wire.write(_decToBcd(dt.date));
        _wire.write(_decToBcd(dt.month));
        _wire.write(_decToBcd(dt.year % 100));
        _wire.endTransmission();
        
        _isRtcInit = true;
        Serial.println("setDateTime Success");
    }
}

DateTime MCP7940::getDateTime()
{
    DateTime dt;
    _wire.beginTransmission(_address);
    _wire.write(0x00);
    _wire.endTransmission();

    _wire.requestFrom((uint8_t)_address, (uint8_t)7);
    dt.second = _bcdToDec(_wire.read() & 0x7F);
    dt.minute = _bcdToDec(_wire.read());
    dt.hour = _bcdToDec(_wire.read() & 0x3F);
    dt.weekday = _bcdToDec(_wire.read() & 0x07);
    dt.date = _bcdToDec(_wire.read());
    dt.month = _bcdToDec(_wire.read() & 0x1F);
    dt.year = _bcdToDec(_wire.read()) + 2000;

    return dt;
}

// Private helpers
uint8_t MCP7940::_bcdToDec(uint8_t bcd)
{
    return (bcd >> 4) * 10 + (bcd & 0x0F);
}

uint8_t MCP7940::_decToBcd(uint8_t dec)
{
    return ((dec / 10) << 4) | (dec % 10);
}

void MCP7940::_enableOscillator()
{
    _wire.beginTransmission(_address);
    _wire.write(0x03);
    _wire.endTransmission();

    _wire.requestFrom((uint8_t)_address, (uint8_t)1);
    uint8_t wkday = _wire.read() | 0x08;

    _wire.beginTransmission(_address);
    _wire.write(0x03);
    _wire.write(wkday);
    _wire.endTransmission();
}

void MCP7940::printDateTime()
{
    DateTime dt = getDateTime();
    Serial.printf("%02d/%02d/%04d %02d:%02d:%02d\n",
                  dt.date, dt.month, dt.year,
                  dt.hour, dt.minute, dt.second);
}