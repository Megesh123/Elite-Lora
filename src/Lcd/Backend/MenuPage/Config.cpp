/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note Configuration page backend logic and variable management.
 */

#include "Headerfile.h"

/**********************
 * CONFIGURATION PAGE FUNCTION
 * Handles configuration page logic and variable adjustments
 **********************/
void CONFIGPAGE::Page()
{
    if (ConfigEnable) // If configuration mode is enabled
    {
        if (Button.ok.isPressed()) // If OK button is pressed
        {
            Button.ok.wait();                               // Wait for button release
            if (ConfigSelectVariable.indexOf("TANK") != -1) // If TANK variable is selected
            {
                VariableCountIncDec(TankCapcity, 1.0f, 4.0f, 1.0f, false, TankBlink); // Adjust tank capacity between 1.0 and 4.0
            }
            else if (ConfigSelectVariable.indexOf("OUTLET") != -1) // If OUTLET variable is selected
            {
                VariableCountIncDec(Config.NoOfOutlet, 1, 3, 1, false, OutletBlink); // Adjust number of outlets between 1 and 3
            }
        }
        Setcursor(); // Update cursor position based on button input
    }
}

/**********************
 * CONFIGURATION PAGE RESET FUNCTION
 * Resets configuration page state
 **********************/
void CONFIGPAGE::Reset()
{

    ConfigFunSetcursor = 12;   // Reset cursor position
    ConfigSelectVariable = ""; // Clear selected variable
}

/**********************
 * CONFIGURATION PAGE SET CURSOR FUNCTION
 * Manages cursor movement and selection on configuration page
 **********************/
void CONFIGPAGE::Setcursor()
{
    if (ConfigEnable) // If configuration mode is enabled
    {
        MenuSetting.Config.SetCursor.Select(); // Update selected variable based on cursor position
        if (Button.down.isPressed())           // If Down button is pressed
        {
            ConfigFunSetcursor += 12;              // Move cursor down
            if (ConfigFunSetcursor > 24)           // Wrap around if exceeding max position
                ConfigFunSetcursor = 12;           // Reset to top position
            MenuSetting.Config.SetCursor.Select(); // Update selected variable based on cursor position
            Button.down.wait();                    // Wait for button release
        }

        if (Button.up.isPressed()) // If Up button is pressed
        {
            ConfigFunSetcursor -= 12;              // Move cursor up
            if (ConfigFunSetcursor < 12)           // Wrap around if exceeding min position
                ConfigFunSetcursor = 24;           // Set to bottom position
            MenuSetting.Config.SetCursor.Select(); // Update selected variable based on cursor position
            Button.up.wait();                      // Wait for button release
        }
    }
}

/**********************
 * CONFIGURATION SET CURSOR SELECTION FUNCTION
 * Updates selected variable based on cursor position
 **********************/
void CONFIGSETCURSOR::Select()
{
    if (ConfigFunSetcursor == 12) // If cursor is at first position
    {
        ConfigSelectVariable = "TANK"; // Select TANK variable
    }
    else if (ConfigFunSetcursor == 24) // If cursor is at second position
    {
        ConfigSelectVariable = "OUTLET"; // Select OUTLET variable
    }
    else
    {
        ConfigSelectVariable = "INVALID"; // Invalid selection
    }
}