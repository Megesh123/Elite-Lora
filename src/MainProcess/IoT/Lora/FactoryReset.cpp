/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note Factory reset handling via LoRa
 */

#include "Headerfile.h"

/**********************
 * FACTORY RESET PROCESS
 * Handles factory reset commands via LoRa
 **********************/
void _factory_reset_process(String payload)
{
  if (payload.indexOf(PlayLoadMessage.FactoryReset) != -1) // Check if the payload contains the factory reset command
  {
    LoRa.response(LoraTopics.system_reset_return, Response.Ok); // Send OK response
    Alert.Enable = false;                                       // Disable any active alerts
    vTaskDelay(pdMS_TO_TICKS(100));                             // Short delay

    Led.off();                      // Turn off all LEDs
    Motor.off();                    // Turn off the motor
    Grease.stop();                  // Stop any grease dispensing operations
    vTaskDelay(pdMS_TO_TICKS(100)); // Short delay

    Config.NoOfOutlet = -1;         // Reset number of outlets
    Eeprom.write();                 // Write changes to EEPROM
    vTaskDelay(pdMS_TO_TICKS(100)); // Short delay
    ESP.restart();                  // Restart the system to complete the factory reset
  }
}