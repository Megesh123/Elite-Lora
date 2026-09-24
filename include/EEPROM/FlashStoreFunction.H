/**********************
 * FLASH STORAGE MANAGEMENT HEADER
 * Defines interface for flash memory operations and data management
 * Handles data logging, storage maintenance, and LoRa data transmission
 **********************/

#ifndef __FLASH_STORE_FUNCTION_H__
#define __FLASH_STORE_FUNCTION_H__

#include <Arduino.h>

/**********************
 * FLASHSTOREFUNCTION CLASS
 * Encapsulates methods for flash memory data storage and management
 **********************/
class FLASHSTOREFUNCTION
{
public:
    void write(String dataprocess); // Write data to flash with timestamp
    void sendLora();                // Send stored data via LoRa
    void erase(int select);         // Erase specific flash regions
    void _startDateChange();        // Update date tracking system
    void SerialPrintFlashData();    // Print flash data for debugging

private:
    void _oldDataClear();                              // Clear old data when storage is full
    void _reset();                                     // Reset flash pointers when limits reached
    void _date_time_data_combined(String dataprocess); // Combine timestamp with data
    void _write_in_flash();                            // Perform actual flash write operation
    void _setLastDate();                               // Set and track the last written date
    int _countDays(String &inputString);               // Count days in date string
    void _oldDate_newDate_combined();                  // Combine old and new dates
    void _oldAddres_newAddress_combined();             // Combine old and new addresses

    void _90_date_to_maintain(String &inputString);        // Maintain 90-day date limit
    void _90_address_to_maintain(String &inputString);     // Maintain 90-day address limit
    void _to_change_the_start_address(String inputString); // Update start address

    void _takeFirstAddr(String &inputString); // Extract first address from string

    // Flash erase operations
    void _erase_for_eeprom_store_all_variable_data(); // Erase EEPROM variable storage
    void _erase_remaining_90_day_address_data();      // Erase old address data
    void _erase_for_eeprom_store_date_address_data(); // Erase date/address storage
    void _erase_all_address_to_begin();               // Erase all flash data

    // LoRa data handling
    void _delete_send_data_address(); // Remove sent data addresses
    void _send_lora_address_reset();  // Reset LoRa address pointer
    void _send_lora_data_read();      // Read and send data via LoRa
};

extern FLASHSTOREFUNCTION Flash; // Global flash storage instance

#endif