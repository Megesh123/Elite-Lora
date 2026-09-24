/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note Alert page frontend logic and display management.
 */

#include "Headerfile.h"
#include "Lcd/Logo/OverloadLogo.h"

/**********************
 * ALERT PAGE FUNCTION
 * Manages the display of various alert messages on the LCD
 **********************/
void LcdUIFunction::AlertPage()
{
    display.clearDisplay();                         // Clear the display buffer
    display.drawBitmap(0, 10, Overload, 50, 50, 1); // Draw the overload logo
    LcdDisplay.Boder();                             // Draw the border on the display
    _aletPage_Setcursor();                          // Handle cursor setting for alert page
    _aletPage_Overload();                           // Handle overload alert display
    _alertPage_ReservoirLowLevel();                 // Handle reservoir low level alert display
    _alertPage_Supervisor_1();                      // Handle supervisory 1 alert display
    _alertPage_Supervisor_2();                      // Handle supervisory 2 alert display
    _alertPage_Supervisor_3();                      // Handle supervisory 3 alert display
    _alertPage_SignalNotDetected();                 // Handle signal not detected alert display
    _alertPage_Pump_Element_Health_1();
    _alertPage_Pump_Element_Health_2();
    _alertPage_Pump_Element_Health_3();
    _alertPage_Unknow();         // Handle unknown alert display
    _alertPage_ResetCompleted(); // Handle reset completed alert display
    display.display();           // Update the display with the buffer content
}

/**********************
 * WARNING PAGE FUNCTIONS
 * Display various warning messages on the LCD
 **********************/
void LcdUIFunction::_waring_TankCapacity()
{
    LcdDisplay.print(20, 30, PRINT_SET_CAPACITY); // Display "SET CAPACITY" message

    if (_shouldBlink(ArrowlastBlinkTime, ArrowblinkState)) // Check if blinking condition is met
    {
        LcdDisplay.print(57, 59, PRINT_PRESS_BACK); // Display back prompt
    }
}

/**********************
 * WARNING PAGE FUNCTIONS
 * Display various warning messages on the LCD
 **********************/
void LcdUIFunction::_waring_ReservoirLowLevel()
{
    LcdDisplay.print(10, 30, PRINT_RESERVOIR_LOW_LEVEL); // Display "RESERVOIR LOW LEVEL" message
    LcdDisplay.print(10, 40, PRINT_REFILL_NOW_NEED);     // Display "REFILL NOW? NEED" message

    if (_shouldBlink(ArrowlastBlinkTime, ArrowblinkState)) // Check if blinking condition is met
    {
        LcdDisplay.print(57, 59, PRINT_PRESS_BACK); // Display back prompt
    }
}

/**********************
 * WARNING PAGE FUNCTIONS
 * Display various warning messages on the LCD
 **********************/
void LcdUIFunction::_waring_Outlet()
{
    LcdDisplay.print(10, 30, PRINT_SET_OUTLET); // Display "SET OUTLET" message

    if (_shouldBlink(ArrowlastBlinkTime, ArrowblinkState)) // Check if blinking condition is met
    {
        LcdDisplay.print(57, 59, PRINT_PRESS_BACK); // Display back prompt
    }
}

/**********************
 * ALERT PAGE SUB-FUNCTIONS
 * Handle specific alert scenarios and display logic
 **********************/
void LcdUIFunction::_aletPage_Setcursor()
{
    unsigned long currentTime = millis(); // Get the current time in milliseconds
    if (blinkState)                       // If blink state is true
    {
        if (currentTime - lastBlinkTime >= 700) // Check if 700 milliseconds have passed
        {
            blinkState = false;          // Set blink state to false
            lastBlinkTime = currentTime; // Update last blink time
        }
    }
    else
    {
        if (currentTime - lastBlinkTime >= 300) // Check if 300 milliseconds have passed
        {
            blinkState = true;           // Set blink state to true
            lastBlinkTime = currentTime; // Update last blink time
        }
    }
}

