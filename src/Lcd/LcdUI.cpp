/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note LCD display management based on system states.
 */

#include "Headerfile.h"
#include "Lcd/Logo/logo.h"

LcdUIFunction LcdUI;

/**********************
 * LCD UI MAIN PAGE FUNCTION
 * Manages the display of the main page on the LCD
 **********************/
void LcdUIFunction::CheckAllValueSetOrNot()
{
    display.clearDisplay();                  // Clear the display buffer
    if (GreaseOut.Reservoir.Percentage < 10) // Check if reservoir level is below 10%
    {
        _waring_ReservoirLowLevel(); // Display reservoir low level warning
    }
    else if (Config.NoOfOutlet < 1) // Check if number of outlets is less than 1
    {
        _waring_Outlet(); // Display outlet warning
    }
    display.display(); // Update the display with the buffer content
}

/**********************
 * LCD UI LOGO FUNCTION
 * Manages the display of the logo on the LCD
 **********************/
void LcdUIFunction::Logo()
{
    display.clearDisplay();                           // Clear the display buffer
    display.drawBitmap(0, 0, epd_bitmap, 128, 64, 1); // Draw the logo bitmap at position (0,0)
    display.display();                                // Update the display with the buffer content
}

/**********************
 * LCD UI TEST RUN FUNCTION
 * Manages the display of the test run page on the LCD
 **********************/
void LcdUIFunction::Testrun()
{
    display.clearDisplay(); // Clear the display buffer
    LcdDisplay.Boder();     // Draw the border on the display
    _supervisoryOutLine();  // Draw the supervisory section lines
    _SupervisoryTime();     // Display supervisory time

    LcdDisplay.print(10, 25, PRINT_TESTRUN); // Display "TEST RUN" text
    LcdDisplay.printTime(82, 25, PRINT_EMPTY, GreaseOut.TestRun.min, GreaseOut.TestRun.sec);

    LcdDisplay._DrawStraightLine(75, 14, 40, HORIZONTAL); // Draw horizontal line
    LcdDisplay._DrawStraightLine(114, 14, 15, VERTICAL);  // Draw vertical line
    LcdDisplay._DrawStraightLine(75, 29, 40, HORIZONTAL); // Draw horizontal line
    LcdDisplay._DrawStraightLine(75, 14, 15, VERTICAL);   // Draw vertical line

    display.display(); // Update the display with the buffer content
}

/**********************
 * SCROLLBAR CALCULATION FUNCTION
 * Manages the calculation and drawing of the scrollbar on the LCD
 **********************/
void LcdUIFunction::_drawScrollBar(int &_totalItems, int &_maxVisibleItems, int &_scrollOffset)
{
    if (_totalItems <= _maxVisibleItems) // No need for scrollbar
    {
        // No scrolling needed — skip drawing the scrollbar
        return;
    }

    int barHeight = (64.0 / _totalItems) * _maxVisibleItems; // height of scrollbar thumb
    int barPos = (64.0 / _totalItems) * _scrollOffset;       // position of scrollbar thumb
    display.drawRect(122, 0, 5, 64, White);                  // scrollbar track
    display.fillRect(123, barPos, 3, barHeight, White);      // scrollbar thumb
}

/**********************
 * SCROLLBAR CALCULATION FUNCTION
 * Manages the calculation of scrollbar position based on button presses
 **********************/
