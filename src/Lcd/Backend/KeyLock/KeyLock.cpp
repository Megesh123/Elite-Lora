#include "HeaderFile.h"       // Includes all necessary declarations, constants, and variable definitions used by this file

// -----------------------------------------------------------------------------
// Function: Disable
// Purpose : Disables key lock if more than 2 button presses detected
// -----------------------------------------------------------------------------
void KeyLockFunction::Disable(int _Button_count)   // Definition of the Disable function with parameter _Button_count
{
    if (_Button_count > 2 && Keylock)              // Checks if button count is greater than 2 and key lock is currently enabled
    {
        Keylock = false;                           // Turns off (disables) the key lock status
        KeylockDisable = true;                     // Sets a flag indicating that key lock has been disabled
        Eeprom.write();                            // Writes updated key lock status to EEPROM (saves state permanently)
    }
}

// -----------------------------------------------------------------------------
// Function: Enable
// Purpose : Enables key lock if more than 2 button presses detected
// -----------------------------------------------------------------------------
void KeyLockFunction::Enable(int _Button_count)    // Definition of the Enable function with parameter _Button_count
{
    if (_Button_count > 2 && !Keylock)             // Checks if button count is greater than 2 and key lock is currently disabled
    {
        Keylock = true;                            // Turns on (enables) the key lock status
        KeylockEnable = true;                      // Sets a flag indicating that key lock has been enabled
        Eeprom.write();                            // Writes updated key lock status to EEPROM (saves state permanently)
    }
}
