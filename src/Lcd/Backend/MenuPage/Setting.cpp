/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note Setting page backend logic and user interactions
 */

#include "Headerfile.h"

/**********************
 * SETTING PAGE FUNCTION
 * Manages setting page logic and user interactions
 **********************/
void SETTINGPAGE::Page()
{
    if (SettingEnable) // If setting mode is enabled
    {
        if (Button.ok.isPressed()) // If OK button is pressed
        {
            Button.ok.wait();                               // Wait for button release
            if (SettingSelectVariable.indexOf("LUB") != -1) // If "LUB" is selected
            {
                VariableCountIncDec(GreaseOut.Lubrication.Grams, 1.0f, 99.0f, 1.0f, true, LubBlink); // Increment/decrement lubrication grams
            }
            else if (SettingSelectVariable.indexOf("PTM HOUR") != -1) // If "PTM HOUR" is selected
            {
                VariableCountIncDec(GreaseOut.PauseTime.Hour, 0, 99, 1, true, PtmHourBlink); // Increment/decrement pause time hours
            }
            else if (SettingSelectVariable.indexOf("PTM MIN") != -1) // If "PTM MIN" is selected
            {
                if (GreaseOut.PauseTime.Hour == 0) // If pause time hours is 0
                {
                    VariableCountIncDec(GreaseOut.PauseTime.Minute, 1, 59, 1, true, PtmMinBlink); // Increment/decrement pause time minutes (1-59)
                }
                else
                {
                    VariableCountIncDec(GreaseOut.PauseTime.Minute, 0, 59, 1, true, PtmMinBlink); // Increment/decrement pause time minutes (0-59)
                }
            }
        }
        if (GreaseOut.PauseTime.Hour == 0 && GreaseOut.PauseTime.Minute == 0) // If both pause time hours and minutes are 0
        {
            GreaseOut.PauseTime.Minute = 1;
        }
    }
}

/**********************
 * SETTING PAGE RESET FUNCTION
 * Resets setting page state
 **********************/
void SETTINGPAGE::Reset()
{

    SettingFunSetcursor = 12;   // Reset setting cursor position
    SettingSelectVariable = ""; // Clear selected variable
}

/**********************
 * SETTING PAGE SET CURSOR FUNCTION
 * Manages cursor movement and selection on setting page
 **********************/
void SETTINGPAGE::Setcursor()
{
    if (SettingEnable) // If setting mode is enabled
    {
        if (Button.down.isPressed()) // If down button is pressed
        {
            SettingFunSetcursor += 12;    // Move cursor down by 12
            if (SettingFunSetcursor > 36) // If cursor exceeds 36
                SettingFunSetcursor = 12; // Wrap around to 12
            Button.down.wait();           // Wait for button release
        }

        if (Button.up.isPressed()) // If up button is pressed
        {
            SettingFunSetcursor -= 12;    // Move cursor up by 12
            if (SettingFunSetcursor < 12) // If cursor is less than 12
                SettingFunSetcursor = 36; // Wrap around to 36
            Button.up.wait();             // Wait for button release
        }
        MenuSetting.Setting.SetCursor.Select(); // Update selected variable based on cursor position
    }
}

/**********************
 * SETTING SET CURSOR SELECT FUNCTION
 * Updates selected variable based on cursor position
 **********************/
void SETTINGSETCURSOR::Select()
{
    int index = SettingFunSetcursor / 12; // 1-based

    if (index == 1)                         // If index is 1
        SettingSelectVariable = "LUB";      // Set selected variable to "LUB"
    else if (index == 2)                    // If index is 2
        SettingSelectVariable = "PTM HOUR"; // Set selected variable to "PTM HOUR"
    else if (index == 3)                    // If index is 3
        SettingSelectVariable = "PTM MIN";  // Set selected variable to "PTM MIN"
}