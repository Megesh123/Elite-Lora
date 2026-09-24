/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note Live data handling and LoRa notification functions
 */

#include "Headerfile.h"

/**********************
 * LIVE DATA FUNCTION
 * Handles live data processing and LoRa updates
 **********************/
void LoRaFunction::liveProcess_Lubrication_Start()
{
  char buffer[100]; // Data buffer

  sprintf(buffer, "*LUBRICATION,%.1f,%02d,%02d,%.1f,%02d,%02d,%.0f,%02d:%02d:%02d,%s,%d#",
          GreaseOut.Lubrication.Live.Grams, GreaseOut.PauseTime.Live.Hour, GreaseOut.PauseTime.Live.Minute, GreaseOut.Lubrication.Total.Grams,
          GreaseOut.PauseTime.Total.Hour, GreaseOut.PauseTime.Total.Minute, GreaseOut.Reservoir.Percentage,
          TotalOperationTime.Hour, TotalOperationTime.Minute, TotalOperationTime.Seconds, Alert.Name, Alert.Total.NoOfAlert);

  LoRa.publish(LoraTopics.LiveData, buffer); // Publish live data to LoRa topic
}

/**********************
 * LIVE DATA FUNCTION
 * Handles live data processing and LoRa updates
 **********************/
void LoRaFunction::liveProcess_Pause_Time_Start()
{
  char buffer[100]; // Data buffer
  sprintf(buffer, "*PAUSE TIME,%.1f,%02d,%02d,%.1f,%02d,%02d,%.0f,%02d:%02d:%02d,%s,%d#",
          GreaseOut.Lubrication.Live.Grams, GreaseOut.PauseTime.Live.Hour, GreaseOut.PauseTime.Live.Minute, GreaseOut.Lubrication.Total.Grams,
          GreaseOut.PauseTime.Total.Hour, GreaseOut.PauseTime.Total.Minute, GreaseOut.Reservoir.Percentage,
          TotalOperationTime.Hour, TotalOperationTime.Minute, TotalOperationTime.Seconds, Alert.Name, Alert.Total.NoOfAlert);

  LoRa.publish(LoraTopics.LiveData, buffer); // Publish live data to LoRa topic
}

/**********************
 * LIVE DATA FUNCTION
 * Handles live data processing and LoRa updates
 **********************/
void LoRaFunction::liveProcess_Lubrication()
{
  char buffer[100]; // Data buffer

  sprintf(buffer, "*LUBRICATION,%.1f,%02d,%02d,%.1f,%02d,%02d,%.0f,%02d:%02d:%02d,%s,%d#",
          GreaseOut.Lubrication.Live.Grams, GreaseOut.PauseTime.Live.Hour, GreaseOut.PauseTime.Live.Minute, GreaseOut.Lubrication.Total.Grams,
          GreaseOut.PauseTime.Total.Hour, GreaseOut.PauseTime.Total.Minute, GreaseOut.Reservoir.Percentage,
          TotalOperationTime.Hour, TotalOperationTime.Minute, TotalOperationTime.Seconds, Alert.Name, Alert.Total.NoOfAlert);

  LoRa.publish(LoraTopics.LiveData, buffer); // Publish live data to LoRa topic
}

/**********************
 * LIVE DATA FUNCTION
 * Handles live data processing and LoRa updates
 **********************/
void LoRaFunction::liveProcess_Pause_Time()
{
  char buffer[100]; // Data buffer
  sprintf(buffer, "*PAUSE TIME,%.1f,%02d,%02d,%.1f,%02d,%02d,%.0f,%02d:%02d:%02d,%s,%d#",
          GreaseOut.Lubrication.Live.Grams, GreaseOut.PauseTime.Live.Hour, GreaseOut.PauseTime.Live.Minute, GreaseOut.Lubrication.Total.Grams,
          GreaseOut.PauseTime.Total.Hour, GreaseOut.PauseTime.Total.Minute, GreaseOut.Reservoir.Percentage,
          TotalOperationTime.Hour, TotalOperationTime.Minute, TotalOperationTime.Seconds, Alert.Name, Alert.Total.NoOfAlert);

  LoRa.publish(LoraTopics.LiveData, buffer); // Publish live data to LoRa topic
}

/**********************
 * LIVE DATA FUNCTION
 * Handles live data processing and LoRa updates
 **********************/
