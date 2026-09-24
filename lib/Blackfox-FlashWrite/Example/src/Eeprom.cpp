#include <Arduino.h>
#include "Eeprom.h"
#include "FlashWrite.h"
#include "System.h"

EEPROMFUNCTION EEPROM;

void EEPROMFUNCTION::begin(size_t size)
{
    Flash.begin(512);
}

void EEPROMFUNCTION::write()
{
    Flash.write(num1);
    Flash.write(num2);
    Flash.writeString(name);
    Flash.writeString(name2);
    Flash.write(num3);
    Flash.write(num4);
    Flash.write(Eeeprom_init);
    Flash.commit();
}

void EEPROMFUNCTION::read()
{
    Flash.read(num1);
    Flash.read(num2);
    Flash.readString(name);
    Flash.readString(name2);
    Flash.read(num3);
    Flash.read(num4);
    Flash.read(Eeeprom_init);
}

void EEPROMFUNCTION::init()
{
    if (!Eeeprom_init)
    {
        num1 = 0;
        num2 = 0;
        num3 = 0;
        num4 = 0;
        Eeeprom_init = true;
        EEPROM.write();
        Serial.println("EEPROM init");
    }
}