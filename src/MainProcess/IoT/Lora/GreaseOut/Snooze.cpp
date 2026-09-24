/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note Snooze handling and LoRa notification functions
 */

#include "Headerfile.h"

/**********************
 * CONTROL SNOOZE FUNCTION
 * Handles snooze control messages
 **********************/
void _control_Snooze(String payload)
{
  if (payload.indexOf(PlayLoadMessage.Snooze) != -1) // If snooze message
  {
    LoRa.response(LoraTopics.Snooze_return, Response.Ok); // Send OK response
    Alert.SnoozeReservoir = true;                         // Enable reservoir snooze
    Alert.Enable = false;                                 // Disable alert
    SetAlertName("NO");                                   // Reset alert name
    AlertFun.clear();                                     // Clear alert
    Eeprom.write();                                       // Write to EEPROM
  }
  // else
  // {
  //   Lora.response(LoraTopics.Snooze_return, Response.Error); // Send ERROR response
  // }
}