/**********************
 * ALERT PAGE SUB-FUNCTIONS
 * Handle specific alert scenarios and display logic
 **********************/
void LcdUIFunction::_aletPage_Overload()
{
    if (blinkState) // If blink state is true
    {
        if (Alert.NameCount == 1) // If alert name count is 1
        {
            LcdDisplay.print(50, 28, PRINT_ALERT_NAME_OVERLOAD, Alert.OverloadValue); // Display overload value
            display.print(PRINT_AMPS);                                                // Display overload value with unit "A"
            LcdDisplay.print(50, 38, PRINT_ALERT);                                    // Display PRINT_ALERT message

            Led.overLoad();                           // Indicate overload condition with LED
            LcdDisplay.print(59, 59, PRINT_RESET_OK); // Display reset prompt
        }
    }
}

/**********************
 * ALERT PAGE SUB-FUNCTIONS
 * Handle specific alert scenarios and display logic
 **********************/
void LcdUIFunction::_alertPage_ReservoirLowLevel()
{
    if (blinkState) // If blink state is true
    {
        if (Alert.NameCount == 2) // If alert name count is 2
        {
            LcdDisplay.print(50, 25, PRINT_ALERT_NAME_RESERVOIR_1); // Display "RESERVOIR" message
            LcdDisplay.print(50, 35, PRINT_ALERT_NAME_RESERVOIR_2); // Display "LOW LEVEL" message
            LcdDisplay.print(50, 45, PRINT_ALERT);                  // Display PRINT_ALERT message

            Led.reservoir(); // Indicate reservoir low level condition with LED
        }
    }
}

/**********************
 * ALERT PAGE SUB-FUNCTIONS
 * Handle specific alert scenarios and display logic
 **********************/
void LcdUIFunction::_alertPage_Supervisor_1()
{
    if (blinkState) // If blink state is true
    {
        if (Alert.NameCount == 3) // If alert name count is 3
        {
            LcdDisplay.print(50, 28, PRINT_ALERT_NAME_SUPERVISORY_1); // Display supervisory 1 message
            LcdDisplay.print(50, 38, PRINT_ALERT);                    // Display alert message

            Led.Supervisory();                        // Indicate supervisory condition with LED
            LcdDisplay.print(59, 59, PRINT_RESET_OK); // Display reset prompt
        }
    }
}

/**********************
 * ALERT PAGE SUB-FUNCTIONS
 * Handle specific alert scenarios and display logic
 **********************/
void LcdUIFunction::_alertPage_Supervisor_2()
{
    if (blinkState) // If blink state is true
    {
        if (Alert.NameCount == 4) // If alert name count is 4
        {
            LcdDisplay.print(50, 28, PRINT_ALERT_NAME_SUPERVISORY_2); // Display supervisory 2 message
            LcdDisplay.print(50, 38, PRINT_ALERT);                    // Display alert message

            Led.Supervisory();                        // Indicate supervisory condition with LED
            LcdDisplay.print(59, 59, PRINT_RESET_OK); // Display reset prompt
        }
    }
}

/**********************
 * ALERT PAGE SUB-FUNCTIONS
 * Handle specific alert scenarios and display logic
 **********************/
void LcdUIFunction::_alertPage_Supervisor_3()
{
    if (blinkState) // If blink state is true
    {
        if (Alert.NameCount == 5) // If alert name count is 5
        {
            LcdDisplay.print(50, 28, PRINT_ALERT_NAME_SUPERVISORY_3); // Display supervisory 3 message
            LcdDisplay.print(50, 38, PRINT_ALERT);                    // Display alert message

            Led.Supervisory();                        // Indicate supervisory condition with LED
            LcdDisplay.print(59, 59, PRINT_RESET_OK); // Display reset prompt
        }
    }
}

/**********************
 * ALERT PAGE SUB-FUNCTIONS
 * Handle specific alert scenarios and display logic
 **********************/
