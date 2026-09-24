/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note Test run page backend logic and user interactions
 */

#include "Headerfile.h"

/**********************
 * TEST RUN PAGE FUNCTION
 * Manages test run page logic and user interactions
 **********************/
void LcdUIFunction::_TestRunPage()
{
    if (TestRunEnable) // While test run mode is enabled
    {
        display.clearDisplay();       // Clear the display buffer
        LcdDisplay.Boder();           // Draw border on the display
        _testRunPage_Setcursor();     // Handle cursor movement
        _testRunPage_FrontPage();     // Display the main test run page
        _testRunErrorOccur();         // Display error message if initialization is not completed
        display.display();            // Update the display with the buffer content
        vTaskDelay(pdMS_TO_TICKS(1)); // Yield to other tasks with a delay of 10 milliseconds
    }
}

/**********************
 * TEST RUN PAGE SET CURSOR FUNCTION
 * Manages cursor movement and selection on test run page
 **********************/
void LcdUIFunction::_testRunPage_Setcursor()
{
    if (VariableSeleteEnable) // If variable selection is enabled
    {
        LcdDisplay.print(10, 12, PRINT_RIGHT_ARROW);       // Print cursor arrow
        LcdDisplay._DrawStraightLine(7, 8, 5, HORIZONTAL); // Draw horizontal line above the selected option
    }
    else
    {
        if (_shouldBlink(ArrowlastBlinkTime, ArrowblinkState)) // If it's time to blink the arrow
        {
            LcdDisplay.print(10, 12, PRINT_RIGHT_ARROW);       // Print cursor arrow
            LcdDisplay._DrawStraightLine(7, 8, 5, HORIZONTAL); // Draw horizontal line above the selected option

            LcdDisplay.print(25, 59, PRINT_OK_ON_OFF); // Display instruction to toggle test run
        }
    }
}

/**********************
 * TEST RUN PAGE FRONT PAGE FUNCTION
 * Displays the main test run page with status and options
 **********************/
void LcdUIFunction::_testRunPage_FrontPage()
{
    LcdDisplay.print(20, 12, PRINT_TEST_RUN); // Print "TEST RUN" at (20,12)
    if (GreaseOut.TestRun.Enable)             // If test run mode is enabled
    {
        display.print(PRINT_ON); // Print "ON"
    }
    else if (!GreaseOut.TestRun.Enable) // If test run mode is disabled
    {
        display.print(PRINT_OFF); // Print "OFF"
    }
}

/**********************
 * TEST RUN PAGE ERROR OCCUR FUNCTION
 * Displays error message if initialization is not completed
 **********************/
void LcdUIFunction::_testRunErrorOccur()
{
    if (TestRunEnableError) // If there is an initialization error
    {
        display.clearDisplay();                        // Clear the display buffer
        LcdDisplay.Boder();                            // Draw border on the display
        LcdDisplay.print(20, 30, PRINT_TEST_RUN_INIT); // Print "INITIALIZATION" message
        LcdDisplay.print(20, 42, PRINT_NOT_COMPLETED); // Print "NOT COMPLETED" message
        display.display();                             // Update the display with the buffer content
        vTaskDelay(pdMS_TO_TICKS(3000));               // Yield to other tasks with a delay of 10 milliseconds
        TestRunEnableError = false;                    // Reset the error flag
    }
}
