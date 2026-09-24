/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note Refill page backend logic and user interaction management.
 */

#include "Headerfile.h"
#include "esp_task_wdt.h"

/**********************
 * REFILL PAGE FUNCTION
 * Manages refill page logic and user interactions
 **********************/
void REFILLPAGE::Page()
{
    if (RefillEnable) // If refill mode is enabled
    {
        if (GreaseOut.Reservoir.Capacity == 0) // If reservoir capacity is zero
        {
            return;
        }
        if (Button.ok.isPressed() && RefillFunSetcursor == 24) // If OK button is pressed and refill cursor is at 24
        {
            RefillConfirmation = true;      // Set refill confirmation flag
            RefillConfirmationYesNo = true; // Default to 'Yes' for confirmation
            Button.ok.wait();               // Wait for button release
        }
        while (RefillConfirmation) // While refill confirmation is active
        {
            if (Button.up.isPressed()) // If up button is pressed
            {
                RefillConfirmationYesNo = !RefillConfirmationYesNo; // Toggle confirmation choice
                Button.up.wait();                                   // Wait for button release
            }
            if (Button.down.isPressed()) // If down button is pressed
            {
                RefillConfirmationYesNo = !RefillConfirmationYesNo; // Toggle confirmation choice
                Button.down.wait();                                 // Wait for button release
            }
            if (Button.back.isPressed()) // If back button is pressed
            {
                RefillConfirmation = false; // Cancel refill confirmation
                Button.back.wait();         // Wait for button release
            }

            if (Button.ok.isPressed()) // If OK button is pressed
            {
                if (RefillConfirmationYesNo) // If user confirmed refill
                {
                    RefillConfirmation = false;        // Clear confirmation flag
                    RefillStart = true;                // Set refill start flag
                    GreaseOut.Reservoir.Enable = true; // Enable reservoir refill
                }
                else // If user declined refill
                {
                    RefillConfirmation = false; // Clear confirmation flag
                }
                Button.ok.wait(); // Wait for button release
            }
            esp_task_wdt_reset();
            vTaskDelay(pdMS_TO_TICKS(1)); // Yield to other tasks with a delay of 1 milliseconds
        }
        while (RefillStart) // While refill process is active
        {
            if (Button.ok.isPressed()) // If OK button is pressed
            {
                RefillStart = false;                // Stop refill process
                GreaseOut.Reservoir.Enable = false; // Disable reservoir refill
                RefillStop = true;                  // Set refill stop flag
                Button.ok.wait();                   // Wait for button release
            }

            Motor.toggle(); // Set motor to intermediate speed for refill
            esp_task_wdt_reset();
            vTaskDelay(pdMS_TO_TICKS(1)); // Yield to other tasks with a delay of 1 milliseconds
        }
        while (RefillStop) // While refill stop process is active
        {
            Motor.off();
            if (Button.ok.isPressed()) // If OK button is pressed
            {
                RefillStop = false;     // Clear refill stop flag
                RefillCompleted = true; // Set refill completed flag
                Button.ok.wait();       // Wait for button release
            }
            esp_task_wdt_reset();
            vTaskDelay(pdMS_TO_TICKS(1)); // Yield to other tasks with a delay of 1 milliseconds
        }
        while (RefillCompleted) // While refill completed process is active
        {
            Motor.off();
            TempTankCapcity = 0;                  // Reset temporary tank capacity
            GreaseOut.Reservoir.Percentage = 100; // Set reservoir percentage to 100%
            GreaseOut.Reservoir.Temp.Percentage = 0;
            GreaseOut.Reservoir.Temp._old_reservoir_value = -1;
            ReconfigEnableRefillNeed = false;
            Alert.SnoozeReservoir = false; // Enable reservoir snooze
            esp_task_wdt_reset();
            vTaskDelay(pdMS_TO_TICKS(1)); // Yield to other tasks with a delay of 1 milliseconds
        }
    }
}

/**********************
 * REFILL PAGE SET CURSOR FUNCTION
 * Manages cursor movement on refill page
 **********************/
void REFILLPAGE::Setcursor()
{
    if (RefillEnable) // If refill mode is enabled
    {
        if (Button.down.isPressed()) // If down button is pressed
        {
            RefillFunSetcursor += 12;    // Move cursor down by 12
            if (RefillFunSetcursor > 24) // If cursor exceeds 24
                RefillFunSetcursor = 12; // Wrap around to 12
            Button.down.wait();          // Wait for button release
        }

        if (Button.up.isPressed()) // If up button is pressed
        {
            RefillFunSetcursor -= 12;    // Move cursor up by 12
            if (RefillFunSetcursor < 12) // If cursor goes below 12
                RefillFunSetcursor = 24; // Wrap around to 24
            Button.up.wait();            // Wait for button release
        }
    }
}
