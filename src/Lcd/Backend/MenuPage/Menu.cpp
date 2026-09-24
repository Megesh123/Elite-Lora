/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note Menu setting backend logic and state management.
 */

#include "Headerfile.h"
#include "esp_task_wdt.h"

MENUFUNCTION MenuSetting;

/**********************
 * MENU SETTING FUNCTION
 * Manages menu setting logic and state transitions
 **********************/
void MENUFUNCTION::SettingFunction()
{
    if (!VoltageMonitor.PowerDownDetected && !bluetooth.ModeCheck)
    {
        AboutDevice.pageEnableDisable(); // Manage About Device page enable/disable
        SLeepMode.Enable();
        SLeepMode.Disable();
        if (!bluetooth.ModeCheck && !GreaseOut.Sleep.Enable) // If not in Bluetooth mode
        {
            MainPage.Function();      // Call main page function
            MenuSetting.Function();   // Call menu setting function
            MenuSetting.pageEnable(); // Enable menu setting
        }
    }
}

/**********************
 * MENU SETTING FUNCTION
 * Manages menu setting logic and state transitions
 **********************/
void MENUFUNCTION::Function()
{
    Grease.TestRunPageEnableDisable(); // Manage test run page enable/disable
    if (!MainEnable)                   // If main is not enabled
    {
        MenuSetting.MEnu.Page();    // Call menu page function
        MenuSetting.Setting.Page(); // Call setting page function
        MenuSetting.Refill.Page();  // Call refill page function
        MenuSetting.TestRun.Page(); // Call test run page function
        MenuSetting.Reset.Page();   // Call reset page function
        MenuSetting.BackFunction(); // Handle back button functionality
    }
}

/**********************
 * MENU PAGE ENABLE FUNCTION
 * Handles enabling the menu page and keylock functionality
 **********************/
void MENUFUNCTION::pageEnable()
{
    int EnableCount = 0;  // Counter for enabling keylock
    int DisableCount = 0; // Counter for disabling keylock
    int enable = false;

    vTaskDelay(pdMS_TO_TICKS(1));                                                                                                                                  // Yield to other tasks with a delay of 10 milliseconds                                                                                                                                                // Short delay for button state stabilization
    if (MainEnable && Button.menu.isPressed() && Button.back.isNotPressed() && Button.up.isNotPressed() && Button.down.isNotPressed() && Button.ok.isNotPressed()) // If main is enabled and menu button is pressed
    {
        Serial.println("Menu Enable"); // Log menu enable action
        vTaskDelay(pdMS_TO_TICKS(10)); // Short delay for button state stabilization

        // Menu access no longer waits on a LoRaWAN downlink (LoraFun.DataReceived):
        // settings are configured locally from this menu, not pushed from the cloud.
        while (Button.menu.isPressed()) // While menu button is pressed
        {
            if (Button.up.isPressed()) // If up button is pressed
            {
                EnableCount++;    // Increment enable counter
                DisableCount = 0; // Reset disable counter
                enable = true;    // Set enable flag
                Button.up.wait(); // Wait for button release
            }

            if (Button.down.isPressed()) // If down button is pressed
            {
                EnableCount = 0;    // Reset enable counter
                DisableCount++;     // Increment disable counter
                enable = true;      // Set enable flag
                Button.down.wait(); // Wait for button release
            }

            esp_task_wdt_reset();
            vTaskDelay(pdMS_TO_TICKS(1)); // Yield to other tasks with a delay of 1 milliseconds
        }
        if (EnableCount > 2 && !Keylock) // If enable counter exceeds threshold and keylock is not active
        {
            Keylock = true;       // Activate keylock
            KeylockEnable = true; // Set keylock enable flag
            Eeprom.write();       // Write to EEPROM
        }
        else if (DisableCount > 2 && Keylock) // If disable counter exceeds threshold and keylock is active
        {
            Keylock = false;       // Deactivate keylock
            KeylockDisable = true; // Set keylock disable flag
            Eeprom.write();        // Write to EEPROM
        }
        else if (!enable && !Keylock) // If no enable/disable action and keylock is not active
        {
            MainEnableWithReset = true;   // Set main enable with reset flag
            MenuResetnotCompleted = true; // Set menu reset not completed flag

            if (GreaseOut.Lubrication.Enable || GreaseOut.TestRun.Enable) // If lubrication or test run is enabled
            {
                MenuSettingMotorOff = true; // Set menu setting motor off flag
            }
            if (GreaseOut.Lubrication.Enable) // If lubrication is enabled
            {
                oldStateCount = 1; // Set old state count for lubrication
            }
            else if (GreaseOut.PauseTime.Enable) // If pause time is enabled
            {
                oldStateCount = 2; // Set old state count for pause time
            }
            else if (GreaseOut.TestRun.Enable) // If test run is enabled
            {
                oldStateCount = 3; // Set old state count for test run
            }
            GreaseOut.Lubrication.Enable = false; // Disable lubrication
            GreaseOut.PauseTime.Enable = false;   // Disable pause time
            GreaseOut.Reservoir.Enable = false;   // Disable reservoir
            Motor.off();
            MenuEnable = true;                       // Enable menu
            MenuSettingEnableDisableCheck = true;    // Set menu setting enable/disable check flag
            MainEnable = false;                      // Disable main
            LoraDetails.UserEnterOfflineMode = true; // Enable offline mode
        }
    }
}

