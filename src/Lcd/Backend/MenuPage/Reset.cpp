/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note Reset page backend logic and user interactions
 */

#include "Headerfile.h"
#include "esp_task_wdt.h"

/**********************
 * RESET PAGE FUNCTION
 * Manages reset page logic and user interactions
 **********************/
void RESRTPAGE::Page()
{
    if (ResetEnable) // If reset mode is enabled
    {
        if (Button.ok.isPressed()) // If OK button is pressed
        {
            if (GreaseOut.Reservoir.Capacity < 1) // If reservoir capacity is less than 1
            {
                ResetAllValueSet = true; // Set reset all value flag
            }
            if (Config.NoOfOutlet < 1) // If number of outlets
            {
                ResetAllValueSet = true; // Set reset all value flag
            }
            if (GreaseOut.Reservoir.Percentage < 10) // If reservoir percentage is less than 10
            {
                ResetAllValueSet = true; // Set reset all value flag
            }

            Button.ok.wait();     // Wait for button release
            if (ResetAllValueSet) // If reset all value flag is set
            {
                while (ResetAllValueSet) // While reset all value flag is set
                {
                    ResetEnable = false;           // Disable reset mode
                    LcdUI.CheckAllValueSetOrNot(); // Check if all values are set
                    if (Button.back.isPressed())   // If OK button is pressed
                    {
                        ResetEnable = true;       // Disable reset mode
                        ResetAllValueSet = false; // Clear reset all value flag
                    }
                    esp_task_wdt_reset();
                    vTaskDelay(pdMS_TO_TICKS(1)); // Yield to other tasks with a delay of 1 milliseconds
                }
            }
            else
            {
                MainEnableWithReset = false;      // Clear main enable with reset flag
                ResetCompleted = true;            // Set reset completed flag
                GreaseOut.TestRun.Enable = false; // Clear test run enable flag
            }
            Button.ok.wait(); // Wait for button release
        }
    }
}
