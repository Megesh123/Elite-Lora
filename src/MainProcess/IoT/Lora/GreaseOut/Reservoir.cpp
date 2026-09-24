/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note Reservoir handling and LoRa notification functions
 */

#include "Headerfile.h"

/**********************
 * RESERVOIR FUNCTION
 * Sends reservoir capacity data via LoRa
 **********************/
void LoRaFunction::Reservoir(String data)
{
  LoRa.publish(LoraTopics.Reservoircapacity, data); // Publish reservoir capacity message
}