void LcdUIFunction::ScroolBarCalculation(char &_Setcursor, int &_Space, int &_totalItems, int &_maxVisibleItems, int &_scrollOffset)
{

    if (Button.down.isPressed()) // If down button is pressed
    {
        _Setcursor += _Space;                 // Move cursor down by space amount
        if (_Setcursor > totalItems * _Space) // If cursor exceeds total items
            _Setcursor = _Space;              // Wrap around to the top

        int selectedIndex = _Setcursor / _Space; // 1-based

        // adjust scrollOffset
        if (selectedIndex > scrollOffset + maxVisibleItems) // If selected index exceeds visible items
        {
            scrollOffset = selectedIndex - maxVisibleItems; // Adjust scroll offset
        }
        if (selectedIndex <= scrollOffset) // If selected index is within scroll offset
        {
            scrollOffset = selectedIndex - 1; // Adjust scroll offset
        }
        Button.down.wait(); // Wait for button release
    }

    if (Button.up.isPressed()) // If up button is pressed
    {
        _Setcursor -= _Space;                 // Move cursor up by space amount
        if (_Setcursor < _Space)              // If cursor goes above the first item
            _Setcursor = totalItems * _Space; // Wrap around to the bottom

        int selectedIndex = _Setcursor / _Space; // 1-based

        // adjust scrollOffset
        if (selectedIndex > scrollOffset + maxVisibleItems) // If selected index exceeds visible items
        {
            scrollOffset = selectedIndex - maxVisibleItems; // Adjust scroll offset
        }
        if (selectedIndex <= scrollOffset) // If selected index is within scroll offset
        {
            scrollOffset = selectedIndex - 1; // Adjust scroll offset
        }
        Button.up.wait(); // Wait for button release
    }
}

/**********************
 * OUTER LINE FUNCTION
 * Draws the outer lines and sections on the LCD display
 **********************/
void LcdUIFunction::_OuterLine()
{
    LcdDisplay.Boder();    // Draw the border on the display
    _supervisoryOutLine(); // Draw the supervisory section lines
    _settingOutLine();     // Draw the setting section line
}

/**********************
 * SUPERVISORY OUTER LINE FUNCTION
 * Draws the supervisory section lines on the LCD display
 **********************/
void LcdUIFunction::_supervisoryOutLine()
{                                                         // supervisory
    LcdDisplay._DrawStraightLine(0, 36, 127, HORIZONTAL); // horizontal line
    LcdDisplay._DrawStraightLine(0, 49, 127, HORIZONTAL); // horizontal line

    LcdDisplay._DrawStraightLine(42, 36, 28, VERTICAL); // vertical line
    LcdDisplay._DrawStraightLine(84, 36, 28, VERTICAL); // vertical line
}

/**********************
 * SETTING OUTER LINE FUNCTION
 * Draws the setting section line on the LCD display
 **********************/
void LcdUIFunction::_settingOutLine()
{
    LcdDisplay._DrawStraightLine(0, 14, 128, HORIZONTAL); // horizontal line
}

/**********************
 * DISPLAY RESERVOIR FUNCTION
 * Manages the display of the reservoir level on the LCD
 **********************/
void LcdUIFunction::_displayReservoir()
{

    if (Alert.EnableDisable.RefillAlert)
    {
        LcdDisplay._DrawStraightLine(84, 14, 52, VERTICAL);
        LcdDisplay.print(87, 23, PRINT_RESERVOIR_LEVEL); // Display "RES LVL" text

        if (GreaseOut.Reservoir.Percentage == 100) // If reservoir level is 100%
        {
            LcdDisplay.print(95, 33, PRINT_EMPTY, int(GreaseOut.Reservoir.Percentage)); // Display reservoir percentage
            display.print(PRINT_PERCENTAGE_SYSMBOL);                                    // Display percentage symbol
        }
        else
        {
            LcdDisplay.print(100, 33, PRINT_EMPTY, int(GreaseOut.Reservoir.Percentage)); // Display reservoir percentage
            display.print(PRINT_PERCENTAGE_SYSMBOL);                                     // Display percentage symbol
        }
    }
    else
    {
        LcdDisplay._DrawStraightLine(84, 14, 52, VERTICAL);
        LcdDisplay.print(93, 27, "DIS"); // Display "RES LVL" text
    }
}

/**********************
 * DISPLAY MODE FUNCTION
 * Manages the display of the current mode on the LCD
 **********************/
