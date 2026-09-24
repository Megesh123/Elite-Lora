/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note Reservoir monitoring implementation for Lubrication Control System
 */

/**********************
 * RESERVOIR MONITORING IMPLEMENTATION
 * Handles reservoir level calculation, refill tracking, and low level alerts
 * Provides real-time reservoir status and triggers alerts when level is critical
 **********************/

#include "Headerfile.h" // Include main system header

/**********************
 * RESERVOIR LEVEL MONITORING FUNCTION
 * Calculate current reservoir fill percentage and trigger low level alerts
 **********************/
void RESERVOIRFUNCTION::read()
{
  if (!Alert.EnableDisable.RefillAlert) // If reservoir monitoring is not enabled, exit function
  {
    return;
  }

  // Without a configured tank capacity the percentage maths would divide by
  // zero and push inf/nan into the alert logic, EEPROM and LoRa payloads.
  if (GreaseOut.Reservoir.Capacity <= 0)
  {
    return;
  }

  GreaseOut.Reservoir.Temp.Percentage += Config.NoOfOutlet; // Add current cycle discharge to total used

  float data = GreaseOut.Reservoir.Capacity - GreaseOut.Reservoir.Temp.Percentage; // Remaining grease

  GreaseOut.Reservoir.Percentage = round((data * 100.0f) / GreaseOut.Reservoir.Capacity); // Percent, rounded

  if (GreaseOut.Reservoir.Percentage < 0)
  {
    GreaseOut.Reservoir.Percentage = 0;
  }
  if (GreaseOut.Reservoir.Percentage > 100)
  {
    GreaseOut.Reservoir.Percentage = 100;
  }

  if (GreaseOut.Reservoir.Percentage <= RESERVOIR_LOW_LEVEL_PERCENT && !Alert.SnoozeReservoir) // If reservoir level is below threshold
  {
    AlertFun.reservoir(); // Trigger reservoir low level alert
  }
}
