#include "HeaderFile.h" // Includes project-wide header file (likely contains global variables, class references, etc.)

ABOUTDEVICEFUNCTION AboutDevice; // Defines the global instance declared in the header file

void TotalStats::greaseOut() // Function to calculate total lubricant discharge
{
    GreaseOut.Lubrication.Total.Grams += Config.NoOfOutlet; // Add to total dispensed
}

void TotalStats::pauseMinute() // Function to increment total pause time in minutes
{
    GreaseOut.PauseTime.Total.Minute++; // Increment total pause minutes
}

void TotalStats::pauseHour() // Function to handle pause hours calculation
{
    if (GreaseOut.PauseTime.Total.Minute > 60) // Check if minutes overflowed
    {
        GreaseOut.PauseTime.Total.Minute = 0; // Reset minutes
        GreaseOut.PauseTime.Total.Hour++;     // Increment hours
    }
}

void TotalStats::alert() // Function to increment total alerts
{
    if (Alert.Total.enable)
    {
        Alert.Total.NoOfAlert++;    // Increment total alert counter
        Alert.Total.enable = false; // Set flag to prevent multiple increments per alert

        if (Alert.NameCount == 1)
        {
            Alert.Total.OverLoad++;
        }
        else if (Alert.NameCount == 2)
        {
            Alert.Total.Reservoir++;
        }
        else if (Alert.NameCount == 3)
        {
            Alert.Total.Supervisory1++;
        }
        else if (Alert.NameCount == 4)
        {
            Alert.Total.Supervisory2++;
        }
        else if (Alert.NameCount == 5)
        {
            Alert.Total.Supervisory3++;
        }
        else if (Alert.NameCount == 6)
        {
            Alert.Total.SignalNotDetected++;
        }
        else if (Alert.NameCount == 7)
        {
            Alert.Total.PumpWeak1++;
        }
        else if (Alert.NameCount == 8)
        {
            Alert.Total.PumpWeak2++;
        }
        else if (Alert.NameCount == 9)
        {
            Alert.Total.PumpWeak3++;
        }
    }
}

//--------------------------------------------------------------
// Implementation of ABOUTDEVICEFUNCTION class member functions
//--------------------------------------------------------------

void ABOUTDEVICEFUNCTION::SetCursor() // Function to handle LCD cursor setting
{
    // Calls UI scroll bar calculation to manage cursor position within the About Device screen
    LcdUI.ScroolBarCalculation(AboutDeviceSetcursor, Space, totalItems, maxVisibleItems, scrollOffset);
}

void ABOUTDEVICEFUNCTION::pageEnableDisable() // Function to enable or disable About Device page
{
    // Checks if both OK and MENU buttons are pressed simultaneously and keylock is inactive
    if (!MenuEnable && Button.ok.isPressed() && Button.menu.isPressed() && Button.back.isNotPressed() && Button.up.isNotPressed() && Button.down.isNotPressed() && !Keylock)
    {
        int _count = 0;
        // Waits until both buttons are released before toggling page
        uint32_t pressStart = millis();

        while (Button.ok.isPressed() || Button.menu.isPressed() && !bluetooth.ModeEnable && !GreaseOut.Sleep.Enable)
        {
            if (millis() - pressStart >= 2000) // 1000 ms = 1 second
            {
                bluetooth.ModeEnable = true; // Enable Bluetooth mode
                Eeprom.write();
                vTaskDelay(pdMS_TO_TICKS(10)); // Prevent watchdog and allow other tasks to run
                ESP.restart();
            }

            vTaskDelay(pdMS_TO_TICKS(10)); // Prevent watchdog and allow other tasks to run
        }

        AboutDevices.Enable = !AboutDevices.Enable; // Toggles About Devices page state (ON/OFF)

        if (AboutDevices.Enable) // If About Devices page is active
        {
            AdminSetting.Enable = true; // Enable admin section for device details
        }
        else // If About Devices page is inactive
        {
            AdminSetting.Enable = false; // Disable admin section
        }

        AboutDeviceSetcursor = 12; // Reset cursor position to starting line (12th row)
        LcdUI.scrollViewReset();   // Reset LCD scroll view to initial state
    }
}
