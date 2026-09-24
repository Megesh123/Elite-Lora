/**********************
 * SERIAL MONITOR INTERFACE HEADER
 * Defines interface for serial communication and system monitoring
 * Handles user input, password protection, and real-time data display
 **********************/

#ifndef __SERIAL_MONITOR_H__
#define __SERIAL_MONITOR_H__

/**********************
 * SERIALMONITOR CLASS
 * Manages serial communication and user interface functions
 **********************/
class SERIALMONITOR
{
public:
    void printdata();                // Print comprehensive system data to serial
    void printMainMenu();            // Display main menu options and commands
    void printLiveDataMenu();        // Display live data monitoring submenu
    void printFormattedDateTime();   // Display formatted current date and time
    void livedata_process();         // Process live data monitoring with user interaction
    void passwordCheck();            // Verify user password input
    void passwordValid();            // Validate successful password authentication
    void passwordinvalid();          // Handle invalid password attempts
    void passwordCorrect();          // Process commands after successful authentication
    void worngPassword();            // Handle wrong password attempts with security delays
    void SerialMonitorAlertEnable(); // Enable and configure system alerts via serial commands
    void available();                // Check for available serial data and process input
};

extern SERIALMONITOR serial; // Global instance of SERIALMONITOR

#endif /* __SERIAL_MONITOR_H__ */