void LcdUIFunction::_Mode()
{
    LcdDisplay.print(20, 23, PRINT_MODE); // Display "MODE:" text

    if (GreaseOut.Lubrication.Enable) // If lubrication mode is enabled
    {
        LcdDisplay.print(20, 23, PRINT_MODE_LUB);                                // Display "MODE: LUB" text
        LcdDisplay.print(30, 33, PRINT_EMPTY, GreaseOut.Lubrication.Live.Grams); // Display live lubrication grams
        display.print(PRINT_GRAM);                                               // Display grams symbol
    }
    else if (GreaseOut.PauseTime.Enable) // If pause time mode is enabled
    {
        LcdDisplay.print(20, 23, PRINT_MODE_PAUSE_TIME);                                                           // Display "MODE: PTM" text
        LcdDisplay.printTime(30, 33, PRINT_EMPTY, GreaseOut.PauseTime.Live.Hour, GreaseOut.PauseTime.Live.Minute); // Display live pause time
    }
    else if (GreaseOut.Reservoir.Enable)
    {
        LcdDisplay.print(20, 23, PRINT_MODE_REFILL);
    }
    else
    {
        display.setCursor(20, 23);       // Set cursor position
        display.print(PRINT_MODE_EMPTY); // Display "MODE: - " text
    }
}

/**********************
 * DISPLAY COMPANY NAME FUNCTION
 * Manages the display of the company name on the LCD
 **********************/
void LcdUIFunction::_displayCompanyName()
{
    LcdDisplay.print(46, 10, PRINT_COMPANY_NAME);                    // Display company name
    LcdDisplay.print(100, 10, PRINT_EMPTY, Lora_disconnected_count); // Display LoRa disconnected count
}

/**********************
 * DISPLAY SUPERVISORY TIME FUNCTION
 * Manages the display of supervisory time statuses on the LCD
 **********************/
void LcdUIFunction::_SupervisoryTime()
{

    _SupervisoryTime_1(); /* Display supervisory time for outlet 1 */
    _SupervisoryTime_2(); /* Display supervisory time for outlet 2 */
    _SupervisoryTime_3(); /* Display supervisory time for outlet 3 */
}

/**********************
 * DISPLAY SUPERVISORY TIME 1 FUNCTION
 * Manages the display of supervisory time status for outlet 1 on the LCD
 **********************/
void LcdUIFunction::_SupervisoryTime_1()
{
    LcdDisplay.print(9, 46, PRINT_OUTLET_1); // Display "OUT 1" text

    if (SupervisoryTime.ELH_OL1 == PUMP_GOOD && Alert.EnableDisable.Supervisory1) // If outlet 1 status is good and supervisory is enabled
    {
        LcdDisplay.print(11, 61, PRINT_GOOD); // Display PRINT_GOOD status
    }
    else if (SupervisoryTime.ELH_OL1 == PUMP_AVERAGE && Alert.EnableDisable.Supervisory1) // If outlet 1 status is average and supervisory is enabled
    {
        LcdDisplay.print(14, 61, PRINT_AVERAGE); // Display PRINT_AVERAGE status
    }
    else if (SupervisoryTime.ELH_OL1 == PUMP_BAD && Alert.EnableDisable.Supervisory1) // If outlet 1 status is bad and supervisory is enabled
    {
        LcdDisplay.print(14, 61, PRINT_BAD); // Display "BAD" status
    }
    else if (Alert.EnableDisable.Supervisory1 && Config.NoOfOutlet > 0 && !SupervisoryTime.AverageEnable_Supervisory1 && start_init == 1) // If supervisory grams exceed average and outlet 1 is enabled
    {
        LcdDisplay.print(14, 61, PRINT_CALCULATE); // Display PRINT_CALCULATE status
    }
    else if (!Alert.EnableDisable.Supervisory1 && Config.NoOfOutlet > 0)
    {
        LcdDisplay.print(14, 61, PRINT_DISABLE); // Display PRINT_DISABLE status
    }
    else
    {
        LcdDisplay.print(19, 61, PRINT_HYPHEN); // Display PRINT_HYPHEN status
    }
}

/**********************
 * DISPLAY SUPERVISORY TIME 2 FUNCTION
 * Manages the display of supervisory time status for outlet 2 on the LCD
 **********************/
