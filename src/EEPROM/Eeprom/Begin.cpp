/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note EEPROM emulation using flash memory for non-volatile data storage.
 */

#include "Headerfile.h" // Include main system header
#include "FlashWrite1.h"

/**********************
 * EEPROM Begin Method
 * Initializes flash memory for EEPROM emulation operations
 **********************/
void EEPROMFUNCTION::begin()
{
  Flash1.begin(2048); // Flash-emulated EEPROM buffer (payload + 12-byte integrity header)
}