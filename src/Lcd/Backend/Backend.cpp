/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note Blink and Variable Increment/Decrement utility functions for UI management.
 */

#include "HeaderFile.h"
#include "esp_task_wdt.h"

/**********************
 * BLINK FUNCTION
 * Manages blinking behavior for UI elements
 **********************/
void BlinkFunction(bool &value)
{
    static unsigned long lastToggleTime = 0;   // Timestamp of the last toggle
    static bool isOn = true;                   // Current state of the blink (ON/OFF)
    unsigned long now = millis();              // Current time
    unsigned long interval = isOn ? 200 : 800; // OFF=200ms, ON=800ms

    if (now - lastToggleTime >= interval) // Time to toggle state
    {
        isOn = !isOn;         // switch state
        value = isOn;         // update output
        lastToggleTime = now; // Update last toggle time
    }
}

/**********************
 * VARIABLE COUNT INCREMENT/DECREMENT FUNCTION
 * Manages incrementing/decrementing variable values with button presses
 **********************/
template <typename T>
void VariableCountIncDec(T &value, T min, T max, T IncDec, bool IncDecSpeed, bool &Blink)
{
    int OldValues = value;                       // Store old value for comparison
    bool CheckMenuSettingEditTrue = false;       // Flag to check if menu setting edit is true
    if (!MenuSettingEditTrue && start_init == 1) // If menu setting edit is not true and initialization is complete
    {
        CheckMenuSettingEditTrue = true; // Set check flag to true
    }

    VariableSeleteEnable = true; // Enable variable selection
    if (IncDecSpeed)
    {
        speedEnable = true; // Enable speed increment/decrement
    }
    else
    {
        speedEnable = false; // Disable speed increment/decrement
    }
    while (VariableSeleteEnable) // While variable selection is enabled
    {
        BlinkFunction(Blink);
        if (Button.up.isPressed() && Button.menu.isNotPressed() && Button.ok.isNotPressed() && Button.back.isNotPressed() && Button.down.isNotPressed()) // If up button is pressed
        {
            Blink = false;                // Disable blink
            if (CheckMenuSettingEditTrue) // If check flag is true
            {
                MenuSettingEditTrue = true; // Set menu setting edit to true
            }
            // IncrementDecrement(value, min, max, IncDec);

            while (Button.up.isPressed() && Button.menu.isNotPressed() && Button.ok.isNotPressed() && Button.back.isNotPressed() && Button.down.isNotPressed()) // While up button is pressed
            {
                IncrementDecrement(value, min, max, IncDec); // Increment/decrement value
                esp_task_wdt_reset();
            }
        }
        if (Button.down.isPressed() && Button.menu.isNotPressed() && Button.ok.isNotPressed() && Button.back.isNotPressed() && Button.up.isNotPressed()) // If down button is pressed
        {
            Blink = false;                // Disable blink
            if (CheckMenuSettingEditTrue) // If check flag is true
            {
                MenuSettingEditTrue = true; // Set menu setting edit to true
            }
            // IncrementDecrement(value, min, max, -IncDec);
            while (Button.down.isPressed() && Button.menu.isNotPressed() && Button.ok.isNotPressed() && Button.back.isNotPressed() && Button.up.isNotPressed()) // While down button is pressed
            {
                IncrementDecrement(value, min, max, -IncDec); // Increment/decrement value
                esp_task_wdt_reset();
            }
        }

        if ((Button.back.isPressed() || Button.ok.isPressed()) && Button.menu.isNotPressed() && Button.up.isNotPressed() && Button.down.isNotPressed()) // If back or ok button is pressed
        {
            VariableSeleteEnable = false;                       // Disable variable selection
            Blink = false;                                      // Disable blink
            speedcount = 0;                                     // Reset speed count
            speed = 400;                                        // Reset speed
            speedEnable = false;                                // Disable speed increment/decrement
            if (OldValues == value && CheckMenuSettingEditTrue) // If value has not changed and check flag is true
            {
                MenuSettingEditTrue = false; // Set menu setting edit to false
            }
            Button.back.wait(); // Wait for button release
            Button.ok.wait();   // Wait for button release
        }
        speed = 400; // Reset speed
        esp_task_wdt_reset();
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

/**********************
 * INCREMENT/DECREMENT FUNCTION
 * Manages incrementing/decrementing variable values with speed control
 **********************/
template <typename T>
void IncrementDecrement(T &value, T min, T max, T IncDec)
{
    value += IncDec; // Increment/decrement value
    if (value > max) // If value exceeds max
        value = min; // Wrap around to min
    if (value < min) // If value is less than min
        value = max; // Wrap around to max

    if (speedEnable) // If speed increment/decrement is enabled
    {
        speedcount++;                            // Increment speed count
        if (speedcount > 10 && speedcount <= 20) // If speed count is between 10 and 20
        {
            speed = 300; // Set speed to 300ms
        }
        else if (speedcount > 20 && speedcount <= 30) // If speed count is between 20 and 30
        {
            speed = 200; // Set speed to 200ms
        }
        else if (speedcount > 30) // If speed count is greater than 30
        {
            speed = 150;     // Set speed to 150ms
            speedcount = 31; // Cap speed count
        }
        vTaskDelay(pdMS_TO_TICKS(speed)); // Delay based on speed
    }
    else
    {
        Button.up.wait();   // Wait for button release
        Button.down.wait(); // Wait for button release
    }
}

void VariableUpdate()
{
    Serial.println("VariableUpdate called");

    if (Alert.Enable)
    {
        if (LoraInitialized && LoraJoined)
        {
            DateTime now = rtc.getDateTime();
            char buffer[100];
            sprintf(buffer, "*ALERTRECTIFIED - %02d/%02d/%02d %02d:%02d:%02d#\n", now.date, now.month, now.year, now.hour, now.minute, now.second, Alert.Name);
            // Serial.println(buffer); // Print to Serial for debugging
            Lora_twp.AlertClear_Data = buffer;
            Lora_twp.AlertClear = true;
        }
        else
        {
            if (flash.OnceWrite) // Check if flash write is allowed
            {
                DateTime now = rtc.getDateTime();
                char buffer[100];
                sprintf(buffer, "*ALERTRECTIFIED - %02d/%02d/%02d %02d:%02d:%02d#\n", now.date, now.month, now.year, now.hour, now.minute, now.second, Alert.Name);
                // Serial.println(buffer);  // Print to Serial for debugging
                Flash.write(buffer);     // Store in flash
                flash.OnceWrite = false; // Prevent duplicate writes
            }
        }
    }

    GreaseOut.Proximity.Seconds = 0; // Reset proximity seconds
    Alert.Enable = false;            // Disable alerts
    SetAlertName("NO");
    Alert.NameCount = 0; // Reset alert name count
    Alert.Total.enable = true;
    MenuResetnotCompleted = false; // Reset menu reset not completed flag
    MenuSettingEditTrue = false;   // Reset menu setting edit true flag

    GreaseOut.Lubrication.Temp.Grams = GreaseOut.Lubrication.Grams; // Update temporary lubrication grams
    GreaseOut.Lubrication.Live.Grams = GreaseOut.Lubrication.Grams; // Update live lubrication grams

    GreaseOut.PauseTime.Temp.Hour = GreaseOut.PauseTime.Hour; // Update temporary pause time hours
    GreaseOut.PauseTime.Live.Hour = GreaseOut.PauseTime.Hour; // Update live pause time hours

    GreaseOut.PauseTime.Temp.Minute = GreaseOut.PauseTime.Minute; // Update temporary pause time minutes
    GreaseOut.PauseTime.Live.Minute = GreaseOut.PauseTime.Minute; // Update live pause time minutes

    GreaseOut.TestRun.sec = TEST_RUN_SEC;
    GreaseOut.TestRun.min = TEST_RUN_MIN;

    MenuSettingEnableDisableCheck = false;
    oldStateCount = 0; // Reset old state count
    SupervisoryTime.AlertCheckOutGrease = 0;
    SupervisoryTime.Supervisory1_LimitSwitch_Pressed = false;
    SupervisoryTime.Supervisory2_LimitSwitch_Pressed = false;
    SupervisoryTime.Supervisory3_LimitSwitch_Pressed = false;
    // Also clear the supervisory average accumulators. If these stay >=
    // SUPERVISORY_AVERAGE_GRAM, SupervisoryTask immediately re-raises a
    // "PUMP WEAK" alert after lubrication is re-enabled and _FunctionStop()
    // disables every mode again (main page then shows "MODE: -"). The
    // Menu > Reset path already cleared these before calling VariableUpdate();
    // doing it here makes every reset path behave the same.
    SupervisoryTime.SupervisoryAverageTakeOutGrease_1 = 0;
    SupervisoryTime.SupervisoryAverageTakeOutGrease_2 = 0;
    SupervisoryTime.SupervisoryAverageTakeOutGrease_3 = 0;
    flash.OnceWrite = true;

    Led.off();        // Turn off alert LED
    Reservoir.read(); // Refresh reservoir status (may re-raise a genuine low-level alert)
    Reset_SendData_Lora = true;
    start_init = 1;
    // ...then re-arm the lubrication process as the very last step, atomically,
    // so no other task observes a half-updated state and nothing (flash write,
    // vTaskDelay) runs between enabling lubrication and returning - that gap is
    // what let AlertTask/GreaseOutTask stomp the re-enable before.
    STATE_LOCK();
    GreaseOut.Lubrication.Enable = true; // Enable lubrication
    GreaseOut.PauseTime.Enable = false;  // Disable pause time
    GreaseOut.TestRun.Enable = false;    // Disable test run
    STATE_UNLOCK();
    Motor.on(); // Turn on motor

    Eeprom.write();                // Persist the cleared state first...
    vTaskDelay(pdMS_TO_TICKS(10)); // Yield to other tasks with a delay of 10 milliseconds
}

template void VariableCountIncDec<float>(float &, float, float, float, bool, bool &);
template void VariableCountIncDec<int>(int &, int, int, int, bool, bool &);
// template void VariableCountIncDec<float>(float&, float, float, float, bool, bool&);
// template void VariableCountIncDec<int>(int&, int, int, int, bool, bool&);
