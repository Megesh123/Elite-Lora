/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note Current monitoring and overload protection implementation.
 */

/**********************
 * CURRENT MONITORING IMPLEMENTATION
 * Handles electrical current measurement, calibration, and overload protection
 * Provides safety mechanism by detecting excessive current draw
 **********************/

#include "Headerfile.h" // Include main system header

// =============================================================================
// CURRENT READING FUNCTION
// Reads current sensor, calculates actual current, and checks for overload conditions
// =============================================================================
void CURRENTFUNCTION::read()
{

  unsigned long now = millis();

  if (now - lastReadMillis < 100)
  {
    return; // Not time yet
  }

  lastReadMillis = now;

  if (bluetooth.ModeCheck)
  {
    return; // No current sensing while in Bluetooth configuration mode
  }

  // ===========================================================================
  // SENSOR DATA ACQUISITION
  // Take multiple samples for noise reduction through averaging.
  // ADC1 is shared with the voltage task - serialise access.
  // ===========================================================================
  CurrentMonitor.adcSum = 0;
  xSemaphoreTake(adcMutex, portMAX_DELAY);
  for (int i = 0; i < ADC_AVERAGING_SAMPLES; i++)
  {
    CurrentMonitor.rawADC = analogRead(PIN_CURRENT_SENSOR);
    CurrentMonitor.adcSum += CurrentMonitor.rawADC;
  }
  xSemaphoreGive(adcMutex);
  CurrentMonitor.rawADC = CurrentMonitor.adcSum / ADC_AVERAGING_SAMPLES; // Average

  // ===========================================================================
  // CURRENT CALCULATION
  // Convert ADC reading to voltage and then to current using calibration
  // ===========================================================================
  float data = (CurrentMonitor.rawADC * ADC_REFERENCE_VOLTAGE) / ADC_MAX_VALUE;        // ADC -> volts (3.3V ref, 12-bit)
  CurrentMonitor.measured = (data - CurrentMonitor.calibration) / CURRENT_SENSITIVITY; // volts -> amps (0.044 V/A)
  if (CurrentMonitor.measured <= 0)
  {
    CurrentMonitor.measured = 0;
  }

  // ===========================================================================
  // OVERLOAD DETECTION
  // Runs on every measured sample, unconditionally. Motor over-current
  // protection must not depend on a comms/config flag, and must keep
  // re-evaluating even while another alert is already latched.
  // ===========================================================================
  if (CurrentMonitor.measured > OVERLOAD_THRESHOLD)
  {
    if (CurrentMonitor.safetyTripCount <= OVERLOAD_DEBOUNCE_COUNT)
    {
      CurrentMonitor.safetyTripCount++; // Count up to the debounce limit (clamp - char field)
    }
  }
  else
  {
    CurrentMonitor.safetyTripCount = 0; // Within safe limits
  }

  if (CurrentMonitor.safetyTripCount > OVERLOAD_DEBOUNCE_COUNT)
  {
    AlertFun.overLoad(); // Trigger / hold overload alert
  }
}

// =============================================================================
// CURRENT SENSOR INITIALIZATION
// Calibrates current sensor by establishing zero-current reference voltage
// =============================================================================
void CURRENTFUNCTION::begin()
{
  CurrentMonitor.rawADC = 0; // Variable to store raw ADC reading
  CurrentMonitor.adcSum = 0; // Variable for averaging multiple readings

  // ===========================================================================
  // CALIBRATION DATA COLLECTION
  // Take multiple samples at zero current to establish baseline reference
  // ===========================================================================
  for (int i = 0; i < ADC_AVERAGING_SAMPLES; i++) // Loop for calibration sampling
  {
    xSemaphoreTake(adcMutex, portMAX_DELAY);
    CurrentMonitor.rawADC = analogRead(PIN_CURRENT_SENSOR); // Read analog value from current sensor pin
    xSemaphoreGive(adcMutex);
    CurrentMonitor.adcSum += CurrentMonitor.rawADC; // Add reading to running total
    vTaskDelay(pdMS_TO_TICKS(1));                   // Yield to other tasks with a delay of 1 milliseconds
  }
  CurrentMonitor.rawADC = CurrentMonitor.adcSum / ADC_AVERAGING_SAMPLES; // Calculate average of calibration readings

  // ===========================================================================
  // REFERENCE VOLTAGE CALCULATION
  // Convert ADC reading to voltage for zero-current baseline
  // ===========================================================================
  CurrentMonitor.calibration = (CurrentMonitor.rawADC * ADC_REFERENCE_VOLTAGE) / ADC_MAX_VALUE; // Convert ADC value to reference voltage

  // Log.info("Current Begin Success");
}

// =============================================================================
// CURRENT READING FUNCTION
// Reads current sensor, calculates actual current, and checks for overload conditions
// =============================================================================
float CURRENTFUNCTION::Read()
{
  CurrentMonitor.rawADC = 0; // Variable to store raw ADC reading
  CurrentMonitor.adcSum = 0; // Variable for averaging multiple readings

  // ===========================================================================
  // SENSOR DATA ACQUISITION
  // Take multiple samples for noise reduction through averaging
  // ===========================================================================
  xSemaphoreTake(adcMutex, portMAX_DELAY);
  for (int i = 0; i < ADC_AVERAGING_SAMPLES; i++) // Loop for sampling
  {
    CurrentMonitor.rawADC = analogRead(PIN_CURRENT_SENSOR); // Read analog value from current sensor pin
    CurrentMonitor.adcSum += CurrentMonitor.rawADC;         // Add reading to running total
  }
  xSemaphoreGive(adcMutex);
  CurrentMonitor.rawADC = CurrentMonitor.adcSum / ADC_AVERAGING_SAMPLES; // Calculate average of readings

  // ===========================================================================
  // CURRENT CALCULATION
  // Convert ADC reading to voltage and then to current using calibration
  // ===========================================================================
  float data = (CurrentMonitor.rawADC * ADC_REFERENCE_VOLTAGE) / ADC_MAX_VALUE;        // Convert ADC value to voltage (3.3V reference, 12-bit ADC)
  CurrentMonitor.measured = (data - CurrentMonitor.calibration) / CURRENT_SENSITIVITY; // Convert voltage to current using calibration factor (0.044V/A)

  if (CurrentMonitor.measured <= 0)
  {
    CurrentMonitor.measured = 0;
  }

  return CurrentMonitor.measured;
}