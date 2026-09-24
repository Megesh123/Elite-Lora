/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note Refill handling and LoRa notification functions
 */

#include "Headerfile.h"

/**********************
 * REFILL START FUNCTION
 * Handles refill operation initiation
 **********************/
void LoRaFunction::refillStart()
{
  GreaseOut.Reservoir.Enable = true; // Enable refill mode
  Grease.stop();                     // Stop grease system
  Motor.toggle();                    // Set motor to intermediate speed
  ResetValue();                      // Reset operational values
}

void LoRaFunction::refillStop()
{
  Motor.off();                        // Turn off motor
  Grease.stop();                      // Stop grease system
  GreaseOut.Reservoir.Enable = false; // Disable refill mode
  ResetValue();                       // Reset operational values
}

/**********************
 * REFILL COMPLETE PROCESS
 * Handles refill completion messages
 **********************/
void _refill_complete_process(String payload)
{
  if (payload.indexOf(PlayLoadMessage.RefillSuccess) != -1) // If refill success message
  {
    LoRa.refillStop(); // Stop refill operation

    LoRa.response(LoraTopics.refill_return, Response.Ok); // Send OK response

    GreaseOut.Reservoir.Temp.Percentage = 0; // Reset temporary refill percentage
    GreaseOut.Reservoir.Percentage = 100;    // Set reservoir percentage to 100%
    Alert.Enable = false;

    Alert.SnoozeReservoir = false; // Enable reservoir snooze
    SetAlertName("");
    Alert.NameCount = 0;
    LoRa.ResetValue();
    LoRa.publish(LoraTopics.Reservoircapacity, String(GreaseOut.Reservoir.Percentage));

    if (start_init == 1)
    {
      Grease.lubricationStart(); // Start lubrication
    }

    Eeprom.write();
  }
}

/**********************
 * RESET VALUE FUNCTION
 * Resets operational values to defaults
 **********************/
void LoRaFunction::ResetValue()
{
  if (start_init == 1)
  {
    GreaseOut.Lubrication.Temp.Grams = GreaseOut.Lubrication.Grams; // Reset temporary grease quantity
    GreaseOut.PauseTime.Temp.Hour = GreaseOut.PauseTime.Hour;       // Reset temporary pause hours
    GreaseOut.PauseTime.Temp.Minute = GreaseOut.PauseTime.Minute;   // Reset temporary pause minutes
    GreaseOut.PauseTime.Seconds = 0;                                // Reset pause seconds

    GreaseOut.Lubrication.Live.Grams = GreaseOut.Lubrication.Temp.Grams; // Add grease quantity for next cycle
    GreaseOut.PauseTime.Seconds = 0;                                     // Set pause seconds counter
    GreaseOut.PauseTime.Live.Minute = GreaseOut.PauseTime.Minute;        // Set temporary pause minutes
    GreaseOut.PauseTime.Live.Hour = GreaseOut.PauseTime.Hour;            // Set temporary pause hours
  }
}

/**********************
 * REFILL MODE START/STOP PROCESS
 * Handles refill mode start/stop commands
 **********************/
void _refillMode_start_stop(String payload)
{
  if (payload.indexOf(PlayLoadMessage.RefillModeStart) != -1) // If refill mode start message
  {
    LoRa.refillStart();                                        // Start refill operation
    LoRa.response(LoraTopics.refill_mode_return, Response.Ok); // Send OK response
  }
  else if (payload.indexOf(PlayLoadMessage.RefillModeStop) != -1) // If refill mode stop message
  {
    LoRa.refillStop();
    LoRa.response(LoraTopics.refill_mode_return, Response.Ok); // Send OK response
  }
}

void _refill_alert_enable_disable(String payload)
{
  if (payload.indexOf("*REFILLALERT,ENABLE#") != -1) // If refill enable message
  {
    Alert.EnableDisable.RefillAlert = false;                   // Enable refill alert
    LoRa.response(LoraTopics.RefillAlert_return, Response.Ok); // Send OK response
  }
  else if (payload.indexOf("*REFILLALERT,DISABLE#") != -1) // If refill disable message
  {
    Alert.EnableDisable.RefillAlert = true; // Disable refill alert  TempTankCapcity = 0;                // Reset temporary tank capacity
    GreaseOut.Reservoir.Percentage = 0;     // Set reservoir percentage to 0%
    GreaseOut.Reservoir.Temp.Percentage = 0;
    GreaseOut.Reservoir.Temp._old_reservoir_value = -1;
    AlertFun.reservoir();                                      // Trigger reservoir low level alert
    LoRa.response(LoraTopics.RefillAlert_return, Response.Ok); // Send OK response
  }
}