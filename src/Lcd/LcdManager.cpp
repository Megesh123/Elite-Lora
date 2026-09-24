/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note LCD display management based on system states.
 */

#include "Headerfile.h"

LcdManager Lcd;

/**********************
 * LCD DISPLAY FUNCTION
 * Manages the overall display logic based on system states
 **********************/
void LcdManager::DisplayFunction()
{
    if (!bluetooth.ModeCheck && !VoltageMonitor.PowerDownDetected) // If not in Bluetooth mode
    {
        LcdUI.OTAPage(); // Display OTA update page if OTA is in progress

        if (!GreaseOut.Sleep.Enable) // If not in sleep mode
        {
            if (MainEnable && !Alert.Enable && !GreaseOut.TestRun.Enable && !AboutDevices.Enable) // If main is enabled, no alert, and not in test run
            {
                LcdUI.MainPage(); // Display main page
            }
            else if (AboutDevices.Enable && !MenuEnable && !TestrunPage)
            {
                Admin.page();
            }
            else if (GreaseOut.TestRun.Enable && MainEnable && !Alert.Enable) // If in test run, main is enabled, and no alert
            {
                LcdUI.Testrun(); // Display test run page
            }
            else if (!MainEnable) // If menu is enabled
            {
                LcdUI.MenuSettingPage(); // Display menu setting page
            }
            else if (Alert.Enable && !MenuSettingEnableDisableCheck) // If alert is enabled and menu setting is not active
            {
                LcdUI.AlertPage(); // Display alert page
            }
        }
        else
        {
            LcdUI.SleepMode(); // Display sleep mode animation
        }
    }
    else if (!bluetooth.ModeCheck)
    {
        display.clearDisplay();
        display.display();
        Motor.off();
        Led.off();
    }
}