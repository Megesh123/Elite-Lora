/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note Voltage monitoring and low voltage protection implementation.
 */

/**********************
 * VOLTAGE MONITORING IMPLEMENTATION
 * Handles system voltage measurement, conversion, and low voltage protection
 * Provides safety shutdown when voltage falls below critical levels
 **********************/

#include "Headerfile.h" // Include main system header
#include "esp_task_wdt.h"

// Sample the voltage ADC once, under the shared ADC1 mutex.
static int voltAnalogRead()
{
  xSemaphoreTake(adcMutex, portMAX_DELAY);
  int v = analogRead(PIN_VOLTAGE_SENSOR);
  xSemaphoreGive(adcMutex);
  return v;
}

// =============================================================================
// VOLTAGE READING AND PROCESSING
// Reads analog voltage, converts to actual voltage, and checks for low voltage conditions
// =============================================================================
void VOLTAGEFUNCTION::read()
{
  VoltageMonitor.PowerDownDetected = false;                                                                                            // Controller failure flag
  VoltageMonitor.rawADC = voltAnalogRead();                                                                           // Read analog.measured (0-4095)
  VoltageMonitor.adcSum = (VoltageMonitor.rawADC * ADC_REFERENCE_VOLTAGE) / ADC_MAX_VALUE;                                             // ESP32 ADC conversion: 3.3V reference, 12-bit resolution
  VoltageMonitor.measured = floatMap(VoltageMonitor.adcSum, VOLTAGE_ADC_MIN, VOLTAGE_ADC_MAX, SYSTEM_VOLTAGE_MIN, SYSTEM_VOLTAGE_MAX); // Use same mapping as read function

  if (VoltageMonitor.measured < VoltageMonitor.calibration) // If voltage is critically low
  {
    // Log.info("voltage down detected");
    Eeprom.write(); // Save current system state to EEPROM
    Serial.print("Voltage: ");
    Serial.println(VoltageMonitor.measured);
    VoltageMonitor.PowerDownDetected = true;
    vTaskDelay(pdMS_TO_TICKS(1)); // Yield to other tasks with a delay of 5000 milliseconds

    int _count = 0;
    while (VoltageMonitor.PowerDownDetected)
    {
      VoltageMonitor.rawADC = voltAnalogRead();                                                                           // Read analog.measured (0-4095)
      VoltageMonitor.adcSum = (VoltageMonitor.rawADC * ADC_REFERENCE_VOLTAGE) / ADC_MAX_VALUE;                                             // ESP32 ADC conversion: 3.3V reference, 12-bit resolution
      VoltageMonitor.measured = floatMap(VoltageMonitor.adcSum, VOLTAGE_ADC_MIN, VOLTAGE_ADC_MAX, SYSTEM_VOLTAGE_MIN, SYSTEM_VOLTAGE_MAX); // Use same mapping as read function

      _count++;

      if (VoltageMonitor.measured > 10)
      {
        Eeprom.write();               // Save current system state to EEPROM
        vTaskDelay(pdMS_TO_TICKS(1)); // Yield to other tasks with a delay of 5000 milliseconds
        ESP.restart();                // Restart the system to attempt recovery from low voltage condition
      }

      esp_task_wdt_reset(); // Intentional power-hold - keep feeding the watchdog
      vTaskDelay(pdMS_TO_TICKS(1));
    }
  }

  if (VoltageMonitor.measured > 29) // If voltage is critically low
  {
    // Log.info("voltage down detected");
    Eeprom.write(); // Save current system state to EEPROM
    Serial.print("Voltage: ");
    Serial.println(VoltageMonitor.measured);
    VoltageMonitor.PowerDownDetected = true;
    vTaskDelay(pdMS_TO_TICKS(1)); // Yield to other tasks with a delay of 5000 milliseconds

    int _count = 0;
    while (VoltageMonitor.PowerDownDetected)
    {
      VoltageMonitor.rawADC = voltAnalogRead();                                                                           // Read analog.measured (0-4095)
      VoltageMonitor.adcSum = (VoltageMonitor.rawADC * ADC_REFERENCE_VOLTAGE) / ADC_MAX_VALUE;                                             // ESP32 ADC conversion: 3.3V reference, 12-bit resolution
      VoltageMonitor.measured = floatMap(VoltageMonitor.adcSum, VOLTAGE_ADC_MIN, VOLTAGE_ADC_MAX, SYSTEM_VOLTAGE_MIN, SYSTEM_VOLTAGE_MAX); // Use same mapping as read function

      _count++;

      if (VoltageMonitor.measured < 28)
      {
        Eeprom.write();               // Save current system state to EEPROM
        vTaskDelay(pdMS_TO_TICKS(1)); // Yield to other tasks with a delay of 5000 milliseconds
        ESP.restart();                // Restart the system to attempt recovery from low voltage condition
      }

      esp_task_wdt_reset(); // Intentional power-hold - keep feeding the watchdog
      vTaskDelay(pdMS_TO_TICKS(1));
    }
  }
}

// =============================================================================
// FLOAT MAPPING FUNCTION
// Maps a.measured from one range to another using floating point precision
// Similar to Arduino map() but for float.measureds
// =============================================================================
float VOLTAGEFUNCTION::floatMap(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// =============================================================================
// VOLTAGE MONITORING INITIALIZATION
// Initializes voltage monitoring system and sets appropriate low voltage threshold
// =============================================================================
void VOLTAGEFUNCTION::begin()
{

  VoltageMonitor.rawADC = voltAnalogRead();                                                                           // Read raw ADC.measured
  VoltageMonitor.adcSum = (VoltageMonitor.rawADC * ADC_REFERENCE_VOLTAGE) / ADC_MAX_VALUE;                                             // Convert to voltage (0-3.3V)
  VoltageMonitor.measured = floatMap(VoltageMonitor.adcSum, VOLTAGE_ADC_MIN, VOLTAGE_ADC_MAX, SYSTEM_VOLTAGE_MIN, SYSTEM_VOLTAGE_MAX); // Use same mapping as read function
  if (VoltageMonitor.measured > VOLTAGE_12V_SYSTEM_MIN && VoltageMonitor.measured < VOLTAGE_12V_SYSTEM_MAX)                            // If system voltage is in 12V range (9-15V detection range)
  {
    VoltageMonitor.calibration = VOLTAGE_12V_THRESHOLD; // Set low voltage threshold for 12V systems
  }
  else // If system voltage is in 24V range
  {
    VoltageMonitor.calibration = VOLTAGE_24V_THRESHOLD; // Set low voltage threshold for 24V systems
  }
  // Log.info("Voltage Begin Success");
}

// =============================================================================
// VOLTAGE READING AND PROCESSING
// Reads analog voltage, converts to actual voltage, and checks for low voltage conditions
// =============================================================================
float VOLTAGEFUNCTION::Read()
{
  VoltageMonitor.PowerDownDetected = false;                                                                                            // Controller failure flag
  VoltageMonitor.rawADC = voltAnalogRead();                                                                           // Read analog.measured (0-4095)
  VoltageMonitor.adcSum = (VoltageMonitor.rawADC * ADC_REFERENCE_VOLTAGE) / ADC_MAX_VALUE;                                             // ESP32 ADC conversion: 3.3V reference, 12-bit resolution
  VoltageMonitor.measured = floatMap(VoltageMonitor.adcSum, VOLTAGE_ADC_MIN, VOLTAGE_ADC_MAX, SYSTEM_VOLTAGE_MIN, SYSTEM_VOLTAGE_MAX); // Use same mapping as read function
  return VoltageMonitor.measured;
}