void LcdUIFunction::_SupervisoryTime_2()
{
    LcdDisplay.print(49, 46, PRINT_OUTLET_2); // Display "OUT 2" text

    if (SupervisoryTime.ELH_OL2 == PUMP_GOOD && Alert.EnableDisable.Supervisory2) // If outlet 2 status is good
    {
        LcdDisplay.print(53, 61, PRINT_GOOD); // Display PRINT_GOOD status
    }
    else if (SupervisoryTime.ELH_OL2 == PUMP_AVERAGE && Alert.EnableDisable.Supervisory2) // If outlet 2 status is average
    {
        LcdDisplay.print(56, 61, PRINT_AVERAGE); // Display PRINT_AVERAGE status
    }
    else if (SupervisoryTime.ELH_OL2 == PUMP_BAD && Alert.EnableDisable.Supervisory2) // If outlet 2 status is bad
    {
        LcdDisplay.print(56, 61, PRINT_BAD); // Display "BAD" status
    }
    else if (Alert.EnableDisable.Supervisory2 && Config.NoOfOutlet > 1 && !SupervisoryTime.AverageEnable_Supervisory2 && start_init == 1) // If supervisory grams exceed average and outlet 2 is enabled
    {
        LcdDisplay.print(56, 61, PRINT_CALCULATE); // Display PRINT_CALCULATE status
    }
    else if (!Alert.EnableDisable.Supervisory2 && Config.NoOfOutlet > 1) // If outlet 2 status is disabled
    {
        LcdDisplay.print(56, 61, PRINT_DISABLE); // Display "DISABLE" status
    }
    else
    {
        LcdDisplay.print(61, 61, PRINT_HYPHEN); // Display PRINT_HYPHEN status
    }
}

/**********************
 * DISPLAY SUPERVISORY TIME 3 FUNCTION
 * Manages the display of supervisory time status for outlet 3 on the LCD
 **********************/
void LcdUIFunction::_SupervisoryTime_3()
{
    LcdDisplay.print(91, 46, PRINT_OUTLET_3); // Display "OUT 3" text

    if (SupervisoryTime.ELH_OL3 == PUMP_GOOD && Alert.EnableDisable.Supervisory3) // If outlet 3 status is good
    {
        LcdDisplay.print(95, 61, PRINT_GOOD); // Display PRINT_GOOD status
    }
    else if (SupervisoryTime.ELH_OL3 == PUMP_AVERAGE && Alert.EnableDisable.Supervisory3) // If outlet 3 status is average
    {
        LcdDisplay.print(98, 61, PRINT_AVERAGE); // Display PRINT_AVERAGE status
    }
    else if (SupervisoryTime.ELH_OL3 == PUMP_BAD && Alert.EnableDisable.Supervisory3) // If outlet 3 status is bad
    {
        LcdDisplay.print(98, 61, PRINT_BAD); // Display "BAD" status
    }
    else if (Alert.EnableDisable.Supervisory3 && Config.NoOfOutlet > 2 && !SupervisoryTime.AverageEnable_Supervisory3 && start_init == 1) // If supervisory grams exceed average and outlet 3 is enabled
    {
        LcdDisplay.print(98, 61, PRINT_CALCULATE); // Display PRINT_CALCULATE status
    }
    else if (!Alert.EnableDisable.Supervisory3 && Config.NoOfOutlet > 2) // If outlet 3 status is disabled
    {
        LcdDisplay.print(103, 61, PRINT_DISABLE); // Display "DISABLE" status
    }
    else
    {
        LcdDisplay.print(103, 61, PRINT_HYPHEN); // Display PRINT_HYPHEN status
    }
}

/**********************
 * BLINK CHECK FUNCTION
 * Determines whether to blink a value based on elapsed time
 **********************/
bool LcdUIFunction::_shouldBlink(unsigned long &lastBlinkTime, bool &blinkState)
{
    unsigned long currentTime = millis(); // Get the current time in milliseconds
    if (blinkState)                       // If currently in blink ON state
    {
        if (currentTime - lastBlinkTime >= 700) // ON for 700ms
        {
            blinkState = false;          // Set blink state to OFF
            lastBlinkTime = currentTime; // Update last blink time
        }
    }
    else
    {
        if (currentTime - lastBlinkTime >= 300) // OFF for 300ms
        {
            blinkState = true;           // Set blink state to ON
            lastBlinkTime = currentTime; // Update last blink time
        }
    }

    return blinkState;
}

