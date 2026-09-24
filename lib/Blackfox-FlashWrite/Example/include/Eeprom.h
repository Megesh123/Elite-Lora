#ifndef __EEPROM_H__
#define __EEPROM_H__

#include <Arduino.h>

class EEPROMFUNCTION 
{
public:
    void begin(size_t size);
    void write();
    void read();
    void init();
};

extern EEPROMFUNCTION EEPROM;


#endif