void LoRaFunction::liveProcess_TestRun()
{
  char buffer[100]; // Data buffer
  sprintf(buffer, "*TESTRUN,%.1f,%02d,%02d,%.1f,%02d,%02d,%.0f,%02d:%02d:%02d,%s,%d#",
          GreaseOut.Lubrication.Live.Grams, GreaseOut.PauseTime.Live.Hour, GreaseOut.PauseTime.Live.Minute, GreaseOut.Lubrication.Total.Grams,
          GreaseOut.PauseTime.Total.Hour, GreaseOut.PauseTime.Total.Minute, GreaseOut.Reservoir.Percentage,
          TotalOperationTime.Hour, TotalOperationTime.Minute, TotalOperationTime.Seconds, Alert.Name, Alert.Total.NoOfAlert);

  LoRa.publish(LoraTopics.LiveData, buffer); // Publish live data to LoRa topic
}

/**********************
 * LIVE DATA FUNCTION
 * Handles live data processing and LoRa updates
 **********************/
void LoRaFunction::liveProcess_Alert()
{
  char buffer[200]; // Data buffer
  sprintf(buffer, "*ALERT,%.1f,%02d,%02d,%.1f,%02d,%02d,%.0f,%02d:%02d:%02d,%s,%d#",
          GreaseOut.Lubrication.Live.Grams, GreaseOut.PauseTime.Live.Hour, GreaseOut.PauseTime.Live.Minute, GreaseOut.Lubrication.Total.Grams,
          GreaseOut.PauseTime.Total.Hour, GreaseOut.PauseTime.Total.Minute, GreaseOut.Reservoir.Percentage,
          TotalOperationTime.Hour, TotalOperationTime.Minute, TotalOperationTime.Seconds, Alert.Name, Alert.Total.NoOfAlert);
  LoRa.publish(LoraTopics.LiveData, buffer); // Publish live data to LoRa topic
}

/**********************
 * LIVE DATA FUNCTION
 * Handles live data processing and LoRa updates
 **********************/
void LoRaFunction::liveProcess_Sleep()
{
  char buffer[100]; // Data buffer
  sprintf(buffer, "*SLEEP,%.1f,%02d,%02d,%.1f,%02d,%02d,%.0f,%02d:%02d:%02d,%s,%d#",
          GreaseOut.Lubrication.Live.Grams, GreaseOut.PauseTime.Live.Hour, GreaseOut.PauseTime.Live.Minute, GreaseOut.Lubrication.Total.Grams,
          GreaseOut.PauseTime.Total.Hour, GreaseOut.PauseTime.Total.Minute, GreaseOut.Reservoir.Percentage,
          TotalOperationTime.Hour, TotalOperationTime.Minute, TotalOperationTime.Seconds, Alert.Name, Alert.Total.NoOfAlert);

  LoRa.publish(LoraTopics.LiveData, buffer); // Publish live data to LoRa topic
}

/**********************
 * APP ONLINE/OFFLINE CHECK
 * Handles app online status checks via LoRa
 **********************/
void _app_online_offline_check(String payload)
{
  if (payload.indexOf(PlayLoadMessage.CheckAppOnline) != -1) // If the payload contains the app online check command
  {
    AlertFun.begin();

    if (GreaseOut.Lubrication.Enable) // If lubrication mode is enabled
    {
      LoRa.liveProcess_Lubrication(); // Process live data for lubrication mode
    }
    else if (GreaseOut.PauseTime.Enable) // If pause time mode is enabled
    {
      LoRa.liveProcess_Pause_Time(); // Process live data for pause time mode
    }
    else if (GreaseOut.TestRun.Enable) // If test run mode is enabled
    {
      LoRa.liveProcess_TestRun(); // Process live data for test run mode
    }
    else if (GreaseOut.Sleep.Enable) // If sleep mode is enabled
    {
      LoRa.liveProcess_Sleep(); // Process live data for sleep mode
    }
    else if (Alert.Enable) // If alert mode is enabled
    {
      LoRa.liveProcess_Alert(); // Process live data for alert mode
    }
    LoraFun.AppOnlineSec = 0; // Reset app online seconds counter
  }
}
/**********************
 * CONTROLLER ONLINE RESPONSE
 * Handles controller online status responses via LoRa
 **********************/
void _controller_online_came_return(String payload)
{
  if (payload.indexOf(PlayLoadMessage.Ok) != -1) // If the payload contains the OK response
  {
    bluetooth.Received = 0; // Reset Bluetooth received data count

    vTaskDelay(pdMS_TO_TICKS(10)); // Yield to other tasks with a delay of 10 milliseconds
  }
}

void LoRaFunction::controllerOnline()
{
  if (!_Controller_online_data_send) // If data has not been sent yet
  {

    LoRa.response(LoraTopics.LiveController, Response.ControllerOnline); // Notify controller online status
    _Controller_online_data_send = true;                                 // Set flag to indicate data has been sent
  }
}