/**********************
 * MENU PAGE FUNCTION
 * Handles menu page logic and navigation
 **********************/
void MENUPAGE::Page()
{
    MenuSetting.MEnu.Setcursor();                                                                                // Update menu cursor position
    MenuSetting.Config.Setcursor();                                                                              // Update config cursor position
    MenuSetting.Setting.Setcursor();                                                                             // Update setting cursor position
    MenuSetting.Refill.Setcursor();                                                                              // Update refill cursor position
    if (!ConfigEnable && !SettingEnable && !RefillEnable && !RefillModeEnable && !ResetEnable && !TestRunEnable) // If no other modes are enabled
    {
        if (Button.ok.isPressed() && Button.menu.isNotPressed() && Button.back.isNotPressed() && Button.up.isNotPressed() && Button.down.isNotPressed()) // If OK button is pressed
        {
            if (MenuFunSetcursor == 12) // If CONFIG option is selected and no alert is active
            {
                ConfigEnable = true; // Enable configuration mode
                MenuEnable = false;  // Disable menu
            }
            else if (MenuFunSetcursor == 24) // If SETTING option is selected and no alert is active
            {
                SettingEnable = true;    // Enable setting mode
                MenuEnable = false;      // Disable menu
                LcdUI.scrollViewReset(); // Reset scroll view
            }
            else if (MenuFunSetcursor == 36) // If REFILL option is selected
            {
                RefillEnable = true; // Enable refill mode
                MenuEnable = false;  // Disable menu
            }
            else if (MenuFunSetcursor == 48)
            {
                ResetEnable = true; // Enable reset mode
                MenuEnable = false; // Disable menu
            }
            else if (MenuFunSetcursor == 60)
            {
                TestRunEnable = true; // Enable test run mode
                MenuEnable = false;   // Disable menu
            }

            Button.ok.wait(); // Wait for button release
        }
    }
}

/**********************
 * MENU BACK FUNCTION
 * Handles back button functionality and state transitions
 **********************/
void MENUFUNCTION::BackFunction()
{
    if (Button.back.isPressed() && Button.menu.isNotPressed() && Button.ok.isNotPressed() && Button.up.isNotPressed() && Button.down.isNotPressed()) // If Back button is pressed
    {
        if (MenuEnable) // If menu is enabled
        {
            MainPage.pageEnable(); // Enable main page
        }
        else if (ConfigEnable) // If configuration mode is enabled
        {
            MenuSetting.MEnu.Reset();   // Reset menu
            MenuSetting.Config.Reset(); // Reset configuration
            ConfigEnable = false;       // Disable configuration mode
        }
        else if (SettingEnable) // If setting mode is enabled
        {
            MenuSetting.MEnu.Reset();    // Reset menu
            MenuSetting.Setting.Reset(); // Reset setting
            SettingEnable = false;       // Disable setting mode
        }
        else if (RefillEnable)
        {
            MenuSetting.MEnu.Reset(); // Reset menu
            RefillFunSetcursor = 12;  // Reset refill cursor position
            RefillEnable = false;     // Disable refill mode
        }
        else if (RefillModeEnable) // If refill mode is enabled
        {
            MenuSetting.MEnu.Reset(); // Reset menu
            RefillModeEnable = false; // Disable refill mode
        }
        else if (TestRunEnable) // If test run mode is enabled
        {
            MenuSetting.MEnu.Reset(); // Reset menu
            TestRunEnable = false;    // Disable test run mode
        }
        else if (ResetEnable) // If reset mode is enabled
        {
            MenuSetting.MEnu.Reset(); // Reset menu
            ResetEnable = false;      // Disable reset mode
        }

        Button.back.wait(); // Wait for button release
    }
}

/**********************
 * MENU PAGE RESET FUNCTION
 * Resets menu page state
 **********************/
void MENUPAGE::Reset()
{
    MenuEnable = true; // Enable menu
}

/**********************
 * MENU PAGE SET CURSOR FUNCTION
 * Manages cursor movement and selection on menu page
 * **********************/

void MENUPAGE::Setcursor()
{
    if (MenuEnable) // If menu is enabled
    {
        if (Button.down.isPressed()) // If Down button is pressed
        {
            MenuFunSetcursor += 12;    // Move cursor down by 12
            if (MenuFunSetcursor > 60) // If cursor exceeds maximum position
                MenuFunSetcursor = 12; // Wrap around to the top
            Button.down.wait();        // Wait for button release
        }

        if (Button.up.isPressed()) // If Up button is pressed
        {
            MenuFunSetcursor -= 12;    // Move cursor up by 12
            if (MenuFunSetcursor < 12) // If cursor goes below minimum position
                MenuFunSetcursor = 60; // Wrap around to the bottom
            Button.up.wait();          // Wait for button release
        }
    }
}

unsigned long previousMillis = 0;
const unsigned long interval = 1000; // 1000 ms = 1 second

void MENUPAGE::_serial_monitor_to_check(String data)
{
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval)
    {
        previousMillis = currentMillis;

        // Code that runs every 1 second
        Serial.println(data);
    }
}

unsigned long previousMillis_1 = 0;

void MENUPAGE::_serial_monitor_to_checkFrontend(String data)
{
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis_1 >= interval)
    {
        previousMillis_1 = currentMillis;

        // Code that runs every 1 second
        Serial.println(data);
    }
}
