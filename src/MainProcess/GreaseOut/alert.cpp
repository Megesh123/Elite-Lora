/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note Alert system implementation for Lubrication Control System
 */

/**********************
 * ALERT SYSTEM IMPLEMENTATION
 * Handles detection, processing, and notification of system alerts
 * Includes overload protection, pressure monitoring, and fault detection
 **********************/

#include "Headerfile.h"
#include <cstring>

void SetAlertName(const char *s)
{
    STATE_LOCK();
    strlcpy(Alert.Name, s ? s : "", sizeof(Alert.Name));
    STATE_UNLOCK();
}

/**********************
 * ALERT OCCURRENCE HANDLER
 * Main alert processing function that triggers appropriate responses
 **********************/
void ALERTFUNCTION::occured()
{
    if (Alert.Enable && start_init == 1) // Check if any alert is enabled
    {
        _AlertIdentifier(); // Identify alert type for LoRa communication
        _FunctionStop();    // Stop all system functions for safety
        AboutDevice.total.alert();
        vTaskDelay(pdMS_TO_TICKS(1)); // Yield to other tasks with a delay of 1 milliseconds
    }
}

/**********************
 * ALERT RESET PROCESS
 * Resets alert system to normal operation state
 **********************/
void ALERTFUNCTION::ResetProcess()
{
    Alert.Enable = false;               // Disable the global alert occurrence flag
    SetAlertName(AlertName.No.c_str()); // Reset alert name to indicate no alert
    Alert.Lora.DataSend = false;        // Reset LoRa data send enable flag
    Alert.NameCount = 0;                // Reset alert name occurrence counter
}

/**********************
 * ALERT CLEAR FUNCTION
 * Clears active alerts and restores normal system operation
 **********************/
void ALERTFUNCTION::clear()
{
    Grease.ResetProcess();
    AlertFun.ResetProcess();
    flash.OnceWrite = true; // Enable one-time flash writing operation
    Led.off();              // Turn off all alert indicator LEDs
}

/**********************
 * SYSTEM SAFETY FUNCTION
 * Stops all active system functions in response to an alert
 **********************/
void ALERTFUNCTION::_FunctionStop()
{
    // occured() checks Alert.Enable, then sleeps ~100ms in _AlertIdentifier()
    // before reaching here. In that gap a reset (VariableUpdate) can clear the
    // alert and re-enable lubrication on another task. Re-check so we don't
    // stomp the freshly re-enabled process back off ("MODE: -" bug).
    if (!Alert.Enable)
        return;
    GreaseOut.Lubrication.Enable = false; // Stop lubrication process immediately
    GreaseOut.PauseTime.Enable = false;   // Disable pause time mode
    GreaseOut.TestRun.Enable = false;     // Stop any test run in progress
    Motor.off();                          // Turn off motor for safety
}

/**********************
 * ALERT TRIGGER FUNCTIONS
 * Activate specific alert conditions based on system status
 **********************/
void ALERTFUNCTION::Supervisory1()
{
    Alert.Enable = true;                           // Enable global alert occurrence flag
    SetAlertName(AlertName.Supervisory_1.c_str()); // Set alert name to overload type
    TotalAlert.Total_Supervisory1++;
}

/**********************
 * ALERT TRIGGER FUNCTIONS
 * Activate specific alert conditions based on system status
 **********************/
void ALERTFUNCTION::Supervisory2()
{
    Alert.Enable = true;                           // Enable global alert occurrence flag
    SetAlertName(AlertName.Supervisory_2.c_str()); // Set alert name to overload type
    TotalAlert.Total_Supervisory2++;
}

/**********************
 * ALERT TRIGGER FUNCTIONS
 * Activate specific alert conditions based on system status
 **********************/
void ALERTFUNCTION::Supervisory3()
{
    Alert.Enable = true;                           // Enable global alert occurrence flag
    SetAlertName(AlertName.Supervisory_3.c_str()); // Set alert name to overload type
    TotalAlert.Total_Supervisory3++;
}

/**********************
 * ALERT TRIGGER FUNCTIONS
 * Activate specific alert conditions based on system status
 **********************/
void ALERTFUNCTION::overLoad()
{
    Alert.Enable = true;                           // Enable global alert occurrence flag
    SetAlertName(AlertName.Overload.c_str());      // Set alert name to overload type
    Alert.OverloadValue = CurrentMonitor.measured; // Store current value that caused overload
}

/**********************
 * ALERT TRIGGER FUNCTIONS
 * Activate specific alert conditions based on system status
 **********************/
void ALERTFUNCTION::reservoir()
{
    Alert.Enable = true;                                   // Enable global alert occurrence flag
    SetAlertName(AlertName.ReservoirLowLevel.c_str());     // Set alert name to reservoir low level type
    Alert.ReservoirValue = GreaseOut.Reservoir.Percentage; // Store current reservoir fill percentage
}

