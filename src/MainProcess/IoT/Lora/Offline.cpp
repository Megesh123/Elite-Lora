/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note Offline LoRa data transmission functions
 */

#include "Headerfile.h"

/**********************
 * OFFLINE DATA START
 * Initiates offline data transmission via LoRa
//  **********************/
// void LoRaFunction::offlineDataStart()
// {
//   if (LoraDetails.Connected) // If LoRa connected
//   {
//     if (flash.Write && !LoraTime.datasend) // Check if offline data exists and not sent
//     {
//       uint32_t currentTime = millis();                                              // Get current time
//       if (currentTime - LoraTime.OfflineCheckTime >= LoraTime.OfflineCheckInterval) // Check transmission interval
//       {
//         Lora.subscribe(LoraTopics.offline_return);                    // Return topic for offline status
//         Lora.response(LoraTopics.offline, Response.OfflineDataStart); // Send offline data start response
//         LoraTime.OfflineCheckTime = currentTime;                      // Update check time
//       }
//     }
//     if (flash.DataReceived) // Check if flash data received confirmation
//     {
//       Flash.sendLora(); // Send flash data via LoRa
//     }
//   }
// }

// /**********************
//  * OFFLINE DATA STOP
//  * Terminates offline data transmission via LoRa
//  **********************/
// void LoRaFunction::offlineDataStop()
// {
//   if (flash.readAddr == flash.currentAddr) // Check if all data sent
//   {
//     flash.StartDate = "";                                       // Clear start date
//     flash.StartAddress = "";                                    // Clear start address
//     Lora.response(LoraTopics.offline, Response.OfflineDataEnd); // Send offline data end response
//     flash.Write = false;                                        // Reset flash write flag
//   }
// }

// /**********************
//  * FLASH DATA TRANSMISSION
//  * Publishes flash data to LoRa topic
//  **********************/
// void LoRaFunction::flashData(String data)
// {
//   if (LoraDetails.Connected) // If LoRa connected
//   {
//     Lora.publish(LoraTopics.offline, data); // Publish flash data to offline topic
//   }
// }

// /**********************
//  * OFFLINE DATA PROCESSING
//  * Handles responses for offline data transmission
//  **********************/
// void _offline_data_process(String payload)
// {
//   if (payload.indexOf(PlayLoadMessage.Ok) != -1) // Check if the payload contains the OK response
//   {
//     flash.DataReceived = true; // Confirm flash data reception
//     LoraTime.datasend = true;  // Mark data as sent
//   }
// }