/**********************
 * BLINK DISPLAY FUNCTION
 * Manages the blinking of values on the LCD
 **********************/
void LcdUIFunction::Blink(char _x, char _y, const char *_value)
{
    if (_shouldBlink(ArrowlastBlinkTime, ArrowblinkState)) // If it's time to blink
    {
        LcdDisplay.print(_x, _y, _value); // Display the value
    }
}

/**********************
 * BLINK DISPLAY FUNCTION
 * Manages the blinking of values on the LCD
 **********************/
template <typename T>
void LcdUIFunction::Blink(char _x, char _y, T _value)
{
    if (_shouldBlink(ArrowlastBlinkTime, ArrowblinkState)) // If it's time to blink
    {
        display.setCursor(_x, _y); // Set cursor position
        display.print(_value);     // Display the value
    }
}

/**********************
 * SCROLL VIEW RESET FUNCTION
 * Resets the scroll view parameters to their default values
 **********************/
void LcdUIFunction::scrollViewReset()
{
    scrollOffset = 0;    // Reset scroll offset to 0
    totalItems = 17;     // Set total items to 17
    itemHeight = 12;     // Set item height to 12
    maxVisibleItems = 5; // Set maximum visible items to 5
    Space = 12;          // Set space between items to 12
}

/**********************
 * DISPLAY TO KEY LOCK ENABLE FUNCTION
 * Manages the display prompt to enable key lock on the LCD
 **********************/
void LcdUIFunction::_displayToKeyLockEnable()
{
    if (Keylock && (Button.menu.isPressed() || Button.back.isPressed() || Button.up.isPressed() || Button.down.isPressed() || Button.ok.isPressed()))
    {
        display.clearDisplay();                                 // Clear the display buffer
        LcdDisplay.Boder();                                     // Draw the border on the display
        LcdDisplay.print(25, 36, PRINT_SET_KEY_LOCK_DISABLE_1); // Display prompt text
        LcdDisplay.print(25, 48, PRINT_SET_KEY_LOCK_DISABLE_2); // Display prompt text
        display.display();                                      // Update the display with the buffer content
        vTaskDelay(pdMS_TO_TICKS(3000));                        // Yield to other tasks with a delay of 3000 milliseconds

        Button.back.wait(); // Wait for button release
        Button.ok.wait();   // Wait for button release
        Button.menu.wait(); // Wait for button release
        Button.up.wait();   // Wait for button release
        Button.down.wait(); // Wait for button release
    }
}

/**********************
 * KEY LOCK ENABLE FUNCTION
 * Manages the display prompt when key lock is enabled on the LCD
 **********************/
void LcdUIFunction::_keyLockEnable()
{
    if (KeylockEnable) // If key lock is enabled
    {
        display.clearDisplay();                          // Clear the display buffer
        LcdDisplay.Boder();                              // Draw the border on the display
        LcdDisplay.print(25, 36, PRINT_KEY_LOCK_ENABLE); // Display "KEY LOCK ENABLE" text
        display.display();                               // Update the display with the buffer content
        vTaskDelay(pdMS_TO_TICKS(3000));                 // Wait for 3 seconds
        KeylockEnable = false;                           // Reset key lock enable flag
    }
}

/**********************
 * KEY LOCK DISABLE FUNCTION
 * Manages the display prompt when key lock is disabled on the LCD
 **********************/
void LcdUIFunction::_keyLockDisable()
{
    if (KeylockDisable) // If key lock is disabled
    {
        display.clearDisplay();                           // Clear the display buffer
        LcdDisplay.Boder();                               // Draw the border on the display
        LcdDisplay.print(25, 36, PRINT_KEY_LOCK_DISABLE); // Display "KEY LOCK DISABLE" text
        display.display();                                // Update the display with the buffer content
        vTaskDelay(pdMS_TO_TICKS(3000));                  // Wait for 3 seconds
        KeylockDisable = false;                           // Reset key lock disable flag
    }
}