/**********************
 * ALERT TRIGGER FUNCTIONS
 * Activate specific alert conditions based on system status
 **********************/
void ALERTFUNCTION::proximityNotSense()
{
    if (GreaseOut.Proximity.Seconds > PROXIMITY_TIMEOUT_SEC) // Check if proximity timeout exceeded
    {
        SetAlertName(AlertName.ProximityNotSense.c_str()); // Set alert name to proximity not sense type
        Alert.Enable = true;                               // Enable global alert occurrence flag
    }
}

/**********************
 * ALERT IDENTIFICATION FUNCTION
 * Identifies the type of alert for LoRa communication
 **********************/
void ALERTFUNCTION::_AlertIdentifier()
{
    _identifierOverload();          // Identify overload alerts
    _identifierReservoir();         // Identify reservoir alerts
    _identifierProximityNotSense(); // Identify proximity sensor alerts
    _identifierSupervisory1();
    _identifierSupervisory2();
    _identifierSupervisory3();
    _identifierPumpElement1_Weak();
    _identifierPumpElement2_Weak();
    _identifierPumpElement3_Weak();

    vTaskDelay(pdMS_TO_TICKS(100)); // Yield to other tasks with a delay of 100 milliseconds
}

/**********************
 * ALERT TYPE IDENTIFICATION METHODS
 * Specific handlers for different alert types
 **********************/
void ALERTFUNCTION::_identifierOverload()
{
    if (strstr(Alert.Name, AlertName.Overload.c_str())) // Check if alert is overload type
    {
        Led.overLoad();      // Activate overload LED indicator
        Alert.NameCount = 1; // Set alert count to overload type
    }
}

/**********************
 * ALERT TYPE IDENTIFICATION METHODS
 * Specific handlers for different alert types
 **********************/
void ALERTFUNCTION::_identifierReservoir()
{
    if (strstr(Alert.Name, AlertName.ReservoirLowLevel.c_str())) // Check if alert is reservoir type
    {
        Led.reservoir();     // Activate reservoir LED indicator
        Alert.NameCount = 2; // Set alert count to reservoir type
    }
}

/**********************
 * ALERT TYPE IDENTIFICATION METHODS
 * Specific handlers for different alert types
 **********************/
void ALERTFUNCTION::_identifierProximityNotSense()
{
    if (strstr(Alert.Name, AlertName.ProximityNotSense.c_str())) // Check if proximity alert
    {
        Led.proximityNotSense(); // Activate proximity LED indicator
        Alert.NameCount = 6;     // Set alert count to proximity type
    }
}

/**********************
 * ALERT TYPE IDENTIFICATION METHODS
 * Specific handlers for different alert types
 **********************/
void ALERTFUNCTION::_identifierSupervisory1()
{
    if (strstr(Alert.Name, AlertName.Supervisory_1.c_str())) // Check if proximity alert
    {
        Led.Supervisory();   // Activate proximity LED indicator
        Alert.NameCount = 3; // Set alert count to proximity type
    }
}

/**********************
 * ALERT TYPE IDENTIFICATION METHODS
 * Specific handlers for different alert types
 **********************/
void ALERTFUNCTION::_identifierSupervisory2()
{
    if (strstr(Alert.Name, AlertName.Supervisory_2.c_str())) // Check if proximity alert
    {
        Led.Supervisory();   // Activate proximity LED indicator
        Alert.NameCount = 4; // Set alert count to proximity type
    }
}

/**********************
 * ALERT TYPE IDENTIFICATION METHODS
 * Specific handlers for different alert types
 **********************/
void ALERTFUNCTION::_identifierSupervisory3()
{
    if (strstr(Alert.Name, AlertName.Supervisory_3.c_str())) // Check if proximity alert
    {
        Led.Supervisory();   // Activate proximity LED indicator
        Alert.NameCount = 5; // Set alert count to proximity type
    }
}

/**********************
 * ALERT TYPE IDENTIFICATION METHODS
 * Specific handlers for different alert types
 **********************/
void ALERTFUNCTION::_identifierPumpElement1_Weak()
{
    if (strstr(Alert.Name, AlertName.PumpWeak_1.c_str())) // Check if proximity alert
    {
        Led.Supervisory();   // Activate proximity LED indicator
        Alert.NameCount = 7; // Set alert count to proximity type
    }
}

/**********************
 * ALERT TYPE IDENTIFICATION METHODS
 * Specific handlers for different alert types
 **********************/
void ALERTFUNCTION::_identifierPumpElement2_Weak()
{
    if (strstr(Alert.Name, AlertName.PumpWeak_2.c_str())) // Check if proximity alert
    {
        Led.Supervisory();   // Activate proximity LED indicator
        Alert.NameCount = 8; // Set alert count to proximity type
    }
}