void LcdUIFunction::_alertPage_SignalNotDetected()
{
    if (blinkState) // If blink state is true
    {
        if (Alert.NameCount == 6) // If alert name count is 6
        {
            LcdDisplay.print(50, 28, PRINT_ALERT_NAME_SIGNAL_NOT_DETECTED_1); // Display signal message
            LcdDisplay.print(50, 38, PRINT_ALERT_NAME_SIGNAL_NOT_DETECTED_2); // Display not detected message

            Led.proximityNotSense();                  // Indicate proximity sensor not sensing condition with LED
            LcdDisplay.print(59, 59, PRINT_RESET_OK); // Display reset prompt
        }
    }
}

void LcdUIFunction::_alertPage_Pump_Element_Health_1()
{
    if (Alert.NameCount == 7)
    {
        LcdDisplay.print(50, 28, PRINT_ALERT_NAME_SUPERVISORY_1);
        LcdDisplay.print(50, 38, PRINT_ALERT_NAME_ELEMENT_WEAK);

        LcdDisplay.print(59, 59, PRINT_RESET_OK);
    }
}

void LcdUIFunction::_alertPage_Pump_Element_Health_2()
{
    if (Alert.NameCount == 8)
    {
        LcdDisplay.print(50, 28, PRINT_ALERT_NAME_SUPERVISORY_2);
        LcdDisplay.print(50, 38, PRINT_ALERT_NAME_ELEMENT_WEAK);

        LcdDisplay.print(59, 59, PRINT_RESET_OK);
    }
}

void LcdUIFunction::_alertPage_Pump_Element_Health_3()
{
    if (Alert.NameCount == 9)
    {
        LcdDisplay.print(50, 28, PRINT_ALERT_NAME_SUPERVISORY_3);
        LcdDisplay.print(50, 38, PRINT_ALERT_NAME_ELEMENT_WEAK);

        LcdDisplay.print(59, 59, PRINT_RESET_OK);
    }
}

/**********************
 * ALERT PAGE SUB-FUNCTIONS
 * Handle specific alert scenarios and display logic
 **********************/
void LcdUIFunction::_alertPage_Unknow()
{
    if (blinkState) // If blink state is true
    {
        // Alert codes 1-9 are all valid. Codes 7-9 are pump-element health alerts;
        // treating them as unknown overlays this message on their alert screen.
        if (Alert.NameCount < 1 || Alert.NameCount > 9) // If alert name count is invalid
        {
            LcdDisplay.print(50, 28, PRINT_ALERT_NAME_UNKNOW); // Display unknown error message
        }
    }
}

/**********************
 * ALERT PAGE SUB-FUNCTIONS
 * Handle specific alert scenarios and display logic
 **********************/
void LcdUIFunction::_alertPage_ResetCompleted()
{
    if (Button.ok.isPressed() && Alert.NameCount != 2) // If OK button is pressed and alert is not overload
    {
        Button.ok.wait(); // Consume the press

        for (int i = 0; i < 3; i++) // Loop to create reset completed effect
        {

            display.clearDisplay();                          // Clear the display buffer
            LcdDisplay.print(20, 30, PRINT_RESET_COMPLETED); // Display reset completed message
            display.display();                               // Update the display with the buffer content
            digitalWrite(PIN_BUZZER, HIGH);                  // Turn on buzzer
            vTaskDelay(pdMS_TO_TICKS(700));                  // Delay for 700 milliseconds
            display.clearDisplay();                          // Clear the display buffer
            display.setCursor(20, 30);                       // Set cursor position
            display.display();                               // Update the display with the buffer content
            digitalWrite(PIN_BUZZER, LOW);                   // Turn off buzzer
            vTaskDelay(pdMS_TO_TICKS(300));                  // Delay for 300 milliseconds
        }

        MenuEnable = false;                       // Disable menu
        MainEnable = true;                        // Enable main
        MenuFunSetcursor = 12;                    // Reset menu cursor position
        LoraDetails.UserEnterOfflineMode = false; // Disable offline mode
        VariableUpdate();
    }
}
