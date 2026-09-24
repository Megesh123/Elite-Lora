#ifndef __ABOUT_DEVICE_H__ // Prevents multiple inclusion of this header file
#define __ABOUT_DEVICE_H__ // Marks the start of the header definition

// Class to handle total runtime and system statistics
class TotalStats
{
private:
public:
    void runTime();     // Tracks and updates total run time in seconds, minutes, hours
    void greaseOut();   // Tracks total lubricant discharge
    void pauseMinute(); // Tracks paused duration in minutes
    void pauseHour();   // Tracks paused duration in hours
    void alert();       // Counts and limits total alert occurrences
};

// Class for managing "About Device" screen and cursor/page control
class ABOUTDEVICEFUNCTION
{
public:
    TotalStats total;         // Object of TotalStats for runtime and alert tracking
    void SetCursor();         // Handles LCD cursor movement for About Device page
    void pageEnableDisable(); // Toggles About Device page visibility and admin mode
    void Page();
};
extern ABOUTDEVICEFUNCTION AboutDevice; // External declaration so other files can access the AboutDevice object

#endif // End of header guard
