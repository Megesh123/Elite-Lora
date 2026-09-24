/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note Configuration page frontend logic and display management.
 */

#include "Headerfile.h"

/**********************
 * CONFIG PAGE FUNCTION
 * Manages configuration page display and interactions
 **********************/
void LcdUIFunction::_ConfigPage()
{
    if (ConfigEnable) // While configuration mode is enabled
    {
        display.clearDisplay();           // Clear the display buffer
        LcdDisplay.Boder();               // Draw border on the display
        LcdUI._configPage_TankCapacity(); // Display tank capacity configuration
        LcdUI._configPage_Outlet();       // Display outlet configuration
        display.display();                // Update the display with the buffer content
        _configPage_Setcursor();
        vTaskDelay(pdMS_TO_TICKS(1)); // Yield to other tasks with a delay of 10 milliseconds
    }
}

/**********************
 * CONFIG PAGE TANK CAPACITY DISPLAY FUNCTION
 * Renders tank capacity configuration on the display
 **********************/
void LcdUIFunction::_configPage_TankCapacity()
{
    LcdDisplay.print(20, 12, PRINT_CAPACITY); // Print "CAPACITY :" at (20,12)
    if (GreaseOut.Reservoir.Capacity == 0)    // If reservoir capacity is zero
    {
        LcdDisplay.print0(); // Print "0" to indicate zero capacity
    }
    display.print(GreaseOut.Reservoir.Capacity, 0); // Print the reservoir capacity value
    display.print(PRINT_GRAM);                      // Print "G" to indicate grams
}

/**********************
 * CONFIG PAGE OUTLET DISPLAY FUNCTION
 * Renders outlet configuration on the display
 **********************/
void LcdUIFunction::_configPage_Outlet()
{
    LcdDisplay.print(20, 24, PRINT_NO_OF_OUTLET); // Print "NO OF OUTLETS :" at (20,24)
    LcdDisplay.print0();                          // Print "0" to indicate zero outlets
    display.print(Config.NoOfOutlet, 0);          // Print the number of outlets value

    LcdDisplay.print(20, 36, "OTA VERSION: "); // Print "OTA VERSION:" at (20,36)
    display.print(OTA.current_version);        // Print the OTA version value
}

/**********************
 * CONFIGURATION PAGE FUNCTION
 * Handles configuration page logic and variable adjustments
 **********************/
void LcdUIFunction::_configPage_Setcursor()
{
    if (VariableSeleteEnable) // If variable selection is enabled
    {
        LcdDisplay.print(10, ConfigFunSetcursor, PRINT_RIGHT_ARROW);            // Print cursor arrow
        LcdDisplay._DrawStraightLine(7, ConfigFunSetcursor - 4, 5, HORIZONTAL); // Draw horizontal line above the selected option
    }
    else
    {
        if (_shouldBlink(ArrowlastBlinkTime, ArrowblinkState)) // If it's time to blink the arrow
        {
            LcdDisplay.print(10, ConfigFunSetcursor, PRINT_RIGHT_ARROW);            // Print cursor arrow
            LcdDisplay._DrawStraightLine(7, ConfigFunSetcursor - 4, 5, HORIZONTAL); // Draw horizontal line above the selected option
        }
    }
}