/**********************
 * ALERT TYPE IDENTIFICATION METHODS
 * Specific handlers for different alert types
 **********************/
void ALERTFUNCTION::_identifierPumpElement3_Weak()
{
    if (strstr(Alert.Name, AlertName.PumpWeak_3.c_str())) // Check if proximity alert
    {
        Led.Supervisory();   // Activate proximity LED indicator
        Alert.NameCount = 9; // Set alert count to proximity type
    }
}

/**********************
 * SYSTEM INITIALIZATION
 * Initialize alert system with previous state from EEPROM
 **********************/

void ALERTFUNCTION::begin()
{
    if (Alert.NameCount == _overload) // Check if previous alert was overload
    {
        Alert.Enable = true;                      // Enable alert occurrence
        SetAlertName(AlertName.Overload.c_str()); // Set alert name to overload
    }
    else if (Alert.NameCount == _reservoir) // Check if previous alert was reservoir
    {
        Alert.Enable = true;                               // Enable alert occurrence
        SetAlertName(AlertName.ReservoirLowLevel.c_str()); // Set alert name to reservoir
    }
    else if (Alert.NameCount == _supervisory1) // Check if previous alert was supervisory1
    {
        Alert.Enable = true;                           // Enable alert occurrence
        SetAlertName(AlertName.Supervisory_1.c_str()); // Set alert name to reservoir
    }
    else if (Alert.NameCount == _supervisory2) // Check if previous alert was supervisory2
    {
        Alert.Enable = true;                           // Enable alert occurrence
        SetAlertName(AlertName.Supervisory_2.c_str()); // Set alert name to reservoir
    }
    else if (Alert.NameCount == _supervisory3) // Check if previous alert was supervisory3
    {
        Alert.Enable = true;                           // Enable alert occurrence
        SetAlertName(AlertName.Supervisory_3.c_str()); // Set alert name to reservoir
    }
    else if (Alert.NameCount == _proximity) // Check if previous alert was proximity
    {
        SetAlertName(AlertName.ProximityNotSense.c_str()); // Set alert name to proximity
        Alert.Enable = true;                               // Enable alert occurrence
    }
    else if (Alert.NameCount == _pumpweak1) // Check if previous alert was pump weak 1
    {
        SetAlertName(AlertName.PumpWeak_1.c_str()); // Set alert name to pump weak 1
        Alert.Enable = true;                        // Enable alert occurrence
    }
    else if (Alert.NameCount == _pumpweak2) // Check if previous alert was pump weak 2
    {
        SetAlertName(AlertName.PumpWeak_2.c_str()); // Set alert name to pump weak 2
        Alert.Enable = true;                        // Enable alert occurrence
    }
    else if (Alert.NameCount == _pumpweak3) // Check if previous alert was pump weak 3
    {
        SetAlertName(AlertName.PumpWeak_3.c_str()); // Set alert name to pump weak 3
        Alert.Enable = true;                        // Enable alert occurrence
    }
    else
    {
        SetAlertName(AlertName.No.c_str());
    }
}

/**********************
 * LoRa ALERT PUBLISHING
 * Identifies alert type and triggers corresponding LoRa communication
 **********************/
long AlertPublishTime = 0; // Variable to track last alert publish time

void ALERTFUNCTION::LoraPublishAlert()
{
    if (!Alert.Enable || start_init != 1)
        return; // Do not publish alerts if system is not initialized or alert is not enabled

    if (millis() - AlertPublishTime > 10000) // Check if 10 seconds have passed since last alert publish
    {
        AlertPublishTime = millis(); // Update last publish time

        if (strstr(Alert.Name, AlertName.Overload.c_str())) // Check if alert is overload type
            LoRa.OverloadAlert();
        else if (strstr(Alert.Name, AlertName.ReservoirLowLevel.c_str())) // Check if alert is reservoir type
            LoRa.ReservoirLowLevelAlert();
        else if (strstr(Alert.Name, AlertName.ProximityNotSense.c_str())) // Check if proximity alert
            LoRa.SignalNotDetected();
        else if (strstr(Alert.Name, AlertName.Supervisory_1.c_str()) || strstr(Alert.Name, AlertName.Supervisory_2.c_str()) || strstr(Alert.Name, AlertName.Supervisory_3.c_str())) // Check if proximity alert
            LoRa.SupervisoryALert();                                                                                                                                                // Call supervisory alert LoRa function
        else if (strstr(Alert.Name, AlertName.PumpWeak_1.c_str()) || strstr(Alert.Name, AlertName.PumpWeak_2.c_str()) || strstr(Alert.Name, AlertName.PumpWeak_3.c_str()))          // Check if pump weak alert
            LoRa.PumpWeakAlert();
    }
    else
    {
        return; // Skip publishing if 10 seconds have not passed to prevent flooding
    }
}