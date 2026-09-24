/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note Main page backend logic and state management.
 */

#include "Headerfile.h"
#include "esp_task_wdt.h"

MAINFUNCTION MainPage;

/**********************
 * MAIN PAGE FUNCTION
 * Handles main page logic and state management
 **********************/
void MAINFUNCTION::Function()
{
    if (MainEnable)
    {
        ConfigFunSetcursor = 12;  // Reset cursor position
        SettingFunSetcursor = 12; // Reset setting cursor position
        RefillFunSetcursor = 12;  // Reset refill cursor position

        if (Reset) // If reset flag is set
        {
            Reset = false;    // Clear reset flag
            VariableUpdate(); // Update system variables
        }
    }
}

/**********************
 * MAIN PAGE ENABLE FUNCTION
 * Manages enabling the main page and checks for configuration completeness
 **********************/
void MAINFUNCTION::pageEnable()
{

    if (Config.NoOfOutlet < 1) // Check if essential configuration values are set/
    {
        ResetAllValueSet = true; // Flag to indicate reset required
    }
    if (GreaseOut.Reservoir.Percentage < 10) // Check if reservoir percentage is below threshold
    {
        ResetAllValueSet = true; // Flag to indicate reset required
    }
    if (GreaseOut.Lubrication.Grams < 1) // Check if lubrication grams is below threshold
    {
        ResetAllValueSet = true; // Flag to indicate reset required
    }
    if (GreaseOut.PauseTime.Minute == 0 && GreaseOut.PauseTime.Hour == 0) // Check if pause time is not set
    {
        ResetAllValueSet = true; // Flag to indicate reset required
    }

    if (ResetAllValueSet && MenuSettingEditTrue) // If reset required and menu setting is being edited
    {
        while (ResetAllValueSet) // Loop until reset is completed
        {
            LcdUI.CheckAllValueSetOrNot(); // Display prompt to user
            if (Button.ok.isPressed())     // If OK button is pressed
            {
                ResetAllValueSet = false; // Clear reset flag
                Button.ok.wait();         // Wait for button release
            }
            if (Button.back.isPressed()) // If Back button is pressed
            {
                MenuEnable = false;                       // Disable menu
                MenuSettingEnableDisableCheck = false;    // Disable menu setting check
                MenuSetting_sec = 0;                      // Reset menu setting timer
                MainEnable = true;                        // Enable main page
                LoraDetails.UserEnterOfflineMode = false; // Disable offline mode
                MenuFunSetcursor = 12;                    // Reset menu cursor position
                ResetAllValueSet = false;                 // Clear reset flag
            }
            esp_task_wdt_reset();
            vTaskDelay(pdMS_TO_TICKS(1)); // Yield to other tasks with a delay of 1 milliseconds
        }
    }
    else
    {
        Serial.println("Main Page Enable");       // Log main page enable action
        MenuEnable = false;                       // Disable menu
        MenuSettingEnableDisableCheck = false;    // Disable menu setting check
        MenuSetting_sec = 0;                      // Reset menu setting timer
        MainEnable = true;                        // Enable main page
        LoraDetails.UserEnterOfflineMode = false; // Disable offline mode
        MenuFunSetcursor = 12;                    // Reset menu cursor position
        ResetAllValueSet = false;                 // Clear reset flag
        vTaskDelay(pdMS_TO_TICKS(10));            // Short delay to ensure state changes take effect
        Reset = false;
        if (MenuResetnotCompleted && !MenuSettingEditTrue) // If menu reset is pending and not in edit mode
        {
            Serial.println("Resetting all values and reinitializing system"); // Log reset action
            Grease.begin();                                                   // Reinitialize grease process
            MenuResetnotCompleted = false;                                    // Clear menu reset flag
        }
    }
}
