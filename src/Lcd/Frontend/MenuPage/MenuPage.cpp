/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note Menu setting page frontend logic and display management.
 */

#include "Headerfile.h"

/**********************
 * MENU SETTING PAGE FUNCTION
 * Manages menu setting page display and interactions
 **********************/
void LcdUIFunction::MenuSettingPage()
{
    _MenuPage(); // Render the menu page
    _ConfigPage();
    _SettingPage();
    _RefillPage();
    _ResetPage();
    _TestRunPage();
}

/**********************
 * MENU PAGE DISPLAY FUNCTION
 * Renders the menu page on the display
 **********************/
void LcdUIFunction::_MenuPage()
{
    if (MenuEnable && !ResetAllValueSet) // If menu is enabled and reset all value flag is not set
    {
        display.clearDisplay(); // Clear the display buffer
        LcdDisplay.Boder();     // Draw border on the display

        if (_shouldBlink(ArrowlastBlinkTime, ArrowblinkState)) // If it's time to blink the arrow
        {
            display.setCursor(10, MenuFunSetcursor);                       // Set cursor position for arrow
            display.print(PRINT_RIGHT_ARROW);                                            // Print arrow symbol
            LcdDisplay._DrawStraightLine(7, MenuFunSetcursor - 4, 5, HORIZONTAL); // Draw horizontal line above the selected menu item
        }

        LcdDisplay.print(20, 12, PRINT_CONFIGURATION); // Print "CONFIGURATION" at (20,12)
        LcdDisplay.print(20, 24, PRINT_SETTING);       // Print "SETTING" at (20,24)
        LcdDisplay.print(20, 36, PRINT_REFILL);        // Print "REFILL" at (20,36)
        LcdDisplay.print(20, 48, PRINT_RESET);         // Print "RESET" at (20,48)
        LcdDisplay.print(20, 60, PRINT_TESTRUN);       // Print "TEST RUN" at (20,60)

        display.display(); // Update the display with the buffer content
    }
}