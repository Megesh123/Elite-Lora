/**********************
 * LCD DISPLAY.H
 * Header file for LCD display management
 **********************/
#ifndef __LCD_DISPLAY_H__
#define __LCD_DISPLAY_H__

/**********************
 * LcdDisplayFunction CLASS
 * Manages LCD display functions and rendering
 **********************/
class LcdDisplayFunction
{
public:
    void Begin();     // Initialize LCD display
    void printOk();   // Print OK message
    void printBack(); // Print Back message
    void Boder();     // Draw border on LCD

    void _DrawStraightLine(char column, char row, int total, char TypeOfLine); // Draw straight line on LCD

    template <typename T>
    void print(char _x, char _y, const char *_valueName, T _value); // Print value of type T
    void print(char _x, char _y, const char *_valueName);           // Print value without specific type
    template <typename T>
    void printTime(char _x, char _y, const char *_valueName, T _value1, T _value2); // Print time values of type T
    template <typename T>
    void printTime(char _x, char _y, const char *_valueName, long _value1, T _value2); // Print time values with long and type T
    void print0();                                                                     // Print zero value
};
extern LcdDisplayFunction LcdDisplay; // Global instance of LcdDisplayFunction

class SignalBarFunction
{
public:
    uint8_t getSignalLevel(int signalQuality);
    void drawSignalBars(uint8_t level);
    void clearSignalArea();
};

extern SignalBarFunction SignalBar;

#endif