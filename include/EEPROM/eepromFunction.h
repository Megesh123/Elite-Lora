/**********************
 * EEPROM EMULATION HEADER
 * Defines interface for flash-based EEPROM emulation
 * Handles persistent storage of system configuration and operational data
 **********************/

#ifndef __EEPROMFUNCTION_H__
#define __EEPROMFUNCTION_H__

/**********************
 * EEPROMFUNCTION CLASS
 * Encapsulates methods for EEPROM emulation using flash memory
 **********************/
class EEPROMFUNCTION
{
public:
    void init();  // Initialize EEPROM emulation system
    void begin(); // Prepare flash memory for EEPROM operations
    void write(); // Write all system configuration and operational data to flash memory
    void read();  // Read all system configuration and operational data from flash memory
    void print(); // Print current EEPROM emulation data for debugging

    /**********************
     * ADDRESS MANAGEMENT
     * Handle flash memory address tracking and management
     **********************/
    void addressWrite(); // Write current flash address information
    void addressRead();  // Read stored flash address information
};

extern EEPROMFUNCTION Eeprom; // Global EEPROM emulation instance

#endif /* __EEPROMFUNCTION_H__ */