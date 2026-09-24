/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note  Flash memory address management for EEPROM emulation.
 */



/**********************
 * EEPROM EMULATION IMPLEMENTATION
 * Provides flash-based storage for system configuration and operational data
 * Implements read/write operations with error checking and data validation
 **********************/

#include "Headerfile.h" // Include main system header
#include "FlashWrite1.h"


// Macro for reading strings from flash buffer
#define FLASH_READ_STRING(str, size)        \
  char str##_buf[size + 1];                 \
  memcpy(str##_buf, buffer + offset, size); \
  str##_buf[size] = '\0';                   \
  str = String(str##_buf);                  \
  offset += size

// Macro for writing strings to flash buffer with size protection
#define FLASH_WRITE_STRING(str, size)                      \
  memset(buffer + offset, 0, size);                        \
  strncpy((char *)buffer + offset, str.c_str(), size - 1); \
  offset += size

/**********************
 * ADDRESS WRITE FUNCTION
 * Writes date and address data to separate flash location
 **********************/
void EEPROMFUNCTION::addressWrite() // Write date and address data to flash
{
  unsigned long startTime = millis();  // Record start time
  esp_err_t ret1;                      // Error code variable
  size_t addr = FLASH_DATE_START_ADDR; // Date/address storage address

  size_t total_size = // Calculate total size
      TOTAL_DAYS_SIZE +
      TOTAL_ADDR_SIZE;

  total_size = (total_size + 3) & ~3; // Align size to 4-byte boundary

  uint8_t *buffer = (uint8_t *)malloc(total_size); // Allocate memory buffer
  if (!buffer)                                     // Check if allocation failed
  {
    Serial.println("Failed to allocate memory for flash write"); // Print error message
    return;                                                      // Exit function
  }

  size_t offset = 0; // Initialize buffer offset

  String &startDate = flash.StartDate;
  String &startAddress = flash.StartAddress;

  // Use macros with references
  FLASH_WRITE_STRING(startDate, TOTAL_DAYS_SIZE);
  FLASH_WRITE_STRING(startAddress, TOTAL_ADDR_SIZE);

  Flash.erase(3); // Erase flash sector before writing

  ret1 = esp_flash_write(esp_flash_default_chip, buffer, addr, total_size); // Write to flash
  free(buffer);                                                             // Free allocated memory

  if (ret1 != ESP_OK) // Check if write operation failed
  {
    Serial.printf("Flash write failed: 0x%x\n", ret1); // Print error code
    return;                                            // Exit function
  }
  // else // If write successful
  // {
  //   Serial.printf("Flash data and address write took %lu ms, wrote %d bytes\n", millis() - startTime, total_size); // Print success message
  // }
}

/**********************
 * ADDRESS READ FUNCTION
 * Reads date and address data from separate flash location
 **********************/
void EEPROMFUNCTION::addressRead() // Read date and address data from flash
{
  unsigned long startTime = millis();  // Record start time
  esp_err_t ret1;                      // Error code variable
  size_t addr = FLASH_DATE_START_ADDR; // Date/address storage address

  size_t total_size = // Calculate total size
      TOTAL_DAYS_SIZE +
      TOTAL_ADDR_SIZE;

  total_size = (total_size + 3) & ~3;              // Align size to 4-byte boundary
  uint8_t *buffer = (uint8_t *)malloc(total_size); // Allocate memory buffer
  if (!buffer)                                     // Check if allocation failed
  {
    Serial.println("Failed to allocate memory for flash read"); // Print error message
    return;                                                     // Exit function
  }

  ret1 = esp_flash_read(esp_flash_default_chip, buffer, addr, total_size); // Read from flash
  if (ret1 != ESP_OK)                                                      // Check if read operation failed
  {
    Serial.printf("Flash read failed: 0x%x\n", ret1); // Print error code
    free(buffer);                                     // Free allocated memory
    return;                                           // Exit function
  }

  size_t offset = 0; // Initialize buffer offset

  String &startDate = flash.StartDate;
  String &startAddress = flash.StartAddress;

  // Use macros with references
  FLASH_READ_STRING(startDate, TOTAL_DAYS_SIZE);
  FLASH_READ_STRING(startAddress, TOTAL_ADDR_SIZE);

  // Serial.print("StartDateTemp: ");
  // Serial.println(flash.StartDateTemp);

  // Serial.print("StartAddressTemp: ");
  // Serial.println(flash.StartAddressTemp);

  free(buffer); // Free allocated memory
}