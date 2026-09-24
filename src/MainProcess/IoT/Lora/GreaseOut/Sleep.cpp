/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note Sleep mode handling and LoRa notification functions
 */

#include "Headerfile.h"

/**********************
 * CONTROLLER SLEEP PROCESS
 * Handles controller sleep mode messages
 **********************/
void _controller_sleep_process(String payload)
{
  if (payload.indexOf(PlayLoadMessage.SleepModeStart) != -1 && start_init == 1) // If sleep mode start message
  {
    LoRa.response(LoraTopics.SleepMode_return, Response.Ok); // Send OK response
    LoRa.NEXUS_NOT_REACHABLE(NEXUS_NOT_REACHABLE_DATA);      // Notify Nexus not reachable
    SLeepMode.start();
  }
  else if (payload.indexOf(PlayLoadMessage.SleepModeStop) != -1 && start_init == 1) // If sleep mode stop message
  {
    LoRa.response(LoraTopics.SleepMode_return, Response.Ok); // Send OK response
    GreaseOut.Sleep.Enable = false;                          // Disable sleep mode
    LoRa.NEXUS_NOT_REACHABLE(NEXUS_REACHABLE_DATA);          // Notify Nexus reachable
    SLeepMode.stop();
  }
  else if (payload.indexOf(PlayLoadMessage.SleepModeStart) != -1 && start_init != 1)
  {
    LoRa.response(LoraTopics.SleepMode_return, Response.Error); // Send ERROR response
    TestRunEnableError = true;
  }
  else if (payload.indexOf(PlayLoadMessage.SleepModeStop) != -1 && start_init != 1)
  {
    LoRa.response(LoraTopics.SleepMode_return, Response.Error); // Send ERROR response
    TestRunEnableError = true;
  }
}
