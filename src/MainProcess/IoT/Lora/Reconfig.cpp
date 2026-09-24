/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note Reconfiguration process via LoRa
 */

#include <Headerfile.h>

/**********************
 * RECONFIGURATION PROCESS
 * Processes incoming reconfiguration commands via LoRa
 **********************/
void LoRaFunction::ReconfigProcessing(String payload)
{
  if (payload.startsWith("*RECONFIG,") && payload.endsWith("#"))
  {
    // Remove "*RECONFIG," from the beginning and "#" from the end
    String content = payload.substring(10, payload.length() - 1);

    int commaPos = content.indexOf(',');

    if (commaPos != -1)
    {
      String outletStr = content.substring(0, commaPos);
      String capacityStr = content.substring(commaPos + 1);

      int oldReservoirCapacity = GreaseOut.Reservoir.Capacity; // Store old reservoir capacity
      Config.NoOfOutlet = outletStr.toInt();
      GreaseOut.Reservoir.Capacity = capacityStr.toInt();

      LoRa.response(LoraTopics.Reconfig_return, Response.Ok);

      if (oldReservoirCapacity != GreaseOut.Reservoir.Capacity)
      {
        ReconfigReset();
      }
    }
    else
    {
      LoRa.response(LoraTopics.Reconfig_return, Response.Error);
    }
  }
}

/**********************
 * RECONFIGURATION PROCESS
 * Handles incoming reconfiguration LoRa messages
 **********************/
void _reconfig_process(String payload)
{
  LoRa.ReconfigProcessing(payload); // Process the reconfiguration command
}

void LoRaFunction::ReconfigReset()
{
  GreaseOut.Proximity.Seconds = 0; // Reset proximity seconds

  GreaseOut.Lubrication.Temp.Grams = GreaseOut.Lubrication.Grams; // Update temporary lubrication grams
  GreaseOut.Lubrication.Live.Grams = GreaseOut.Lubrication.Grams; // Update live lubrication grams

  GreaseOut.PauseTime.Temp.Hour = GreaseOut.PauseTime.Hour; // Update temporary pause time hours
  GreaseOut.PauseTime.Live.Hour = GreaseOut.PauseTime.Hour; // Update live pause time hours

  GreaseOut.PauseTime.Temp.Minute = GreaseOut.PauseTime.Minute; // Update temporary pause time minutes
  GreaseOut.PauseTime.Live.Minute = GreaseOut.PauseTime.Minute; // Update live pause time minutes

  GreaseOut.TestRun.sec = TEST_RUN_SEC;
  GreaseOut.TestRun.min = TEST_RUN_MIN;

  vTaskDelay(pdMS_TO_TICKS(10));        // Yield to other tasks with a delay of 10 milliseconds
  GreaseOut.Lubrication.Enable = false; // Enable lubrication
  GreaseOut.PauseTime.Enable = false;   // Disable pause time
  GreaseOut.TestRun.Enable = false;     // Disable test run

  Alert.Enable = false; // Disable alerts
  SetAlertName("NO");
  Alert.NameCount = 0; // Reset alert name count
  Alert.Total.enable = true;
  MenuResetnotCompleted = false; // Reset menu reset not completed flag
  MenuSettingEditTrue = false;   // Reset menu setting edit true flag

  TempTankCapcity = 0;                // Reset temporary tank capacity
  GreaseOut.Reservoir.Percentage = 0; // Set reservoir percentage to 0%
  GreaseOut.Reservoir.Temp.Percentage = 0;
  GreaseOut.Reservoir.Temp._old_reservoir_value = -1;

  AlertFun.reservoir(); // Trigger reservoir low level alert

  Eeprom.write(); // Write to EEPROM
}