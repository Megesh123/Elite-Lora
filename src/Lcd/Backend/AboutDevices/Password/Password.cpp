#include "HeaderFile.h"
#include "esp_task_wdt.h"

ADMINSETTING Admin;

void ADMINSETTING::page()
{
    if (AdminSetting.Enable)
    {
        DisplayEnterPassword();
        display.display();
        CurrentMillis = millis(); // Get the current time
        AdminSetting.EnterPassword = true;
        while (AdminSetting.EnterPassword)
        {
            CurrentMillis = millis();

            // Refresh the display
            DisplayEnterPassword();
            _Blink_Current_Digit();
            // Handle OK button action
            _ok();

            // page() runs inside the subscribed LCDDisplay task.  Keep the
            // task cooperative while the user remains on this screen.
            esp_task_wdt_reset();
            vTaskDelay(pdMS_TO_TICKS(10));
        }

        // Clear display after exiting the loop
        if (!AdminSetting.EnterPassword)
        {
            display.clearDisplay();
        }
    }
}

bool ADMINSETTING::_up()
{
    bool buttonHandled = false;

    // Check for Up button press
    if (Button.up.isPressed())
    {
        buttonHandled = true; // Indicate a button press occurred

        // If button is pressed and the interval has passed, perform the action
        if (CurrentMillis - ButtonPressMillis >= ButtonInterval)
        {
            ButtonPressMillis = CurrentMillis; // Update the last button action time
            AdminSetting.EnteredPassword[CurrentDigit]++;
            if (AdminSetting.EnteredPassword[CurrentDigit] > 9)
                AdminSetting.EnteredPassword[CurrentDigit] = 0; // Loop around digits 0-9
        }
    }

    return buttonHandled;
}

bool ADMINSETTING::_down()
{
    bool buttonHandled = false;

    // Check for Down button press
    if (Button.down.isPressed())
    {
        buttonHandled = true; // Indicate a button press occurred

        // If button is pressed and the interval has passed, perform the action
        if (CurrentMillis - ButtonPressMillis >= ButtonInterval)
        {
            ButtonPressMillis = CurrentMillis; // Update the last button action time
            AdminSetting.EnteredPassword[CurrentDigit]--;
            if (AdminSetting.EnteredPassword[CurrentDigit] < 0)
                AdminSetting.EnteredPassword[CurrentDigit] = 9; // Loop around digits 0-9
        }
    }

    return buttonHandled;
}

void ADMINSETTING::_blink()
{
    // Manage blinking with separate on/off durations
    if (BlinkState && (CurrentMillis - PreviousMillis >= OnTime))
    {
        BlinkState = false;             // Turn off the blink
        PreviousMillis = CurrentMillis; // Update the last toggle time
    }
    else if (!BlinkState && (CurrentMillis - PreviousMillis >= OffTime))
    {
        BlinkState = true;              // Turn on the blink
        PreviousMillis = CurrentMillis; // Update the last toggle time
    }
}

bool ADMINSETTING::_back()
{
    if (Button.back.isPressed())
    {
        Button.back.wait(); // Wait for button release without busy-spinning

        // Reset the entered password and current digit
        for (int i = 0; i < 6; i++)
        {
            AdminSetting.EnteredPassword[i] = 0; // Reset each digit to 0
        }

        CurrentDigit = 0;
        MainEnable = true;
        AboutDevices.Enable = false;
        AdminSetting.EnterPassword = false; // Exit the password entry mode

        return true; // Indicate that the Back button was pressed
    }
    return false; // Indicate that the Back button was not pressed
}

bool ADMINSETTING::_menu()
{
    if (Button.menu.isPressed())
    {
        Button.menu.wait(); // Wait for button release without busy-spinning

        CurrentDigit++;
        if (CurrentDigit > 5)
            CurrentDigit = 0; // Loop back to the first digit

        return true; // Button was pressed
    }
    return false; // Button was not pressed
}

void ADMINSETTING::_ok()
{
    if (Button.ok.isPressed())
    {
        Button.ok.wait(); // Wait for button release without busy-spinning

        // Check if the entered password matches the set password
        long int enteredPass = 0;
        for (int i = 0; i < 6; i++)
        {
            enteredPass = enteredPass * 10 + AdminSetting.EnteredPassword[i];
        }
        CorrectPassword(enteredPass);
        WorngPassword(enteredPass);
    }
}

void ADMINSETTING::_Blink_Current_Digit()
{
    // Check button actions
    bool isButtonPressed = _up() || _down() || _menu() || _back(); // Check if any button is pressed

    if (!isButtonPressed) // Only handle blinking if no button is pressed
    {
        _blink(); // Manage blinking logic
    }
    else
    {
        BlinkState = true; // Ensure the digit is always displayed during button presses
    }

    for (int i = 0; i < 6; i++)
    {
        display.setCursor(35 + (i * 10), 40);

        if (i == CurrentDigit)
        {
            if (BlinkState) // Show the digit during the "on" state or when a button is pressed
            {
                display.print(AdminSetting.EnteredPassword[i]);
            }
            else
            {
                display.print(" "); // Hide the digit during the "off" state
            }
        }
        else
        {
            display.print(AdminSetting.EnteredPassword[i]); // Always show non-highlighted digits
        }
    }
    display.display();
}

void ADMINSETTING::CorrectPassword(long int enteredPass)
{
    if (enteredPass == PASSWORD)
    {
        AdminSetting.PasswordCorrect = true;
        AdminSetting.EnterPassword = false;
        GreaseOut.Lubrication.Enable = false;
        GreaseOut.PauseTime.Enable = false;
        TestrunPage = false;
        Motor.off();
        while (AdminSetting.PasswordCorrect && AdminSetting.Enable)
        {

            // Display ALS configuration screen
            AboutDevice.Page();
            AboutDevice.SetCursor();

            // Exit configuration screen if Back is pressed
            if (Button.back.isPressed())
            {
                Button.back.wait(); // Wait for button release without busy-spinning

                AdminSetting.PasswordCorrect = false;
                MainEnable = true;
                AdminSetting.Enable = false;
                AboutDevices.Enable = false;
                for (int i = 0; i < 6; i++)
                {
                    AdminSetting.EnteredPassword[i] = 0; // Reset each digit to 0
                }
                CurrentDigit = 0;

                if (start_init == 1)
                {

                    if (check_refill_status)
                    {
                        VariableUpdate();
                    }
                    else
                    {
                        GreaseOut.Reservoir.Percentage = 0; // Set reservoir percentage to 0%
                        GreaseOut.Reservoir.Temp.Percentage = 0;
                        GreaseOut.Reservoir.Temp._old_reservoir_value = -1;
                        AlertFun.reservoir(); // Trigger reservoir low level alert
                    }

                    vTaskDelay(pdMS_TO_TICKS(10)); // Yield to other tasks with a delay of 10 milliseconds
                    // ESP.restart(); // Restart system to apply new configuration
                    display.clearDisplay();
                }

                // The configuration page can remain open indefinitely, so it
                // must service the LCD task watchdog as well.
                esp_task_wdt_reset();
                vTaskDelay(pdMS_TO_TICKS(10));
            }
        }
    }
}
void ADMINSETTING::WorngPassword(long int enteredPass)
{
    if (enteredPass != PASSWORD)
    {
        DisplayWorngPassword();
        AdminSetting.EnterPassword = false;
        for (int i = 0; i < 6; i++)
        {
            AdminSetting.EnteredPassword[i] = 0; // Reset each digit to 0
        }
        CurrentDigit = 0;
    }
}
