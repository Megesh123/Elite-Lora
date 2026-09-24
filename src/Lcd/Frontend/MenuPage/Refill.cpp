/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note Refill page backend logic and user interaction management.
 */

#include "Headerfile.h"

/**********************
 * REFILL PAGE FUNCTION
 * Manages refill page logic and user interactions
 **********************/
void LcdUIFunction::_RefillPage()
{
    if (RefillEnable) // If refill mode is enabled
    {
        display.clearDisplay(); // Clear the display buffer
        LcdDisplay.Boder();     // Draw border on the display

        if (GreaseOut.Reservoir.Capacity > 0) // If reservoir capacity is greater than zero
        {
            if (Alert.EnableDisable.RefillAlert)
            {
                _refillPage_FrontPage();       // Display the main refill page
                _refillPage_Setcursor();       // Handle cursor blinking
                _refillPage_refillFrontPage(); // Display refill confirmation prompt
                _refillPage_RefillStart();     // Display refill start status
                _refillPage_RefillStop();      // Display refill stop status
                _refillPage_RefillCompleted(); // Display refill completion status
            }
            else
            {
                LcdDisplay.print(20, 30, "DISABLE"); // Prompt to set reservoir capacity
            }
        }
        else
        {
            LcdDisplay.print(20, 30, PRINT_SET_CAPACITY);          // Prompt to set reservoir capacity
            if (_shouldBlink(ArrowlastBlinkTime, ArrowblinkState)) // If it's time to blink the arrow
            {
                LcdDisplay.printBack(); // Print back button prompt
            }
        }

        display.display();            // Update the display with the buffer content
        vTaskDelay(pdMS_TO_TICKS(1)); // Yield to other tasks with a delay of 10 milliseconds
    }
}

/**********************
 * REFILL PAGE SET CURSOR FUNCTION
 * Manages cursor blinking on the refill page
 **********************/
void LcdUIFunction::_refillPage_Setcursor()
{
    unsigned long currentTime = millis(); // Get the current time in milliseconds
    if (blinkState)                       // If blink state is ON
    {
        if (currentTime - lastBlinkTime >= 700) // If 700ms have passed since last blink
        {
            blinkState = false;          // Turn OFF
            lastBlinkTime = currentTime; // Update last blink time
        }
    }
    else
    {
        if (currentTime - lastBlinkTime >= 300) // If 300ms have passed since last blink
        {
            blinkState = true;           // Turn ON
            lastBlinkTime = currentTime; // Update last blink time
        }
    }
}

/**********************
 * REFILL PAGE REFILL FRONT PAGE FUNCTION
 * Displays refill confirmation prompt
 **********************/
void LcdUIFunction::_refillPage_refillFrontPage()
{
    if (RefillConfirmation) // If refill confirmation is active
    {
        LcdDisplay.print(10, 20, PRINT_ASK_REFILL_MODE); // Prompt user for refill confirmation
        LcdDisplay.print(30, 30, PRINT_REFILL_MODE);     // Prompt user for refill confirmation

        if (!blinkState) // If blink state is OFF
        {
            if (RefillConfirmationYesNo) // If user selected 'No'
            {
                LcdDisplay.print(95, 55, PRINT_NO); // Highlight 'No' option
            }
            else
            {
                LcdDisplay.print(20, 55, PRINT_YES); // Highlight 'Yes' option
            }
        }
        else
        {
            LcdDisplay.print(20, 55, PRINT_YES); // Print 'Yes' option
            LcdDisplay.print(95, 55, PRINT_NO);  // Print 'No' option
        }
    }
}

/**********************
 * REFILL PAGE REFILL START FUNCTION
 * Displays refill start status and handles user input
 **********************/
void LcdUIFunction::_refillPage_RefillStart()
{
    if (RefillStart) // If refill process is active
    {
        LcdDisplay.print(20, 30, PRINT_REFILL_START); // Display refill start status

        Led.Testrun_on();                                      // Turn on test run LED indicator
        if (_shouldBlink(ArrowlastBlinkTime, ArrowblinkState)) // If it's time to blink the arrow
        {
            if (RefillFunSetcursor == 24) // If cursor is on the stop option
            {
                LcdDisplay.print(30, 59, PRINT_OK_STOP); // Prompt to stop refill
            }
        }
    }
}

