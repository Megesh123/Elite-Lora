/**
 * @file Main.cpp
 * @author Megeshwaran D <megesh@bfes.co.in>
 * @version 1.0.0
 * @date 2025-10-01
 * @copyright Copyright (c) 2025 [Blackfox Embedded Solutions]
 *
 * @note Test run page backend logic and user interactions
 */

#include "Headerfile.h"

/**********************
 * TEST RUN PAGE FUNCTION
 * Manages test run page logic and user interactions
 **********************/
bool _one_print_1 = false; // Flag to ensure one-time printing of test run status
bool _one_print_2 = false; // Flag to ensure one-time printing of test run status
bool _one_print_3 = false; // Flag to ensure one-time printing of test run status

void TESTRUNPAGE::Page()
{

    if (TestRunEnable) // If test run mode is enabled
    {
        if (Button.ok.isPressed() && start_init == 1) // If OK button is pressed and initialization is complete
        {
            GreaseOut.TestRun.Enable = !GreaseOut.TestRun.Enable; // Toggle test run enable state

            if (GreaseOut.TestRun.Enable) // If initialization is complete
            {
                Grease.TestRunOn(); // Activate test run mode
            }
            else
            {
                Grease.TestRunOff();                                            // Activate test run mode
                GreaseOut.Lubrication.Temp.Grams = GreaseOut.Lubrication.Grams; // Update temporary lubrication grams
                GreaseOut.Lubrication.Live.Grams = GreaseOut.Lubrication.Grams; // Update live lubrication grams

                GreaseOut.PauseTime.Temp.Hour = GreaseOut.PauseTime.Hour; // Update temporary pause time hours
                GreaseOut.PauseTime.Live.Hour = GreaseOut.PauseTime.Hour; // Update live pause time hours

                GreaseOut.PauseTime.Temp.Minute = GreaseOut.PauseTime.Minute; // Update temporary pause time minutes
                GreaseOut.PauseTime.Live.Minute = GreaseOut.PauseTime.Minute; // Update live pause time minutes

                GreaseOut.TestRun.sec = TEST_RUN_SEC;
                GreaseOut.TestRun.min = TEST_RUN_MIN;

                oldStateCount = 0; // Reset old state count
                SupervisoryTime.AlertCheckOutGrease = 0;
                SupervisoryTime.Supervisory1_LimitSwitch_Pressed = false;
                SupervisoryTime.Supervisory2_LimitSwitch_Pressed = false;
                SupervisoryTime.Supervisory3_LimitSwitch_Pressed = false;
                GreaseOut.Lubrication.Enable = false; // Enable lubrication
                GreaseOut.PauseTime.Enable = false;   // Disable pause time
                GreaseOut.TestRun.Enable = false;     // Disable test run
            }
            Button.ok.wait(); // Wait for button release
        }
        else if (Button.ok.isPressed()) // If OK button is pressed but initialization is not complete
        {
            TestRunEnableError = true; // Set test run enable error flag
            Button.ok.wait();          // Wait for button release
        }
    }
}
