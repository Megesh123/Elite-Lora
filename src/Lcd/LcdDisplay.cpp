/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note LCD display initialization and utility functions. 
 */

#include "Headerfile.h"
#include "Lcd/Font5x7Fixed.h"

LcdDisplayFunction LcdDisplay;

/**********************
 * LCD DISPLAY INITIALIZATION
 * Sets up the LCD display with initial parameters
 **********************/
void LcdDisplayFunction::Begin()
{
    display.begin(12);              // Initialize display with contrast value 12
    display.clearDisplay();         // Clear the display buffer
    display.setFont(&Font5x7Fixed); // Set the font to Font5x7Fixed
    display.setTextSize(1);         //
    display.fillScreen(Black);      // Fill the screen with black color
    display.setTextColor(White);    // Set text color to white
}

/**********************
 * DRAW BORDER FUNCTION
 * Draws a border around the LCD display
 **********************/
void LcdDisplayFunction::Boder()
{
    _DrawStraightLine(0, 0, 127, HORIZONTAL);  // Top border
    _DrawStraightLine(0, 63, 127, HORIZONTAL); // Bottom border

    _DrawStraightLine(0, 0, 64, VERTICAL);   // Left border
    _DrawStraightLine(127, 0, 64, VERTICAL); // Right border
}

/**********************
 * DRAW STRAIGHT LINE FUNCTION
 * Draws a straight line on the LCD display
 **********************/
void LcdDisplayFunction::_DrawStraightLine(char column, char row, int total, char TypeOfLine)
{
    if (TypeOfLine == HORIZONTAL) //
    {
        for (int i = 0; i < total; i++)
        {
            LcdDisplay.print(column, row, "."); // Print dot at specified position
            column = column + 1;                // Move to the next column
        }
    }
    else if (TypeOfLine == VERTICAL)
    {
        for (int j = 0; j < total; j++)
        {
            LcdDisplay.print(column, row, "."); // Print dot at specified position
            row = row + 1;                      // Move to the next row
        }
    }
}

/**********************
 * PRINT OK FUNCTION
 * Displays the "PRESS OK" prompt on the LCD
 **********************/
void LcdDisplayFunction::printOk()
{
    LcdDisplay.print(67, 59, PRINT_PRESS_OK); // Print the "PRESS OK" message at specified position
}

/**********************
 * PRINT BACK FUNCTION
 * Displays the "PRESS BACK" prompt on the LCD
 **********************/
void LcdDisplayFunction::printBack()
{
    LcdDisplay.print(57, 57, PRINT_PRESS_BACK); // Print the "PRESS BACK" message at specified position
}

/**********************
 * PRINT ZERO FUNCTION
 * Prints a leading zero for formatting
 **********************/
void LcdDisplayFunction::print0()
{
    display.print(PRINT_ZERO); // Print leading zero
}

/**********************
 * PRINT FUNCTION TEMPLATE
 * Prints a value with its name at specified coordinates
 **********************/
template <typename T>
void LcdDisplayFunction::print(char _x, char _y, const char *_valueName, T _value)
{
    display.setCursor(_x, _y); // Set cursor position
    display.print(_valueName); // Print the value name
    if (_value < 10)           // If value is less than 10
        LcdDisplay.print0();   // Print leading zero

    if (std::is_floating_point<T>::value)
    {
        display.print(_value, 1); // Print floating-point value with 1 decimal place
    }
    else
    {
        display.print(_value); // Print integer value
    }
}

/**********************
 * PRINT FUNCTION OVERLOAD
 * Prints a value name at specified coordinates
 **********************/
void LcdDisplayFunction::print(char _x, char _y, const char *_valueName)
{

    display.setCursor(_x, _y); // Set cursor position
    display.print(_valueName); // Print the value name
}

/**********************
 * PRINT TIME FUNCTION TEMPLATE
 * Prints time in HH:MM format with a label at specified coordinates
 **********************/
template <typename T>
void LcdDisplayFunction::printTime(char _x, char _y, const char *_valueName, T _value1, T _value2)
{
    display.setCursor(_x, _y); // Set cursor position

    display.print(_valueName); // Print the value name

    if (_value1 < 10)        // If hours value is less than 10
        LcdDisplay.print0(); // Print leading zero
    display.print(_value1);  // Print hours value

    display.print(":");      // Print colon separator
    if (_value2 < 10)        // If minutes value is less than 10
        LcdDisplay.print0(); // Print leading zero
    display.print(_value2);  // Print minutes value
}

/**********************
 * PRINT TIME FUNCTION TEMPLATE OVERLOAD
 * Prints time in HH:MM format with a label at specified coordinates
 **********************/
template <typename T>
void LcdDisplayFunction::printTime(char _x, char _y, const char *_valueName, long _value1, T _value2)
{
    display.setCursor(_x, _y); // Set cursor position

    display.print(_valueName); // Print the value name

    if (_value1 < 10)        // If hours value is less than 10
        LcdDisplay.print0(); // Print leading zero
    display.print(_value1);  // Print hours value

    display.print(":");      // Print colon separator
    if (_value2 < 10)        // If minutes value is less than 10
        LcdDisplay.print0(); // Print leading zero
    display.print(_value2);  // Print minutes value
}

template void LcdDisplayFunction::print<int>(char, char, const char *, int);
template void LcdDisplayFunction::print<float>(char, char, const char *, float);
template void LcdDisplayFunction::printTime<int>(char, char, const char *, int, int);
