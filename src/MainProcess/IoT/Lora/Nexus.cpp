/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note Nexus interface LoRa notification functions
 */

#include "Headerfile.h"

/**********************
 * NEXUS IN ONLINE STATUS
 * Sends Nexus interface online status via LoRa
 **********************/
void LoRaFunction::NEXUS_IN_ON(String data)
{
  if (LoRa.publish(LoraTopics.Dashboard, data)) // Publish online status to dashboard topic
  {
    LoraDetails.DashboardAckPending = true;
  }
}

/**********************
 * NEXUS NOT REACHABLE STATUS
 * Sends Nexus interface not reachable status via LoRa
 **********************/
void LoRaFunction::NEXUS_NOT_REACHABLE(String data)
{

  if (LoRa.publish(LoraTopics.Dashboard, data)) // Publish not reachable status to dashboard topic
  {
    LoraDetails.DashboardAckPending = true;
  }
}

/**********************
 * NEXUS ALERT STATUS
 * Sends Nexus interface alert status via LoRa
 **********************/
void LoRaFunction::NEXUS_IN_ALERT(String data)
{
  if (LoRa.publish(LoraTopics.Dashboard, data)) // Publish alert status to dashboard topic
  {
    LoraDetails.DashboardAckPending = true;
  }
}

/**********************
 * NEXUS ONLINE UPDATE
 * Updates Nexus online status based on configured interval
 **********************/
void LoRaFunction::NEXUS_ONLINE_UPDATE()
{
  if (NexuxIn.Seconds >= NEXUS_IN_ON_SECONDS) // If the configured interval has elapsed
  {
    // Log.info("Nexus Online Status Update - Seconds: %d", NexuxIn.Seconds); // Log the online status update
    NexuxIn.Seconds = 0; // Reset seconds counter
    if (!Alert.Enable)
    {
      LoRa.NEXUS_IN_ON(NEXUS_IN_ON_DATA); // Send Nexus online status
    }
  }
}

void LoRaFunctionTimer::NexuxInOnline()
{
  if (NexuxIn.Enable)
    NexuxIn.Seconds++;
}
