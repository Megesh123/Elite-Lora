/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note LoRa message receiving and processing functions
 */

#include "Headerfile.h"

/**********************
 * LORA MESSAGE PROCESSOR
 * Routes messages to appropriate handlers based on topic and payload
 **********************/
void _receive_message_process(String topic, String payload)
{

  if (!LoraDetails.UserEnterOfflineMode)
  {
    // Exact full-topic match. indexOf()/substring matching mis-routed messages
    // whenever one topic string was a substring of another (e.g. "/MODE" inside
    // "/ACK_CONMODE"); the first matching branch would win.
    if (topic == LoraTopics.Dashboard_return)
    {
      LoraDetails.lora_connected_but_not_received = 0;
      LoraDetails.DashboardAckPending = false;
    }
    else if (topic == LoraTopics.offline_return)
    {
      Received._offline_data_process = payload;
    }
    else if (topic == LoraTopics.subscribe) // App command topic
    {
      Received._setting_data_process = payload;
    }
    else if (topic == LoraTopics.alert_return) // Alert return topic
    {
      Received._alert_data_process = payload;
    }
    else if (topic == LoraTopics.alert_clear) // Alert clear topic
    {
      Received._alert_clear_process = payload;
    }
    else if (topic == LoraTopics.LiveController_return) // Live controller return topic
    {
      Received._controller_online_came_return = payload;
    }
    else if (topic == LoraTopics.Setting_return) // Setting topic
    {
      Received._Setting = payload;
    }
    else if (topic == LoraTopics.Mode)
    {
      Received._mode = payload;
    }
    else if (topic == LoraTopics.ConMode_return)
    {
      Received._conmode = payload;
    }
  }
  else
  {
    if (topic == LoraTopics.ConMode_return)
    {
      Received._conmode = payload;
    }
  }
}

void Receive()
{
  // Placeholder for additional receive handler 1
  if (Received._offline_data_process != "")
  {
    // _offline_data_process(Received._offline_data_process); // Handle offline data transmission
    Received._offline_data_process = "";
  }
  if (Received._setting_data_process != "")
  {
    _setting_data_process(Received._setting_data_process); // Process system configuration data
    Received._setting_data_process = "";
  }
  if (Received._alert_data_process != "")
  {
    _alert_data_process(Received._alert_data_process); // Handle alert acknowledgments
    Received._alert_data_process = "";
  }
  if (Received._alert_clear_process != "")
  {
    _alert_clear_process(Received._alert_clear_process); // Process alert clearance commands
    Received._alert_clear_process = "";
  }
  if (Received._controller_online_came_return != "")
  {
    _controller_online_came_return(Received._controller_online_came_return); // Handle controller status
    Received._controller_online_came_return = "";
  }
  if (Received._Setting != "")
  {
    if (Received._Setting.indexOf("OK") != -1)
    {
      Reset_SendData_Lora = false;
    }
    Reset_sendData_Lora_Subscribe = false;
    Received._Setting = "";
  }
  if (Received._mode != "")
  {
    _refillMode_start_stop(Received._mode);    // Handle refill mode control
    _testRun_start_stop(Received._mode);       // Process test run commands
    _controller_sleep_process(Received._mode); // Process sleep mode commands
    _control_Snooze(Received._mode);           // Handle alert snooze commands
    _app_online_offline_check(Received._mode); // Handle app status checks
    _refill_complete_process(Received._mode);  // Process refill completion
    _factory_reset_process(Received._mode);    // Handle factory reset commands
    _reconfig_process(Received._mode);         // Process system reconfiguration
    _refill_alert_enable_disable(Received._mode);
    _supervisory_Enable_Disable(Received._mode); // Handle supervisory level enable/disable commands

    Received._mode = "";
  }

  if (Received._conmode != "")
  {
    if (Received._conmode == "OK")
    {
      Twoway.Return();
    }
    else
    {
      Serial.println("CON MODE - UNKNOW");
    }
    Received._conmode = "";
  }
}
