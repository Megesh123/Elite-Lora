/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note MOTOR CONTROL IMPLEMENTATION
 */

/**********************
 * MOTOR CONTROL IMPLEMENTATION
 * Provides safe control of the pump motor with proximity sensor integration
 * Implements safety features and proper motor management
 **********************/

#include "Headerfile.h" // Include main system header

// Hard wall-clock ceiling for any single proximity-driven motor run. A stuck,
// disconnected or failed proximity sensor must never be able to leave the grease
// pump energised (dead-head / over-pressure hazard). Every exit path from the
// spin loops below forces Motor.off() first.
static const uint32_t MOTOR_PROX_TIMEOUT_MS = 3000;

/**********************
 * MOTOR BEGIN OPERATION
 * Initializes and starts motor with proximity sensor safety checks
 **********************/
void MOTORFUNCTION::begin()
{
    if (GreaseOut.Lubrication.Enable || GreaseOut.TestRun.Enable) // Check if lubrication or test run mode is active
    {
        if (!Alert.Enable) // Check if no alerts are active (safety check)
        {
            const uint32_t start = millis(); // One shared deadline for the whole prime cycle

            // Run while the proximity sensor has not yet sensed...
            while (digitalRead(PIN_PROXIMITY_SENSOR) == PROXIMITY_NOT_SENSE)
            {
                if (millis() - start >= MOTOR_PROX_TIMEOUT_MS)
                {
                    Motor.off(); // Safety: sensor never transitioned - stop the pump
                    return;
                }
                Motor.on();
                vTaskDelay(pdMS_TO_TICKS(1));
            }

            // ...then run while it is held sensed, so one full stroke completes.
            while (digitalRead(PIN_PROXIMITY_SENSOR) == PROXIMITY_SENSE)
            {
                if (millis() - start >= MOTOR_PROX_TIMEOUT_MS)
                {
                    Motor.off(); // Safety: sensor stuck in sensed state - stop the pump
                    return;
                }
                Motor.on();
                vTaskDelay(pdMS_TO_TICKS(1));
            }
        }
        Motor.off();                   // Turn motor off after operation
        vTaskDelay(pdMS_TO_TICKS(10)); // Delay before next operation
    }
}

/**********************
 * MOTOR RESET OPERATION
 * Resets motor operation with proximity sensing integration
 **********************/
void MOTORFUNCTION::reset()
{
    GreaseOut.Proximity.AlertCheck = true; // Enable proximity alert check

    const uint32_t start = millis(); // Shared deadline - see MOTOR_PROX_TIMEOUT_MS

    while (digitalRead(PIN_PROXIMITY_SENSOR) == PROXIMITY_NOT_SENSE)
    {
        if (millis() - start >= MOTOR_PROX_TIMEOUT_MS)
        {
            Motor.off(); // Safety: sensor never transitioned - stop the pump
            return;
        }
        Motor.on();
        vTaskDelay(pdMS_TO_TICKS(1));
    }

    while (digitalRead(PIN_PROXIMITY_SENSOR) == PROXIMITY_SENSE)
    {
        if (millis() - start >= MOTOR_PROX_TIMEOUT_MS)
        {
            Motor.off(); // Safety: sensor stuck in sensed state - stop the pump
            return;
        }
        Motor.on();
        vTaskDelay(pdMS_TO_TICKS(1));
    }

    Motor.off();                    // Turn motor off after reset operation
    vTaskDelay(pdMS_TO_TICKS(100)); // Non-blocking delay before next operation
}

/**********************
 * MOTOR ON
 * Turns motor on at full speed (0 = full on)
 **********************/
void MOTORFUNCTION::on()
{
    pinMode(PIN_PUMP_MOTOR, OUTPUT); // Motor pump control pin
    digitalWrite(PIN_PUMP_MOTOR, LOW);
}

/**********************
 * MOTOR OFF
 * Turns motor off completely (255 = full off)
 **********************/
void MOTORFUNCTION::off()
{
    pinMode(PIN_PUMP_MOTOR, OUTPUT); // Motor pump control pin
    digitalWrite(PIN_PUMP_MOTOR, HIGH);
}

/**********************
 * MOTOR TOGGLE SPEED
 * Toggles motor to intermediate speed (~25% duty cycle)
 **********************/
void MOTORFUNCTION::toggle()
{
    pinMode(PIN_PUMP_MOTOR, OUTPUT);                // Motor pump control pin
    analogWrite(PIN_PUMP_MOTOR, PUMP_SPEED_MEDIUM); // Write PWM value 192 to pump pin (medium speed)
}