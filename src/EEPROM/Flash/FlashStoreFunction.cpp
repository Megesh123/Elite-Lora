/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note Flash storage management for data logging and LoRa transmission.
 */

/**********************
 * FLASH STORAGE MANAGEMENT IMPLEMENTATION
 * Handles data logging, storage maintenance, and flash memory operations
 * Implements circular buffer with 90-day retention and LoRa integration
 **********************/

#include "Headerfile.h" // Include main system header

char _data[DATA_SIZE]; // Buffer for data storage operations

/**********************
 * DATA WRITE OPERATION
 * Writes timestamped data to flash memory with storage management
 **********************/
void FLASHSTOREFUNCTION::write(String dataprocess) // Write data to flash with timestamp
{
  flash.Write = true;                                                // Set flash write flag
  Serial.printf("Writing data to flash: %s\n", dataprocess.c_str()); // Debug message
  _date_time_data_combined(dataprocess);                             // Combine timestamp with data
  _oldDataClear();                                                   // Clear old data if needed
  _write_in_flash();                                                 // Perform flash write operation
}

/**********************
 * LoRa DATA TRANSMISSION
 * Sends stored flash data via LoRa when connected
 **********************/
void FLASHSTOREFUNCTION::sendLora() // Send stored data via LoRa
{
  while (LoraDetails.Connected && flash.readAddr != flash.currentAddr && flash.DataReceived) // While data exists to send
  {
    _send_lora_data_read();       // Read and send data
    _delete_send_data_address();  // Remove addresses of sent data
    _send_lora_address_reset();   // Reset address pointer if needed
    vTaskDelay(pdMS_TO_TICKS(1)); // Yield to other tasks with a delay of 1 milliseconds
  }
}

/**********************
 * FLASH ERASE OPERATIONS
 * Erases specific regions of flash memory based on selection
 **********************/
void FLASHSTOREFUNCTION::erase(int select) // Erase specific flash regions
{
  if (select == 1) // Option 1: Erase EEPROM variable storage
  {
    _erase_for_eeprom_store_all_variable_data();
  }
  else if (select == 2) // Option 2: Erase old address data
  {
    _erase_remaining_90_day_address_data();
  }
  else if (select == 3) // Option 3: Erase date/address storage
  {
    _erase_for_eeprom_store_date_address_data();
  }
  else if (select == 4) // Option 4: Erase all flash data
  {
    _erase_all_address_to_begin();
  }
}

/**********************
 * DATE-TIME DATA COMBINATION
 * Creates timestamped data string for storage
 **********************/
void FLASHSTOREFUNCTION::_date_time_data_combined(String data) // Combine timestamp with data
{
  DateTime now = rtc.getDateTime();                                    // Get current date and time from RTC
  snprintf(_data, sizeof(_data), "%02d:%02d:%04d, %02d:%02d:%02d, %s", // Format timestamp and data (bounded)
           now.date, now.month, now.year, now.hour, now.minute, now.second, data.c_str());
}

/**********************
 * OLD DATA CLEARANCE
 * Clears old data when flash storage reaches capacity
 **********************/
void FLASHSTOREFUNCTION::_oldDataClear() // Clear old data when storage is full
{
  if (flash.currentAddr == FLASH_ADDR_START) // Check if at start address (wrapped around)
  {
    // Log.debug(" Clear the Old Data in Flash");
    Flash.erase(2); // Erase remaining old data
  }
}

/**********************
 * FLASH WRITE OPERATION
 * Performs actual write operation to flash memory
 **********************/
void FLASHSTOREFUNCTION::_write_in_flash() // Perform actual flash write
{
  esp_err_t ret = esp_flash_write(esp_flash_default_chip, _data, flash.currentAddr, DATA_SIZE); // Write to flash
  if (ret == ESP_OK)                                                                            // Check if write successful
  {
    // Log.info("Write OK @ 0x%06X: %s \n", flash.currentAddr, _data); // Debug message
    flash.currentAddr += DATA_SIZE; // Increment write address
    _setLastDate();                 // Update date tracking
  }
  else // If write failed
  {
    // Log.error("Write failed @ 0x%06X: %s\n", flash.currentAddr, esp_err_to_name(ret)); // Error message
    _reset(); // Reset flash system
  }
}

