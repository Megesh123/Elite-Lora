/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note Button input device handling for user interface.
 */

#include "headerfile.h"

// Create the global Button object
BUTTONFUNCTION Button(PIN_SW_MENU, PIN_SW_BACK, PIN_SW_UP, PIN_SW_DOWN, PIN_SW_OK); // Initialize with pin numbers

/**********************
 * BUTTON WAIT FUNCTION
 * Waits until the button is released
 **********************/
bool BUTTONKEY::wait(uint32_t timeoutMs)
{
    uint32_t start = millis();
    while (digitalRead(Keypin) == BUTTON_PRESSED)
    {
        if (millis() - start >= timeoutMs)
            return false; // give up, don't hang
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    return true;
}

/**********************
 * BUTTON PRESS CHECK
 * Checks if the button is currently pressed
 **********************/
bool BUTTONKEY::isPressed()
{
    return (digitalRead(Keypin) == BUTTON_PRESSED); // Return true if button is pressed
}

/**********************
 * BUTTON NOT PRESS CHECK
 * Checks if the button is currently not pressed
 **********************/
bool BUTTONKEY::isNotPressed()
{
    return (digitalRead(Keypin) == BUTTON_NOT_PRESSED); // Return true if button is not pressed
}
