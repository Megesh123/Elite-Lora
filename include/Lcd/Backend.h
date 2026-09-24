/**********************
 * BACKEND.H
 * Header file for LCD backend settings and menu management
 **********************/
#ifndef __SETTING_H__
#define __SETTING_H__

/**********************
 * Class Definitions
 **********************/
class CONFIGSETCURSOR
{
public:
    void Select(); // Select configuration cursor
};

/**********************
 * Class Definitions
 **********************/
class SETTINGSETCURSOR
{
public:
    void Select(); // Select setting cursor
};

/**********************
 * Class Definitions
 **********************/
class MENUPAGE
{
public:
    void Page();      // Display menu page
    void Reset();     // Reset menu page
    void Setcursor(); // Set cursor position
    void _serial_monitor_to_check(String data);
    void _serial_monitor_to_checkFrontend(String data);
};

/**********************
 * Class Definitions
 **********************/
class CONFIGPAGE
{
public:
    CONFIGSETCURSOR SetCursor; // Configuration cursor
    void Page();               // Display configuration page
    void Reset();              // Reset configuration page
    void Setcursor();          // Set cursor position
};

/**********************
 * Class Definitions
 **********************/
class SETTINGPAGE
{
public:
    SETTINGSETCURSOR SetCursor; // Setting cursor
    void Page();                // Display setting page
    void Reset();               // Reset setting page
    void Setcursor();           // Set cursor position
};

/**********************
 * Class Definitions
 **********************/
class REFILLPAGE
{
public:
    void Page();      // Display refill page
    void Setcursor(); // Set cursor position
};

/**********************
 * Class Definitions
 **********************/
class TESTRUNPAGE
{
public:
    void Page(); // Display test run page
};

/**********************
 * Class Definitions
 **********************/
class RESRTPAGE
{
public:
    void Page(); // Display reset page
};

/**********************
 * Function Prototypes
 **********************/
void BlinkFunction(bool &value); // Function to handle blinking effect

/**********************
 * Template Function Prototypes
 **********************/
template <typename T>
void IncrementDecrement(T &value, T min, T max, T IncDec); // Function to increment/decrement variable with speed control
template <typename T>
void VariableCountIncDec(T &value, T min, T max, T IncDec, bool IncDecSpeed, bool &Blink); // Function to handle variable increment/decrement with blinking effect

/**********************
 * Class Definitions
 **********************/
class MENUFUNCTION
{
public:
    MENUPAGE MEnu;       // Menu page
    CONFIGPAGE Config;   // Configuration page
    SETTINGPAGE Setting; // Setting page
    REFILLPAGE Refill;   // Refill page
    TESTRUNPAGE TestRun; // Test run page
    RESRTPAGE Reset;     // Reset page

    void Function();        // Main function to handle menu operations
    void pageEnable();      // Enable menu page
    void SettingFunction(); // Function to handle settings
private:
    void BackFunction(); // Function to handle back operation
};
extern MENUFUNCTION MenuSetting; // Global instance of MENUFUNCTION

/**********************
 * Class Definitions
 **********************/
class MAINFUNCTION
{
public:
    void Function();   // Main function
    void pageEnable(); // Enable main page
};

extern MAINFUNCTION MainPage; // Global instance of MAINFUNCTION

/**********************
 * Function Prototypes
 **********************/
void VariableUpdate(); // Function to update variables

// Define a class named KeyLockFunction that handles enabling and disabling a key lock feature
class KeyLockFunction
{
public:                              // Public section — functions accessible from outside the class
    void Enable(int _Button_count);  // Function to enable key lock when certain button conditions are met
    void Disable(int _Button_count); // Function to disable key lock when certain button conditions are met
};

extern KeyLockFunction KeyLock;

class SLEEPMODE
{
public:
    void Enable();
    void Disable();
    void start();
    void stop();
};
extern SLEEPMODE SLeepMode;

class LcdSleepModeFunction
{
public:
    void Page();
};
extern LcdSleepModeFunction Sleepmode;

class LcdKeyLockFunction
{
public:
    void Enable();
    void Disable();
    void Error();
};
extern LcdKeyLockFunction LcdKeyLock;
#endif