/**********************
 * FLASH SYSTEM RESET
 * Resets flash pointers when storage limits are reached
 **********************/
void FLASHSTOREFUNCTION::_reset() // Reset flash system
{
  if (flash.currentAddr >= FLASH_ADDR_END) // Check if reached end of flash
  {
    // Log.debug("Flash storage limit reached!"); // Warning message
    Flash.erase(2);               // Erase old data
    vTaskDelay(pdMS_TO_TICKS(1)); // Yield to other tasks with a delay of 1 milliseconds
  }
}

/**********************
 * DATE COMBINATION
 * Combines old and new dates for tracking
 **********************/
void FLASHSTOREFUNCTION::_oldDate_newDate_combined() // Combine old and new dates
{
  if (flash.StartDate.length() > 0) // Check if existing dates exist
  {
    flash.StartDate += ","; // Add comma separator
  }
  flash.StartDate += flash.PerverseDate; // Append new date
}

/**********************
 * ADDRESS COMBINATION
 * Combines old and new addresses for tracking
 **********************/
void FLASHSTOREFUNCTION::_oldAddres_newAddress_combined() // Combine old and new addresses
{
  if (flash.StartAddress.length() > 0) // Check if existing addresses exist
  {
    flash.StartAddress += ","; // Add comma separator
  }
  flash.StartAddress += String(flash.currentAddr, HEX); // Append new address in hex
}

/**********************
 * LAST DATE SETTING
 * Sets and tracks the last written date
 **********************/
void FLASHSTOREFUNCTION::_setLastDate() // Set last written date
{
  DateTime now = rtc.getDateTime();                                                  // Get current date from RTC
  char buffer[20];                                                                   // Buffer for date string
  snprintf(buffer, sizeof(buffer), "%02d:%02d:%04d", now.date, now.month, now.year); // Format date (bounded)
  flash.LastDate = buffer;                                                           // Store last date
  _startDateChange();                                                                // Update date tracking system
}

/**********************
 * DATE TRACKING UPDATE
 * Updates the date tracking system when date changes
 **********************/
void FLASHSTOREFUNCTION::_startDateChange() // Update date tracking
{
  if (flash.PerverseDate != flash.LastDate) // Check if date changed
  {
    flash.PerverseDate = flash.LastDate;         // Update previous date
    _oldDate_newDate_combined();                 // Combine with existing dates
    _oldAddres_newAddress_combined();            // Combine with existing addresses
    _90_date_to_maintain(flash.StartDate);       // Maintain 90-day limit for dates
    _90_address_to_maintain(flash.StartAddress); // Maintain 90-day limit for addresses

    Eeprom.addressWrite(); // Write updated dates/addresses to EEPROM
  }
}

/**********************
 * DAY COUNT CALCULATION
 * Counts the number of days in a date string
 **********************/
int FLASHSTOREFUNCTION::_countDays(String &inputString) // Count days in date string
{
  if (inputString.length() == 0) // Check if string is empty
  {
    return 0; // Return zero days
  }
  int count = 1;                                 // Start with first day
  for (int i = 0; i < inputString.length(); i++) // Loop through string
  {
    if (inputString.charAt(i) == ',') // Count comma separators
    {
      count++; // Increment day count
    }
  }
  return count; // Return total day count
}

/**********************
 * 90-DAY DATE MAINTENANCE
 * Ensures only 90 days of dates are maintained
 **********************/
void FLASHSTOREFUNCTION::_90_date_to_maintain(String &inputString) // Maintain 90-day date limit
{
  int currentCount = _countDays(inputString); // Count current days

  if (currentCount > MAX_DAYS_STORAGE) // Check if exceeds 90 days
  {
    int firstComma = inputString.indexOf(','); // Find first comma
    if (firstComma != -1)                      // If comma found
    {
      inputString = inputString.substring(firstComma + 1); // Remove oldest date
    }
    else // If no commas (single date)
    {
      inputString = ""; // Clear string
    }
  }
}

