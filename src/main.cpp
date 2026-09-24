/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note Main application file for Lubrication Control System
 */

#include "Headerfile.h"
#include "OTA.h"

// Define your OTA parameters
const char *versionURL = "https://raw.githubusercontent.com/Megesh123/memolub_link_pro_10v/main/Version.txt";
const char *firmwareURL = "https://raw.githubusercontent.com/Megesh123/memolub_link_pro_10v/main/firmware.bin";
const char *CURRENT_VERSION = CURRENT_VERSION_LINK_ULTRA_ELITE; // Current firmware version
// Create OTA instance
OTA_FUNCTION ota(CURRENT_VERSION, versionURL, firmwareURL);

/**********************
 * SETUP FUNCTION
 * Initializes system components and logging
 **********************/

void setup()
{
  // Log.begin();   // Initialize logging system
  Serial.begin(115200);
  delay(10);

  Serial.printf(
      "\nReset reason: %d\n",
      esp_reset_reason());

  System.init(); // Initialize system components
}

/**********************
 * LOOP FUNCTION
 * Main execution loop for the application
 **********************/
void loop()
{
  if (bluetooth.ModeCheck) // If in Bluetooth mode
  {
    BluetoothFun.Process(); // Handle Bluetooth operations
  }
}