/**********************
 * REFILL PAGE REFILL STOP FUNCTION
 * Displays refill stop status and handles user input
 **********************/
void LcdUIFunction::_refillPage_RefillStop()
{
    if (RefillStop) // If refill stop process is active
    {
        LcdDisplay.print(20, 30, PRINT_REFILL_STOP);           // Display refill stop status
        Led.Testrun_off();                                     // Turn off test run LED indicator
        if (_shouldBlink(ArrowlastBlinkTime, ArrowblinkState)) // If it's time to blink the arrow
        {
            if (RefillFunSetcursor == 24) // If cursor is on the complete option
            {
                LcdDisplay.print(5, 59, PRINT_OK_COMPLETED); // Prompt to complete refill
            }
        }
    }
}

/**********************
 * REFILL PAGE REFILL COMPLETED FUNCTION
 * Displays refill completion status and resets flags
 **********************/
void LcdUIFunction::_refillPage_RefillCompleted()
{
    if (RefillCompleted) // If refill process is completed
    {
        for (int i = 0; i < 3; i++) // Flash completion message 3 times
        {
            display.clearDisplay();
            LcdDisplay.print(10, 30, PRINT_REFILL_SUCCESS);   // Display success message
            LcdDisplay.print(35, 40, PRINT_REFILL_COMPLETED); // Display completed message
            display.display();                                // Update the display

            Led.Testrun_on();               // Turn on test run LED indicator
            vTaskDelay(pdMS_TO_TICKS(750)); // Wait for 750ms
            display.clearDisplay();         // Clear the display buffer
            Led.Testrun_off();              // Turn off test run LED indicator
            display.display();              // Update the display
            vTaskDelay(pdMS_TO_TICKS(250)); // Wait for 250ms
        }

        DateTime now = rtc.getDateTime();
        char buffer[100];
        sprintf(buffer, "*REFILL SUCCESS - %02d/%02d/%02d %02d:%02d:%02d#\n", now.date, now.month, now.year, now.hour, now.minute, now.second);
        Lora_twp.RefillSuccessful_Data = buffer;
        Lora_twp.RefillSuccessful = true;

        RefillCompleted = false;     // Reset refill completed flag
        RefillEnable = true;         // Re-enable refill mode
        MenuSettingEditTrue = false; // Clear menu setting edit flag
    }
}

/**********************
 * REFILL PAGE FRONT PAGE FUNCTION
 * Displays the main refill page with status and options
 **********************/
void LcdUIFunction::_refillPage_FrontPage()
{

    if (!RefillConfirmation && !RefillStart && !RefillStop && !RefillCompleted) // If no refill action is active
    {

        if (VariableSeleteEnable) // If variable selection is enabled
        {
            LcdDisplay.print(10, RefillFunSetcursor, PRINT_RIGHT_ARROW);            // Print arrow at current cursor position
            LcdDisplay._DrawStraightLine(7, RefillFunSetcursor - 4, 5, HORIZONTAL); // Draw horizontal line above the selected option
        }
        else
        {
            if (_shouldBlink(ArrowlastBlinkTime, ArrowblinkState)) // If it's time to blink the arrow
            {
                LcdDisplay.print(10, RefillFunSetcursor, PRINT_RIGHT_ARROW);            // Print arrow at current cursor position
                LcdDisplay._DrawStraightLine(7, RefillFunSetcursor - 4, 5, HORIZONTAL); // Draw horizontal line above the selected option
                if (RefillFunSetcursor == 24)                                           // If cursor is on the start option
                {
                    LcdDisplay.printOk(); // Display OK arrow
                }
            }
        }

        LcdDisplay.print(20, 12, PRINT_REFILL_STATUS, int(GreaseOut.Reservoir.Percentage)); // Print reservoir status percentage
        display.print(PRINT_PERCENTAGE_SYSMBOL);                                            // Print percentage symbol

        LcdDisplay.print(20, 24, PRINT_REFILL_NOW); // Prompt to start refill

        lastBlinkTime = 0; // Reset last blink time
        blinkState = true; // Set blink state to ON
    }
}