/**********************
 * 90-DAY ADDRESS MAINTENANCE
 * Ensures only 90 days of addresses are maintained
 **********************/
void FLASHSTOREFUNCTION::_90_address_to_maintain(String &inputString) // Maintain 90-day address limit
{
  int currentCount = _countDays(inputString); // Count current addresses

  if (currentCount > MAX_DAYS_STORAGE) // Check if exceeds 90 days
  {
    int firstComma = inputString.indexOf(','); // Find first comma
    if (firstComma != -1)                      // If comma found
    {
      inputString = inputString.substring(firstComma + 1); // Remove oldest address
    }
    else // If no commas (single address)
    {
      inputString = ""; // Clear string
    }
  }
  _to_change_the_start_address(inputString); // Update start address
}

/**********************
 * START ADDRESS UPDATE
 * Updates the read start address based on maintained addresses
 **********************/
void FLASHSTOREFUNCTION::_to_change_the_start_address(String inputString) // Update start address
{
  size_t temp_readAddr = flash.readAddr; // Store current read address
  if (inputString.indexOf(',') == -1)    // Check if single address
  {
    flash.readAddr = strtoul(inputString.c_str(), NULL, 16); // Convert to integer
  }
  else // If multiple addresses
  {
    String firstAddr = inputString.substring(0, inputString.indexOf(',')); // Extract first address
    flash.readAddr = strtoul(firstAddr.c_str(), NULL, 16);                 // Convert to integer
  }
  // Log.debug("readAddr well changed \n old Addr 0x%06X \n new Addr 0x%06X \n", temp_readAddr, flash.readAddr); // Debug message
}

/**********************
 * FIRST ADDRESS EXTRACTION
 * Extracts the first address from a comma-separated string
 **********************/
void FLASHSTOREFUNCTION::_takeFirstAddr(String &inputString) // Extract first address
{
  int firstComma = inputString.indexOf(','); // Find first comma

  if (firstComma == -1) // If no commas (single address)
  {
    flash.TakeAddr = inputString; // Use entire string
  }
  else // If multiple addresses
  {
    flash.TakeAddr = inputString.substring(0, firstComma); // Extract first address
    // Log.debug("Extracted address  0x%06X \n", flash.TakeAddr); // Debug message
  }
}

/**********************
 * EEPROM VARIABLE DATA ERASE
 * Erases the flash region used for EEPROM variable storage
 **********************/
void FLASHSTOREFUNCTION::_erase_for_eeprom_store_all_variable_data() // Erase EEPROM variable storage
{
  size_t bytes_to_erase = FLASH_STORAGE_ADDR + FLASH_EEPROM_TOTAL_CHAR - FLASH_STORAGE_ADDR; // Calculate bytes to erase
  size_t sectors_to_erase = (bytes_to_erase + 4095) / 4096;                                  // Calculate sectors to erase

  if (sectors_to_erase == 0) // Check if nothing to erase
  {
    // Log.debug("Nothing to erase - no data written yet 1"); // Debug message
    return; // Exit function
  }

  esp_err_t ret1 = esp_flash_erase_region(esp_flash_default_chip, FLASH_STORAGE_ADDR, sectors_to_erase * 4096); // Erase region

  if (ret1 != ESP_OK) // Check if erase failed
  {
    // Log.error("Erase failed 1: %s\n", esp_err_to_name(ret1)); // Error message
  }
}

/**********************
 * OLD ADDRESS DATA ERASE
 * Erases flash regions containing old address data
 **********************/
