/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note Setting data processing via LoRa
 */

#include "Headerfile.h"

unsigned long _data_send = 0;         // stores last execution time
const unsigned long interval = 30000; // 30000 ms = 30 second

/**********************
 * SETTING DATA PROCESSING
 * Processes system configuration commands
 **********************/
void LoRaFunction::MessageProcessing(String payload)
{
  if (payload.startsWith("*S,") && payload.endsWith("#")) // Check for setting command
  {
    if (Config.NoOfOutlet > 0 && GreaseOut.Reservoir.Percentage > 2) //&& RefillPercentage > 2
    {
      LoRa.response(LoraTopics.subscribe_return, Response.Ok);
      SettingPageData(payload); // Process setting data
      Led.off();                // Save to EEPROM
      Supervisory._pump_status_send_lora = true;
      GreaseOut.Reservoir.Temp._old_reservoir_value = -1;
      LoRa.liveProcess_Lubrication(); // Send live data update

      GreaseOut.Proximity.Seconds = 0;
      Alert.Enable = false;
      SetAlertName("");
      Alert.NameCount = 0;
      Grease.lubricationStart(); // Start lubrication
      Motor.on();                // Turn on motor
      ResetValue();
      start_init = 1; // Set start initialization flag
      Eeprom.write();
    }
    else
    {
      LoRa.publish(LoraTopics.subscribe_return, Response.Error); // Publish error if conditions not met
    }
    // Reset operational values
  }
  else
  {
    LoRa.publish(LoraTopics.subscribe_return, Response.Error); // Publish error for invalid command format
  }
}

/**********************
 * SETTING PAGE DATA
 * Parses and applies configuration settings from LoRa payload
 **********************/
void LoRaFunction::SettingPageData(String payload)
{
  payload = payload.substring(2, payload.length() - 1); // Remove command envelope
  String data[9];                                       // Increased array size for additional parameters
  int index = 0;                                        // Parameter index

  while (payload.indexOf(COMMA) != -1 && index < 9) // Parse up to 9 parameters
  {
    int commaIndex = payload.indexOf(COMMA);         // Find comma position
    String value = payload.substring(0, commaIndex); // Extract parameter value
    value.trim();                                    // Trim whitespace
    if (!value.isEmpty())                            // Store non-empty values
    {
      data[index] = value; // Store parameter
      index++;             // Increment index
    }
    payload = payload.substring(commaIndex + 1); // Remove processed parameter
  }

  payload.trim();                      // Trim remaining payload
  if (!payload.isEmpty() && index < 9) // Store last parameter if present
  {
    data[index] = payload; // Store last parameter
  }

  GreaseOut.Lubrication.Grams = data[0].toInt();      // Set grease quantity
  GreaseOut.Lubrication.Temp.Grams = data[0].toInt(); // Set temporary grease quantity
  GreaseOut.Lubrication.Live.Grams = data[0].toInt(); // Set live grease quantity

  // Parse pause time (HH:MM format)
  int colonIndex = data[1].indexOf(':'); // Find colon position
  if (colonIndex != -1)                  // If colon found
  {
    GreaseOut.PauseTime.Hour = data[1].substring(0, colonIndex).toInt();         // Set pause hours
    GreaseOut.PauseTime.Minute = data[1].substring(colonIndex + 1).toInt();      // Set pause minutes
    GreaseOut.PauseTime.Temp.Hour = data[1].substring(0, colonIndex).toInt();    // Set pause hours
    GreaseOut.PauseTime.Temp.Minute = data[1].substring(colonIndex + 1).toInt(); // Set pause minutes
  }

  LoraFun.DataReceived = true; // Mark data as received
}

/**********************
 * SETTING DATA PROCESS
 * Handles incoming setting LoRa messages
 **********************/
void _setting_data_process(String payload)
{
  LoRa.MessageProcessing(payload); // Process the setting data
}

void LoRaFunction::OfflineSettingPageData()
{
  if (Reset_SendData_Lora)
  {
    unsigned long now = millis();

    if (now - _data_send >= interval)
    {

      _data_send = now; // update timestamp

      String data = "S," + String(GreaseOut.Lubrication.Grams) + "," +
                    String(GreaseOut.PauseTime.Hour) + ":" +
                    String(GreaseOut.PauseTime.Minute);

      if (GreaseOut.Lubrication.Grams > 0 && (GreaseOut.PauseTime.Hour > 0 || GreaseOut.PauseTime.Minute))
      {
        LoRa.publish(LoraTopics.Setting, data);
      }
      else
      {
        Reset_SendData_Lora = false;
        Reset_sendData_Lora_Subscribe = false;
        Serial.println("Offline Setting Page Data will be '0', so data not send");
      }
    }
  }
}
