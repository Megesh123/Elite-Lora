/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note Reset page backend logic and user interactions
 */

#include "Headerfile.h"

/**********************
 * RESET PAGE FUNCTION
 * Manages reset page logic and user interactions
 **********************/
void LcdUIFunction::_ResetPage()
{
    if (ResetEnable) // While reset mode is enabled
    {
        display.clearDisplay();       // Clear the display buffer
        LcdDisplay.Boder();           // Draw border on the display
        _resetPage_Setcursor();       // Handle cursor movement
        _resetPage_FrontPage();       // Display the main reset page
        _resetPage_ResetCompleted();  // Handle reset completion status
        display.display();            // Update the display with the buffer content
        vTaskDelay(pdMS_TO_TICKS(1)); // Short delay for button state stabilization
    }
}

/**********************
 * RESET PAGE SET CURSOR FUNCTION
 * Manages cursor movement and selection on reset page
 **********************/
void LcdUIFunction::_resetPage_Setcursor()
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
            LcdDisplay.printOk();                              // Display OK arrow
        }
    }
}

/**********************
 * RESET PAGE RESET COMPLETED FUNCTION
 * Displays reset completion status and resets flags
 **********************/
void LcdUIFunction::_resetPage_ResetCompleted()
{
    if (ResetCompleted) // If reset process is completed
    {
        for (int i = 0; i < 3; i++) // Flash completion message 3 times
        {
            display.clearDisplay();                          // Clear the display buffer
            LcdDisplay.print(20, 30, PRINT_RESET_COMPLETED); // Display reset completed message
            display.display();                               // Update the display
            digitalWrite(PIN_BUZZER, HIGH);
            vTaskDelay(pdMS_TO_TICKS(700)); // Wait for 700ms
            display.clearDisplay();         // Clear the display buffer
            display.setCursor(20, 30);      // Set cursor position
            display.display();              // Update the display
            digitalWrite(PIN_BUZZER, LOW);
            vTaskDelay(pdMS_TO_TICKS(300)); // Wait for 300ms
        }

        ResetCompleted = false;                                // Reset reset completed flag
        ResetEnable = false;                                   // Disable reset mode
        MenuEnable = false;                                    // Disable menu mode
        MainEnable = true;                                     // Enable main mode
        MenuFunSetcursor = 12;                                 // Reset menu cursor position                             // Set reset flag
        LoraDetails.UserEnterOfflineMode = false;              // Clear LoRa offline mode flag
        Reset_SendData_Lora = true;                            // Set send data via LoRa flag
        SupervisoryTime.SupervisoryAverageTakeOutGrease_1 = 0; // Reset supervisory average grease taken out
        SupervisoryTime.SupervisoryAverageTakeOutGrease_2 = 0; // Reset supervisory average grease taken out
        SupervisoryTime.SupervisoryAverageTakeOutGrease_3 = 0; // Reset supervisory average grease taken out
        SupervisoryTime.AlertCheckOutGrease = 0;               // Reset supervisory alert check grease
        VariableUpdate();
        // Eeprom.write();
        vTaskDelay(pdMS_TO_TICKS(100)); // Yield to other tasks with a delay of 100 milliseconds

        // ESP.restart(); // Restart the ESP device
    }
}

/**********************
 * RESET PAGE FRONT PAGE FUNCTION
 * Displays the main reset page
 **********************/
void LcdUIFunction::_resetPage_FrontPage()
{
    LcdDisplay.print(20, 12, PRINT_RESET); // Print "RESET" at (20,12)
}