void FLASHSTOREFUNCTION::_erase_remaining_90_day_address_data() // Erase old address data
{
  // Log.debug("\nErasing flash from start address...");           // Debug message
  _takeFirstAddr(flash.StartAddress);                           // Extract first address
  flash.readAddr = strtoul(flash.TakeAddr.c_str(), NULL, 16);   // Convert to integer
  size_t TakeAddr1 = strtoul(flash.TakeAddr.c_str(), NULL, 16); // Convert to integer
  size_t bytes_to_erase = TakeAddr1 - FLASH_ADDR_START;         // Calculate bytes to erase
  size_t sectors_to_erase = (bytes_to_erase + 4095) / 4096;     // Calculate sectors to erase

  if (sectors_to_erase == 0) // Check if nothing to erase
  {
    // Log.debug("Nothing to erase - no data written yet 2"); // Debug message
    return; // Exit function
  }

  esp_err_t ret = esp_flash_erase_region(esp_flash_default_chip, FLASH_ADDR_START, sectors_to_erase * 4096); // Erase region

  if (ret == ESP_OK) // Check if erase successful
  {
    // Log.debug("Successfully erased %d sectors (total %d bytes)\n", sectors_to_erase, sectors_to_erase * 4096); // Debug message
    // Log.debug("Addr: 0x%06X\n ", flash.currentAddr);                                                           // Debug message
    flash.currentAddr = FLASH_ADDR_START; // Reset write address
  }
  else // If erase failed
  {
    // Log.error("Erase failed 2: %s\n", esp_err_to_name(ret)); // Error message
  }
}

/**********************
 * DATE/ADDRESS STORAGE ERASE
 * Erases flash region used for date and address storage
 **********************/
void FLASHSTOREFUNCTION::_erase_for_eeprom_store_date_address_data() // Erase date/address storage
{
  size_t bytes_to_erase = FLASH_DATE_START_ADDR + FLASH_DATE_STORAGE_TOTAL_CHAR - FLASH_DATE_START_ADDR; // Calculate bytes
  size_t sectors_to_erase = (bytes_to_erase + 4095) / 4096;                                              // Calculate sectors

  if (sectors_to_erase == 0) // Check if nothing to erase
  {
    // Log.debug("Nothing to erase - no data written yet  3"); // Debug message
    return; // Exit function
  }

  esp_err_t ret1 = esp_flash_erase_region(esp_flash_default_chip, FLASH_DATE_START_ADDR, sectors_to_erase * 4096); // Erase region

  if (ret1 != ESP_OK) // Check if erase failed
  {
    // Log.error("Erase failed 3: %s\n", esp_err_to_name(ret1)); // Error message
  }
}

/**********************
 * COMPLETE FLASH ERASE
 * Erases all flash data from storage beginning
 **********************/
void FLASHSTOREFUNCTION::_erase_all_address_to_begin() // Erase all flash data
{
  size_t bytes_to_erase = FLASH_ADDR_END - FLASH_STORAGE_ADDR; // Calculate total bytes
  size_t sectors_to_erase = (bytes_to_erase + 4095) / 4096;    // Calculate total sectors

  if (sectors_to_erase == 0) // Check if nothing to erase
  {
    // Log.debug("Nothing to erase - no data written yet  4"); // Debug message
    return; // Exit function
  }

  esp_err_t ret1 = esp_flash_erase_region(esp_flash_default_chip, FLASH_STORAGE_ADDR, sectors_to_erase * 4096); // Erase all

  if (ret1 != ESP_OK) // Check if erase failed
  {
    // Log.error("Erase failed 4: %s\n", esp_err_to_name(ret1)); // Error message
  }
}

/**********************
 * LoRa DATA READING
 * Reads data from flash and sends via LoRa
 **********************/
void FLASHSTOREFUNCTION::_send_lora_data_read() // Read and send data via LoRa
{
  char buffer[100];                                                                          // Buffer for data reading
  esp_err_t ret = esp_flash_read(esp_flash_default_chip, buffer, flash.readAddr, DATA_SIZE); // Read from flash

  if (ret == ESP_OK) // Check if read successful
  {
    flash.DataReceived = false;  // Reset data confirm flag
    flash.readAddr += DATA_SIZE; // Increment read address
    // Log.info("0x%06X: %.*s\n", flash.readAddr, DATA_SIZE, buffer); // Debug message
    LoRa.flashData(buffer); // Send data via LoRa

    LoRa.offlineDataStop(); // Stop offline data mode
  }
  else // If read failed
  {
    // Log.error("Read failed @ 0x%06X: %s\n", flash.readAddr, esp_err_to_name(ret)); // Error message
  }
}

