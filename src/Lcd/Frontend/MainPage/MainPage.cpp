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

/**********************
 * MAIN PAGE FUNCTION
 * Manages main page display and interactions
 **********************/
void LcdUIFunction::MainPage()
{
    if (MainEnable) // If main is enabled
    {
        _mainPage();               // Call main page display function
        _testRunErrorOccur();      // Check for test run errors
        _keyLockEnable();          // Enable key lock if necessary
        _keyLockDisable();         // Disable key lock if necessary
        _displayToKeyLockEnable(); // Display key lock enable status
    }
}

/**********************
 * MAIN PAGE DISPLAY FUNCTION
 * Renders the main page on the display
 **********************/
void LcdUIFunction::_mainPage()
{
    display.clearDisplay();  // Clear the display buffer
    if (MenuSettingEditTrue) // If menu setting edit is true
    {
        LcdDisplay.print(8, 30, PRINT_RESET_TO_SAVE_CHANGES);  // Prompt user to reset to save changes
        if (_shouldBlink(ArrowlastBlinkTime, ArrowblinkState)) // If it's time to blink the arrow
        {
            LcdDisplay.printOk(); // Display OK arrow
        }
        if (Button.ok.isPressed()) // If OK button is pressed
        {
            Button.ok.wait();              // Wait for button release
            Reset = true;                  // Set reset flag
            MenuResetnotCompleted = false; // Clear menu reset not completed flag
        }
    }
    else
    {
        _OuterLine();          // Draw outer line on the display
        _displayReservoir();   // Display reservoir information
        _displayCompanyName(); // Display company name
        _SupervisoryTime();    // Display supervisory time
        _Mode();               // Display current mode
    }
    display.display(); // Update the display with the buffer content
}
