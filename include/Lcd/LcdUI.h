/**********************
 * LCD UI.H
 * Header file for LCD user interface
 **********************/
#ifndef __LCD_UI_H__
#define __LCD_UI_H__

/**********************
 * BluetoothDisplay CLASS
 * Manages Bluetooth display functions
 **********************/
class BluetoothDisplay
{
public:
    void Logo();       // Display Bluetooth logo
    void Function();   // Main function to handle Bluetooth display operations
    void Enable();     // Enable Bluetooth display
    void Enabled();    // Bluetooth display enabled
    void Connecting(); // Display connecting status
    void Connected();  // Display connected status
    void Receive();    // Display receiving status
    void Received();   // Display received status
};

/**********************
 * LcdUIFunction CLASS
 * Manages LCD user interface functions
 **********************/
class LcdUIFunction
{
public:
    BluetoothDisplay Bluetooth;   // Bluetooth display management
    void Logo();                  // Display main logo
    void SleepMode();             // Display sleep mode
    void MainPage();              // Display main page
    void CheckAllValueSetOrNot(); // Check if all values are set
    void AlertPage();             // Display alert page
    void MenuSettingPage();
    void _MenuPage();          // Display menu page
    void _ConfigPage();        // Display configuration page
    void _SettingPage();       // Display setting page
    void _RefillPage();        // Display refill page
    void _TestRunPage();       // Display test run page
    void _ResetPage();         // Display reset page
    void Testrun();            // Display test run
    void OTAPage();            // Display OTA update page
    void ReconfigRefillPage(); // Display reconfiguration refill page

    void ScroolBarCalculation(char &_Setcursor, int &_Space, int &_totalItems, int &_maxVisibleItems, int &_scrollOffset); // Calculate scroll bar parameters
    void _drawScrollBar(int &_totalItems, int &_maxVisibleItems, int &_scrollOffset);                                      // Draw scroll bar on LCD

    void scrollViewReset(); // Reset scroll view parameters

    template <typename T>
    void Blink(char _x, char _y, T _value);                            // Blink value of type T
    void Blink(char _x, char _y, const char *_value);                  // Blink value without specific type
    int _getSupMin(int outlet);                                        // Get supervisory minute for outlet
    int _getSupSec(int outlet);                                        // Get supervisory second for outlet
    void _OuterLine();                                                 // Draw outer line on LCD
    void _supervisoryOutLine();                                        // Draw supervisory outline
    void _settingOutLine();                                            // Draw setting outline
    void _Mode();                                                      // Display mode
    bool _shouldBlink(unsigned long &lastBlinkTime, bool &blinkState); // Determine if blinking should occur

    void _displayReservoir();   // Display reservoir information
    void _displayCompanyName(); // Display company name
    void _SupervisoryTime();    // Display supervisory time

    void _testRunErrorOccur();      // Display test run error occurrence
    void _keyLockEnable();          // Enable key lock
    void _keyLockDisable();         // Disable key lock
    void _displayToKeyLockEnable(); // Display key lock enable message
    void _mainPage();               // Display main page

    void _configPage_Setcursor();    // Set cursor for setting page
    void _configPage_TankCapacity(); // Display tank capacity
    void _configPage_Outlet();       // Display outlet information

    void _settingPage_Setcursor();                  // Set cursor for setting page
    void _settingPage_LubDischarge();               // Display lubrication discharge setting
    void _settingPage_PauseTime_Hour();             // Display pause time hour setting
    void _settingPage_PauseTime_Min();              // Display pause time minute setting
    void _settingPage_SupervisoryTime(int _select); // Display supervisory time setting

    void _refillPage_Setcursor();       // Set cursor for refill page
    void _refillPage_FrontPage();       // Display refill front page
    void _refillPage_refillFrontPage(); // Display refill front page
    void _refillPage_RefillStart();     // Start refill process
    void _refillPage_RefillStop();      // Stop refill process
    void _refillPage_RefillCompleted(); // Display refill completed message

    void _testRunPage_Setcursor(); // Set cursor for test run page
    void _testRunPage_FrontPage(); // Display test run front page

    void _resetPage_Setcursor();      // Set cursor for reset page
    void _resetPage_FrontPage();      // Display reset front page
    void _resetPage_ResetCompleted(); // Display reset completed message

    void _waring_TankCapacity();      // Display tank capacity warning
    void _waring_ReservoirLowLevel(); // Display reservoir low level warning
    void _waring_Outlet();            // Display outlet warning

    void _aletPage_Setcursor();          // Set cursor for alert page
    void _aletPage_Overload();           // Display overload alert
    void _alertPage_ReservoirLowLevel(); // Display reservoir low level alert
    void _alertPage_Supervisor_1();      // Display supervisor 1 alert
    void _alertPage_Supervisor_2();      // Display supervisor 2 alert
    void _alertPage_Supervisor_3();      // Display supervisor 3 alert
    void _alertPage_SignalNotDetected(); // Display signal not detected alert
    void _alertPage_Pump_Element_Health_1();
    void _alertPage_Pump_Element_Health_2();
    void _alertPage_Pump_Element_Health_3();
    void _alertPage_Unknow();         // Display unknown alert
    void _alertPage_ResetCompleted(); // Display alert reset completed message

    void _SupervisoryTime_1(); // Display supervisory time 1
    void _SupervisoryTime_2(); // Display supervisory time 2
    void _SupervisoryTime_3(); // Display supervisory time 3

    unsigned long lastSignalCheck = 0;
    const long Sleep_interval = 300; // Interval for animation frame changes in milliseconds
    long Sleep_previousMillis = 0;   // Store the last time the animation frame was updated
    int Sleep_count = 0;             // Counter for animation frames
};
extern LcdUIFunction LcdUI; // Global instance of LcdUIFunction

#endif