/**********************
 * LoRa ADDRESS RESET
 * Resets read address when end of flash is reached
 **********************/
void FLASHSTOREFUNCTION::_send_lora_address_reset() // Reset LoRa address pointer
{
  if (flash.readAddr > FLASH_ADDR_END) // Check if beyond flash end
  {
    flash.readAddr = FLASH_ADDR_START; // Reset to start address
    // Log.debug("Addr Change Successfully now readAddr  0x%06X \n", flash.readAddr); // Debug message
  }
}

/**********************
 * SENT DATA ADDRESS REMOVAL
 * Removes addresses of data that has been sent via LoRa
 **********************/
void FLASHSTOREFUNCTION::_delete_send_data_address() // Remove sent data addresses
{
  if (flash.StartAddress.length() > 0) // Check if addresses exist
  {
    // Extract first address from flash.StartAddress
    int firstComma = flash.StartAddress.indexOf(',');                                                            // Find first comma
    String firstAddrStr = (firstComma == -1) ? flash.StartAddress : flash.StartAddress.substring(0, firstComma); // Extract first address

    // Convert to numerical value
    size_t firstAddr = strtoul(firstAddrStr.c_str(), NULL, 16); // Convert to integer

    // Check if readAddr has passed this address
    if (flash.readAddr > firstAddr) // If data has been sent
    {
      // Remove first address
      if (firstComma == -1) // If single address
      {
        flash.StartAddress = ""; // Clear addresses
      }
      else // If multiple addresses
      {
        flash.StartAddress = flash.StartAddress.substring(firstComma + 1); // Remove first address
      }

      // Remove corresponding first date
      if (flash.StartDate.length() > 0) // Check if dates exist
      {
        firstComma = flash.StartDate.indexOf(','); // Find first comma
        if (firstComma == -1)                      // If single date
        {
          flash.StartDate = ""; // Clear dates
        }
        else // If multiple dates
        {
          flash.StartDate = flash.StartDate.substring(firstComma + 1); // Remove first date
        }
      }
      // Log.debug("flash.StartDate :  0x%06X \n", flash.StartDate);             // Debug message
      // Log.debug("flash.StartAddress :  0x%06X \n", flash.StartAddress);       // Debug message
      // Log.debug("Removed oldest address and date as readAddr has passed it"); // Debug message
    }
  }
}

void FLASHSTOREFUNCTION::SerialPrintFlashData()
{
  size_t _temp_readAddr = flash.readAddr;
  while (_temp_readAddr != flash.currentAddr) // Check if all data sent
  {
    char buffer[100];                                                                          // Buffer for data reading
    esp_err_t ret = esp_flash_read(esp_flash_default_chip, buffer, _temp_readAddr, DATA_SIZE); // Read from flash

    if (ret == ESP_OK) // Check if read successful
    {
      flash.DataReceived = false;  // Reset data confirm flag
      _temp_readAddr += DATA_SIZE; // Increment read address
      // Log.info("0x%06X: %.*s\n", _temp_readAddr, DATA_SIZE, buffer); // Debug message
    }
    else // If read failed
    {
      // Log.error("Read failed @ 0x%06X: %s\n", _temp_readAddr, esp_err_to_name(ret)); // Error message
    }
    if (_temp_readAddr > FLASH_ADDR_END) // Check if beyond flash end
    {
      _temp_readAddr = FLASH_ADDR_START; // Reset to start address
      // Log.debug("Addr Change Successfully now readAddr  0x%06X \n", _temp_readAddr); // Debug message
    }
    vTaskDelay(pdMS_TO_TICKS(1)); // Yield to other tasks with a delay of 1 milliseconds
  }
}