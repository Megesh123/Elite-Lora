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
void LcdUIFunction::_SettingPage()
{
    if (SettingEnable) // If setting mode is enabled
    {
        display.clearDisplay(); // Clear the display buffer
        LcdDisplay.Boder();     // Draw border on the display

        _settingPage_Setcursor();      // Handle cursor blinking
        _settingPage_LubDischarge();   // Display lubrication discharge setting
        _settingPage_PauseTime_Hour(); // Display pause time hour setting
        _settingPage_PauseTime_Min();  // Display pause time minute setting

        display.display();            // Update the display with the buffer content
        vTaskDelay(pdMS_TO_TICKS(1)); // Yield to other tasks with a delay of 10 milliseconds
    }
}

/**********************
 * SETTING PAGE SET CURSOR FUNCTION
 * Manages cursor movement and selection on setting page
 **********************/
void LcdUIFunction::_settingPage_Setcursor()
{
    if (VariableSeleteEnable) // If variable selection is enabled
    {
        LcdDisplay.print(10, SettingFunSetcursor, PRINT_RIGHT_ARROW);            // Print cursor arrow
        LcdDisplay._DrawStraightLine(7, SettingFunSetcursor - 4, 5, HORIZONTAL); // Draw horizontal line above the selected option
    }
    else
    {
        if (_shouldBlink(ArrowlastBlinkTime, ArrowblinkState)) // If it's time to blink the arrow
        {
            LcdDisplay.print(10, SettingFunSetcursor, PRINT_RIGHT_ARROW);            // Print cursor arrow
            LcdDisplay._DrawStraightLine(7, SettingFunSetcursor - 4, 5, HORIZONTAL); // Draw horizontal line above the selected option
        }
    }
}

/**********************
 * SETTING PAGE LUBRICATION DISCHARGE FUNCTION
 * Displays lubrication discharge setting on the display
 **********************/
void LcdUIFunction::_settingPage_LubDischarge()
{
    display.setCursor(20, 12);      // Set cursor position
    display.print(PRINT_DISCHARGE); // Print "LUB DIS : "
    if (!LubBlink)                  // If lubrication blink state is OFF
    {
        if (GreaseOut.Lubrication.Grams < 10) // If grams value is less than 10
        {
            LcdDisplay.print0(); // Print leading zero for single-digit values
        }
        display.print(GreaseOut.Lubrication.Grams, 0); // Print the grams value
        display.print(PRINT_GRAM);                     // Print "G" to indicate grams
    }
}

/**********************
 * SETTING PAGE PAUSE TIME HOUR FUNCTION
 * Displays pause time hour setting on the display
 **********************/
void LcdUIFunction::_settingPage_PauseTime_Hour()
{
    display.setCursor(20, 24);       // Set cursor position
    display.print(PRINT_PAUSE_HOUR); // Print "PTM HOR : "
    if (!PtmHourBlink)               // If pause time hour blink state is OFF
    {
        if (GreaseOut.PauseTime.Hour < 10) // If hour value is less than 10
        {
            LcdDisplay.print0(); // Print leading zero for single-digit values
        }
        display.print(GreaseOut.PauseTime.Hour); // Print the hour value
    }
}

/**********************
 * SETTING PAGE PAUSE TIME MINUTE FUNCTION
 * Displays pause time minute setting on the display
 **********************/
void LcdUIFunction::_settingPage_PauseTime_Min()
{
    display.setCursor(20, 36);      // Set cursor position
    display.print(PRINT_PAUSE_MIN); // Print "PTM MIN : "
    if (!PtmMinBlink)               // If pause time minute blink state is OFF
    {
        if (GreaseOut.PauseTime.Minute < 10) // If minute value is less than 10
        {
            LcdDisplay.print0(); // Print leading zero for single-digit values
        }
        display.print(GreaseOut.PauseTime.Minute); // Print the minute value
    }
}
