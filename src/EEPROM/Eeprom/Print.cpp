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
 * EEPROM Print Method
 * Display EEPROM contents for verification and troubleshooting
 **********************/
#define PRINT(name, value, type)                                      \
  if (strcmp(#type, "bool") == 0)                                     \
  {                                                                   \
    Serial.printf("%-30s %-20s\n", #name, formatBool(value).c_str()); \
  }                                                                   \
  else if (strcmp(#type, "size_t") == 0)                              \
  {                                                                   \
    Serial.printf("%-30s 0x%06X\n", #name, value);                    \
  }                                                                   \
  else                                                                \
  {                                                                   \
    Serial.printf("%-30s %-20s\n", #name, String(value).c_str());     \
  }

/**********************
 * EEPROM Print Method
 * Display EEPROM contents for verification and troubleshooting
 **********************/
void EEPROMFUNCTION::print()
{
  Serial.println("\n===== Flash Data Dump =====");                      // Header for flash data dump
  Serial.println("Variable Name   Value   Data Type");                  // Column headers
  Serial.println("--------------------------------------------------"); // Separator line

  // Helper function to format boolean values
  auto formatBool = [](bool value) -> String
  {
    return value ? "true" : "false"; // Return "true" or "false" as string
  };

  /**********************
   * LUBRICATION PARAMETERS DISPLAY
   * Show current lubrication configuration values
   **********************/
  Serial.println("\n===== LUBRICATION PARAMETERS =====");                   // Lubrication section header
  PRINT(GreaseOut.Lubrication.Grams, GreaseOut.Lubrication.Grams, float);   // Display grease quantity
  PRINT(GreaseOut.PauseTime.Hour, GreaseOut.PauseTime.Hour, int);           // Display pause hours
  PRINT(GreaseOut.PauseTime.Minute, GreaseOut.PauseTime.Minute, int);       // Display pause minutes
  PRINT(Config.NoOfOutlet, Config.NoOfOutlet, int);                         // Display outlet count
  PRINT(GreaseOut.Reservoir.Capacity, GreaseOut.Reservoir.Capacity, float); // Display reservoir capacity

  /**********************
   * TEST RUN CONFIGURATION DISPLAY
   * Show current test run settings
   **********************/
  Serial.println("\n===== TEST RUN CONFIG =====");                 // Test run section header
  PRINT(GreaseOut.TestRun.Enable, GreaseOut.TestRun.Enable, bool); // Display test run enable status

  /**********************
   * NETWORK CONFIGURATION DISPLAY
   * Show current LoRaWAN provisioning settings
   **********************/
  Serial.println("\n===== NETWORK CONFIGURATION =====");     // Network configuration header
  PRINT(LoraDetails.APPEUI, LoraDetails.APPEUI, String);     // Display LoRaWAN App EUI
  PRINT(LoraDetails.port, LoraDetails.port, int);            // Display LoRa port
  PRINT(LoraDetails.Clientid, LoraDetails.Clientid, String); // Display LoRa client ID
  PRINT(LoraDetails.DEVEUI, LoraDetails.DEVEUI, String);     // Display LoRaWAN Device EUI
  PRINT(LoraDetails.APPKEY, LoraDetails.APPKEY, String);     // Display LoRaWAN App Key
  PRINT(bluetooth.Received, bluetooth.Received, int);        // Display Bluetooth received count

  /**********************
   * RTC CONFIGURATION DISPLAY
   * Show current real-time clock settings
   **********************/
  Serial.println("\n===== RTC CONFIGURATION =====");     // RTC configuration header
  PRINT(RTC.Enable, RTC.Enable, bool);                   // Display RTC enable status
  PRINT(RTC.year, RTC.year, int);                        // Display RTC year
  PRINT(RTC.month, RTC.month, int);                      // Display RTC month
  PRINT(RTC.date, RTC.date, int);                        // Display RTC date
  PRINT(RTC.hour, RTC.hour, int);                        // Display RTC hour
  PRINT(RTC.minute, RTC.minute, int);                    // Display RTC minute
  PRINT(flash.LastDate, flash.LastDate, String);         // Display last recorded date
  PRINT(flash.PerverseDate, flash.PerverseDate, String); // Display previous date for comparison
  PRINT(bluetooth.com_data, bluetooth.com_data, String); // Display Bluetooth communication data
  Serial.println("===== End of Flash Data =====");
}
