/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note Sleep mode animation display management on the LCD.
 */

#include "Headerfile.h"
#include "Lcd/Logo/SleepModeLogo1.h"
#include "Lcd/Logo/SleepModeLogo2.h"
#include "Lcd/Logo/SleepModeLogo3.h"

/**********************
 * SLEEP MODE FUNCTION
 * Manages the display of sleep mode animation on the LCD
 **********************/
enum SleepAnimationState
{
    SleepFrame1,
    SleepFrame2,
    SleepFrame3
};

void LcdUIFunction::SleepMode()
{

    display.clearDisplay();

    if (Sleep_count == SleepFrame1)
    {                                                         // Clear the display buffer
        display.drawBitmap(10, 0, sleepmodelogo1, 70, 70, 1); // Draw the first sleep mode logo
    }
    else if (Sleep_count == SleepFrame2)
    {                                                         // Clear the display buffer
        display.drawBitmap(10, 0, sleepmodelogo2, 70, 70, 1); // Draw the second sleep mode logo
    }
    else if (Sleep_count == SleepFrame3)
    {                                                         // Clear the display buffer
        display.drawBitmap(10, 0, sleepmodelogo3, 70, 70, 1); // Draw the third sleep mode logo
    }

    display.display(); // Update the display with the buffer content

    if (millis() - Sleep_previousMillis >= Sleep_interval) // Check if it's time to update the animation frame
    {
        Sleep_previousMillis = millis(); // Update the last update time
        Sleep_count++;                   // Increment the animation frame counter

        if (Sleep_count > 2)
            Sleep_count = 0;
    }
}